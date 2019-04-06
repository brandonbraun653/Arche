/********************************************************************************
 *   File Name:
 *      sys_main.cpp
 *
 *   Description:
 *      Primary thread for executing the bootloader protocol
 *
 *   2019 | Brandon Braun | brandonbraun653@gmail.com
 ********************************************************************************/

/* Arche Includes */
#include <Arche/arche.hpp>
#include <Arche/threads.hpp>
#include <Arche/config/watchdog.hpp>

/* Chimera Includes */
#include <Chimera/chimera.hpp>
#include <Chimera/threading.hpp>

Chimera::Watchdog::WatchdogClass watchdog;

namespace Arche
{
  void sysMain( void *argument )
  {
    /*------------------------------------------------
    Start up the system watchdog
    ------------------------------------------------*/
    watchdog.initialize( Config::Watchdog::TIMEOUT_MS );
    watchdog.start();

#if defined( DEBUG )
    watchdog.pauseOnDebugHalt( true );
#endif

    Chimera::Threading::signalSetupComplete();

    /*------------------------------------------------
    Instruct the command dispatcher to start listening
    ------------------------------------------------*/
    TaskHandle_t dispatchThread = xTaskGetHandle( threadName_commandDispatch );
    xTaskNotify( dispatchThread, TaskMessage::WAKEUP, eSetValueWithOverwrite );

    for ( ;; )
    {
      watchdog.kick();
      Chimera::delayMilliseconds( 100 );
    }
  }
}  // namespace Arche
