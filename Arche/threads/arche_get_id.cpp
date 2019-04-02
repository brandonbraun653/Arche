/********************************************************************************
 *   File Name:
 *    arche_get_id.cpp
 *
 *   Description:
 *    Implements the bootloader Get ID command
 *
 *   2019 | Brandon Braun | brandonbraun653@gmail.com
 ********************************************************************************/

/* Arche Includes */
#include <Arche/arche.hpp>
#include <Arche/threads.hpp>
#include <Arche/commands.hpp>

/* Chimera Includes */
#include <Chimera/chimera.hpp>
#include <Chimera/threading.hpp>
#include <Chimera/system.hpp>

namespace Arche
{
  void threadGetID( void *argument )
  {
    static constexpr uint8_t numBytes     = 2;
    static constexpr uint32_t DMADelay_mS = 5;
    
    Chimera::System::Identifier system;
    std::array<uint8_t, 3> getIDPayload{};

    Chimera::Threading::signalSetupComplete();

    /*------------------------------------------------
    Post-initialization, get the handle to the serial
    root thread so we can wake it up later.
    ------------------------------------------------*/
    TaskHandle_t dispatchThread = xTaskGetHandle( threadName_commandDispatch );

    for ( ;; )
    {
      /*------------------------------------------------
      Wait for another thread to wake us up to do work
      ------------------------------------------------*/
      if ( ulTaskNotifyTake( pdTRUE, portMAX_DELAY ) == TaskMessage::WAKEUP )
      {
        if ( serial.reserve( Chimera::Threading::TIMEOUT_DONT_WAIT ) == Chimera::CommonStatusCodes::OK )
        {
          /*------------------------------------------------
          Protocol Step 1: Send ACK byte
          ------------------------------------------------*/
          Command::sendACKByte();
          Chimera::delayMilliseconds( DMADelay_mS );

          /*------------------------------------------------
          Protocol Step 2: Send the number of bytes + device ID
          ------------------------------------------------*/
          uint16_t deviceID = static_cast<uint16_t>( system.deviceID() );
          getIDPayload[ 0 ] = numBytes;
          getIDPayload[ 1 ] = static_cast<uint8_t>( ( deviceID & 0xFF00 ) >> 8 );
          getIDPayload[ 2 ] = static_cast<uint8_t>( ( deviceID & 0x00FF ) );

          serial.write( getIDPayload.data(), getIDPayload.size() );
          Chimera::delayMilliseconds( DMADelay_mS );

          /*------------------------------------------------
          Protocol Step 3: Send ACK byte
          ------------------------------------------------*/
          Command::sendACKByte();
          Chimera::delayMilliseconds( DMADelay_mS );

          /*------------------------------------------------
          We are done with the command sequence. Go back to the dispatch thread.
          ------------------------------------------------*/
          serial.release();
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