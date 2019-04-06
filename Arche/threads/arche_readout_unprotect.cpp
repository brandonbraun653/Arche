/********************************************************************************
 *   File Name:
 *    arche_readout_unprotect.cpp
 *
 *   Description:
 *    Implements the bootloader Readout Unprotect command
 *
 *   2019 | Brandon Braun | brandonbraun653@gmail.com
 ********************************************************************************/

/* Arche Includes */
#include <Arche/threads.hpp>

/* Chimera Includes */
#include <Chimera/chimera.hpp>
#include <Chimera/threading.hpp>

namespace Arche
{
  void threadReadoutUnprotect( void *argument )
  {
    Chimera::Threading::signalSetupComplete();

    for ( ;; )
    {
      Chimera::delayMilliseconds( 100 );
    }
  }
}