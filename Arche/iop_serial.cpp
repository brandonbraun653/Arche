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
#include <Arche/arche_threads.hpp>
#include <Arche/config/serial.hpp>
#include <Arche/config/memory.hpp>

/* Chimera Includes */
#include <Chimera/threading.hpp>

namespace Arche
{
  namespace IOP
  {
    void iopSerial(void *argument)
    {

      Chimera::Threading::signalThreadSetupComplete();

      for (;;)
      {

        Chimera::delayMilliseconds(100);
      }
    }
  }
}

