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
  void threadGet( void *argument )
  {
    Chimera::Threading::signalSetupComplete();

    /*------------------------------------------------
    Post-initialization, get the handle to the serial root thread
    ------------------------------------------------*/
    TaskHandle_t dispatchThread = xTaskGetHandle( threadName_commandDispatch );

    std::string msg = "Hello from: getThread\r\n";

    for ( ;; )
    {
      /*------------------------------------------------
      Wait for another thread to wake us up to do work
      ------------------------------------------------*/
      if ( ulTaskNotifyTake( pdTRUE, portMAX_DELAY ) == TaskMessage::WAKEUP )
      {
        if ( serial.reserve( 0 ) == Chimera::CommonStatusCodes::OK )
        {
          serial.write( reinterpret_cast<const uint8_t *>( msg.data() ), msg.length() );

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