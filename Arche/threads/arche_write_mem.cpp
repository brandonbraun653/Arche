/********************************************************************************
 *   File Name:
 *    arche_write_mem.cpp
 *
 *   Description:
 *    Implements the bootloader Write Memory command
 *
 *   2019 | Brandon Braun | brandonbraun653@gmail.com
 ********************************************************************************/

/* Arche Includes */
#include <Arche/arche.hpp>
#include <Arche/commands.hpp>
#include <Arche/threads.hpp>
#include <Arche/utilities.hpp>

/* Chimera Includes */
#include <Chimera/chimera.hpp>
#include <Chimera/threading.hpp>

namespace Arche
{
  enum SMWriteMemory
  {
    WDM_RDP_IS_ACTIVE_CHECK,
    WDM_RX_START_ADDRESS,
    WDM_RX_BYTES_TO_WRITE,
    WDM_WRITE_DATA,
    WDM_WRITE_OPTIONS,
    WDM_SEND_NACK,
    WDM_EXIT,
    WDM_MAX_NUM_STATES
  };

  static constexpr uint32_t SM_LOOP_DELAY_MS    = 5; /* State machine loop delay */
  static constexpr uint32_t RX_ADDRESS_LENGTH   = 5; /* 4 address bytes + 1 checksum byte */
  static constexpr uint32_t RX_NUM_BYTES_LENGTH = 2; /* 1 length byte + 1 checksum byte */

  void threadWriteMem( void *argument )
  {
    Chimera::Threading::signalSetupComplete();

    /*------------------------------------------------
    Post-initialization, get the handle to the serial
    root thread so we can wake it up later.
    ------------------------------------------------*/
    TaskHandle_t dispatchThread = xTaskGetHandle( threadName_commandDispatch );

    while ( 1 )
    {
      /*------------------------------------------------
      Wait for another thread to wake us up to do work
      ------------------------------------------------*/
      if ( ulTaskNotifyTake( pdTRUE, portMAX_DELAY ) == TaskMessage::WAKEUP )
      {
        if ( serial.reserve( Chimera::Threading::TIMEOUT_DONT_WAIT ) == Chimera::CommonStatusCodes::OK )
        {
          uint8_t currentState = WDM_RDP_IS_ACTIVE_CHECK;
          uint8_t actualChecksum   = 0u;
          uint8_t expectedChecksum = 0u;
          uint8_t bytesToWrite     = 0u;
          uint16_t bytesToRead     = 0u;
          uint32_t startAddress    = 0u;
          size_t numSerialBytes    = 0;

          /*------------------------------------------------
          Implements the state machine on pg. 20 of AN3155 Rev 9
          ------------------------------------------------*/
          while ( currentState < WDM_MAX_NUM_STATES )
          {
            switch ( currentState )
            {
              case WDM_RDP_IS_ACTIVE_CHECK:
                if ( isRDPActive() )
                {
                  currentState = WDM_SEND_NACK;
                }
                else
                {
                  Command::sendACKByte();
                  currentState = WDM_RX_START_ADDRESS;
                }
                break;

              case WDM_RX_START_ADDRESS:
                if ( serial.available( &numSerialBytes ) && ( numSerialBytes >= RX_ADDRESS_LENGTH ) )
                {
                  /*------------------------------------------------
                  Read out the transfered data
                  ------------------------------------------------*/
                  memset( systemBuffer.data(), 0, RX_ADDRESS_LENGTH );
                  serial.readAsync( systemBuffer.data(), RX_ADDRESS_LENGTH );

                  /*------------------------------------------------
                  Calculate the checksum and pull out the start address
                  ------------------------------------------------*/
                  actualChecksum   = XORCheckSum( systemBuffer.data(), 0, RX_ADDRESS_LENGTH - 2 );
                  expectedChecksum = systemBuffer[ RX_ADDRESS_LENGTH - 1 ];
                  memcpy( &startAddress, systemBuffer.data(), ( RX_ADDRESS_LENGTH - 1 ) );

                  /*------------------------------------------------
                  Inform the host of the result
                  ------------------------------------------------*/
                  if ( ( actualChecksum == expectedChecksum ) && isAddressValid( startAddress ) )
                  {
                    serial.flush( Chimera::Serial::SubPeripheral::RX );
                    Command::sendACKByte();
                    currentState = WDM_RX_BYTES_TO_WRITE;
                  }
                  else
                  {
                    currentState = WDM_SEND_NACK;
                  }
                }
                break;

              case WDM_RX_BYTES_TO_WRITE:
                if ( serial.available( &numSerialBytes ) )
                {
                  /*------------------------------------------------
                  Figure out how many bytes we are going to be writing
                  ------------------------------------------------*/
                  systemBuffer[ 0 ] = 0u;
                  serial.readAsync( systemBuffer.data(), 1 );
                  bytesToWrite = systemBuffer[ 0 ]; /* 'N' according to the application note */
                  bytesToRead  = bytesToWrite + 1u; /* There is a checksum appended on the end */

                  /*------------------------------------------------
                  Wait for all the data to arrive
                  ------------------------------------------------*/
                  memset( systemBuffer.data(), 0, systemBuffer.size() );
                  numSerialBytes = 0;

                  do
                  {
                    serial.available( &numSerialBytes );

                    if ( numSerialBytes >= bytesToRead )
                    {
                      serial.readAsync( systemBuffer.data(), bytesToRead );
                      break;
                    }

                    Chimera::delayMilliseconds( SM_LOOP_DELAY_MS );
                  } while ( numSerialBytes < bytesToWrite );

                  /*------------------------------------------------
                  Calculate the checksum
                  ------------------------------------------------*/
                  actualChecksum   = XORCheckSum( systemBuffer.data(), 0, bytesToRead - 2 );
                  expectedChecksum = systemBuffer[ bytesToRead - 1 ];

                  /*------------------------------------------------
                  Decide how we are going to process the data
                  ------------------------------------------------*/
                  if ( actualChecksum != expectedChecksum )
                  {
                    currentState = WDM_SEND_NACK;
                  }
                  else
                  {
                    if ( isAddressValid( startAddress ) && !isAddressOptionBytes( startAddress ) )
                    {
                      currentState = WDM_WRITE_DATA;
                    }
                    else if ( isAddressOptionBytes( startAddress ) )
                    {
                      currentState = WDM_WRITE_OPTIONS;
                    }
                  }
                }
                break;

              case WDM_WRITE_DATA:
                flash.write( startAddress, systemBuffer.data(), bytesToWrite );
                currentState = WDM_EXIT;
                break;

              case WDM_WRITE_OPTIONS:
                // TODO: Eventually support this
                currentState = WDM_EXIT;
                break;

              case WDM_SEND_NACK:
                Command::sendNACKByte();
                serial.release();
                currentState = WDM_EXIT;
                break;

              case WDM_EXIT:
              default:
                currentState = WDM_MAX_NUM_STATES;
                break;
            }

            Chimera::delayMilliseconds( SM_LOOP_DELAY_MS );
          }
        }
        else
        {
          // TODO: Throw an assert to be logged here
        }

        /*------------------------------------------------
        Regardless of what happens, wake up the dispatch
        thread so more commands can be handled.
        ------------------------------------------------*/
        xTaskNotify( dispatchThread, TaskMessage::WAKEUP, eSetValueWithOverwrite );
      }
    }
  }
}