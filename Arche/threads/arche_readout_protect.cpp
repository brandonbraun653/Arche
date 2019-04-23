/********************************************************************************
 *   File Name:
 *    arche_readout_protect.cpp
 *
 *   Description:
 *    Implements the bootloader Reaout Protect command
 *
 *   2019 | Brandon Braun | brandonbraun653@gmail.com
 ********************************************************************************/

/* Arche Includes */
#include <Arche/threads.hpp>
#include <Arche/utilities.hpp>

/* Chimera Includes */
#include <Chimera/chimera.hpp>
#include <Chimera/threading.hpp>

namespace Arche
{
  static bool readProtectionEnabled = false;
  
  void threadReadoutProtect( void *argument )
  {
    Chimera::Threading::signalSetupComplete();

    for ( ;; )
    {
      Chimera::delayMilliseconds( 100 );
    }
  }


  bool isRDPActive()
  {
    return readProtectionEnabled;
  }
}