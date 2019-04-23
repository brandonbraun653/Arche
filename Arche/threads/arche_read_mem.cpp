/********************************************************************************
 *   File Name:
 *    arche_read_mem.cpp
 *
 *   Description:
 *    Implements the bootloader Read Memory command
 *
 *   2019 | Brandon Braun | brandonbraun653@gmail.com
 ********************************************************************************/

/* C++ Includes */
#include <cmath>

/* Arche Includes */
#include <Arche/arche.hpp>
#include <Arche/commands.hpp>
#include <Arche/config/serial.hpp>
#include <Arche/threads.hpp>
#include <Arche/utilities.hpp>

/* Chimera Includes */
#include <Chimera/chimera.hpp>
#include <Chimera/memory.hpp>
#include <Chimera/threading.hpp>

Chimera::Memory::InternalFlash flash;

namespace Arche
{
  enum ReadMemorySM
  {
    RDM_RDP_IS_ACTIVE_CHECK,
    RDM_RX_START_ADDRESS,
    RDM_RX_NUM_BYTES_TO_READ,
    RDM_TRANSMIT_DATA_TO_HOST,
    RDM_SEND_NACK,
    RDM_EXIT,
    RDM_MAX_NUM_STATES
  };

  static constexpr uint32_t SM_LOOP_DELAY_MS    = 5;
  static constexpr uint32_t RX_ADDRESS_LENGTH   = 5; /* 4 address bytes + 1 checksum byte */
  static constexpr uint32_t RX_NUM_BYTES_LENGTH = 2; /* 1 length byte + 1 checksum byte */

  void threadReadMem( void *argument )
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
          uint8_t currentState     = RDM_RDP_IS_ACTIVE_CHECK;
          uint8_t actualChecksum   = 0u;
          uint8_t expectedChecksum = 0u;
          uint8_t bytesToRead      = 0u;
          uint32_t startAddress    = 0u;
          uint32_t transmitDelay   = 0u;
          size_t numSerialBytes    = 0;

          /*------------------------------------------------
          Implements the state machine on pg. 15 of AN3155 Rev 9
          ------------------------------------------------*/
          while ( currentState < RDM_MAX_NUM_STATES )
          {
            switch ( currentState )
            {
              case RDM_RDP_IS_ACTIVE_CHECK:
                if ( isRDPActive() )
                {
                  currentState = RDM_SEND_NACK;
                }
                else
                {
                  Command::sendACKByte();
                  currentState = RDM_RX_START_ADDRESS;
                }
                break;
                
              case RDM_RX_START_ADDRESS:
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
                  actualChecksum = XORCheckSum( systemBuffer.data(), 0, RX_ADDRESS_LENGTH - 2 );
                  expectedChecksum = systemBuffer[ RX_ADDRESS_LENGTH - 1 ];
                  memcpy( &startAddress, systemBuffer.data(), ( RX_ADDRESS_LENGTH - 1 ) );

                  /*------------------------------------------------
                  Inform the host of the result
                  ------------------------------------------------*/
                  if ( ( actualChecksum == expectedChecksum ) && isAddressValid( startAddress ) )
                  {
                    Command::sendACKByte();
                    currentState = RDM_RX_NUM_BYTES_TO_READ;
                  }
                  else
                  {
                    currentState = RDM_SEND_NACK;
                  }
                }
                break;
                
              case RDM_RX_NUM_BYTES_TO_READ:
                if ( serial.available( &numSerialBytes ) && ( numSerialBytes >= RX_NUM_BYTES_LENGTH ) )
                {
                  /*------------------------------------------------
                  Read out the transfered data
                  ------------------------------------------------*/
                  memset( systemBuffer.data(), 0, RX_NUM_BYTES_LENGTH );
                  serial.readAsync( systemBuffer.data(), RX_NUM_BYTES_LENGTH );

                  /*------------------------------------------------
                  Calculate the checksum and pull out the number of bytes to read
                  ------------------------------------------------*/
                  actualChecksum   = ~systemBuffer[ 0 ];
                  expectedChecksum = systemBuffer[ RX_NUM_BYTES_LENGTH - 1 ];
                  bytesToRead      = systemBuffer[ 0 ];

                  /*------------------------------------------------
                  Inform the host of the result
                  ------------------------------------------------*/
                  if ( actualChecksum == expectedChecksum )
                  {
                    Command::sendACKByte();
                    currentState = RDM_TRANSMIT_DATA_TO_HOST;
                  }
                  else
                  {
                    currentState = RDM_SEND_NACK;
                  }
                }
                break;
                
              case RDM_TRANSMIT_DATA_TO_HOST:
                /*------------------------------------------------
                Write the data to the host
                ------------------------------------------------*/
                memset( systemBuffer.data(), 0, bytesToRead );
                flash.read( startAddress, systemBuffer.data(), bytesToRead );
                serial.write( systemBuffer.data(), bytesToRead );

                /*------------------------------------------------
                Dynamically calculate the delay needed to allow the
                transfer to complete before releasing the hardware.
                ------------------------------------------------*/
                transmitDelay = static_cast<uint32_t>( ceil( Config::Serial::MS_PER_BYTE * static_cast<float>( bytesToRead ) ) );
                Chimera::delayMilliseconds(transmitDelay);
                currentState = RDM_EXIT;
                break;

              case RDM_SEND_NACK:
                Command::sendNACKByte();
                serial.release();
                currentState = RDM_EXIT;
                break;
                
              case RDM_EXIT:
              default:
                currentState = RDM_MAX_NUM_STATES;
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