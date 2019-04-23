/********************************************************************************
 *   File Name:
 *    arche_get.cpp
 *
 *   Description:
 *    Implements the bootloader Get command
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
  /* clang-format off */
  /*------------------------------------------------
  Figure 3: AN3155 Rev 9, pg. 9
  ------------------------------------------------*/
  static constexpr std::array<uint8_t, 13> getResponse = {
    11,   /* Number of bytes */
    Arche::BOOTLOADER_VERSION,
    0x00, /* Get Command */
    0x01, /* Get Version and Read Protection Status */
    0x02, /* Get ID */
    0x11, /* Read Memory Command */
    0x21, /* Go Command */
    0x31, /* Write Memory Command */
    0x43, /* Erase Command */
    0x63, /* Write Protect Command */
    0x73, /* Write Unprotect Command */
    0x82, /* Readout Protect Command */
    0x92  /* Readout Unprotect Command */
  };
  /* clang-format on */


  void threadGet( void *argument )
  {
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

          /*------------------------------------------------
          Protocol Step 2-4: Send the number of bytes + version + supported commands
          ------------------------------------------------*/
          serial.write( getResponse.data(), getResponse.size() );

          /*------------------------------------------------
          Protocol Step 5: Send ACK byte
          ------------------------------------------------*/
          Command::sendACKByte();

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