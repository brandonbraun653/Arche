/********************************************************************************
 *   File Name:
 *    arche_get_ver.cpp
 *
 *   Description:
 *    Implements the bootloader Get Version command
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

namespace Arche
{
  /*------------------------------------------------
  Figure 5: AN3155 Rev 9, pg. 11
  ------------------------------------------------*/
  static constexpr std::array<uint8_t, 3> getVerResponse = {
    Arche::BOOTLOADER_VERSION,
    0x00, /* Get Command */
    0x00  /* Get Version and Read Protection Status */
  };
  
  void threadGetVer( void *argument )
  {
    /*------------------------------------------------
    The DMA hardware is usually too fast for the serial port and
    will overload it. This small time buffer proved useful in
    stabilizing the system.
    ------------------------------------------------*/
    static constexpr uint32_t DMADelay_mS = 5;

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
          Protocol Step 2: Send the number of bytes + version + supported commands
          ------------------------------------------------*/
          serial.write( getVerResponse.data(), getVerResponse.size() );
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