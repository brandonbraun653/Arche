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
#include <Arche/config/watchdog.hpp>

/* Chimera Includes */
#include <Chimera/chimera.hpp>
#include <Chimera/threading.hpp>

Chimera::Watchdog::WatchdogClass watchdog;

namespace Arche
{
  namespace SYS
  {
    void sysMain(void *argument)
    {
      /*------------------------------------------------
      Start up the system watchdog
      ------------------------------------------------*/
      watchdog.initialize(Config::Watchdog::TIMEOUT_MS);
      watchdog.start();

      #if defined( DEBUG )
      watchdog.pauseOnDebugHalt(true);
      #endif 

      Chimera::Threading::signalSetupComplete();

      for (;;)
      {
        watchdog.kick();
        Chimera::delayMilliseconds(100);
      }
    }
  }
}

