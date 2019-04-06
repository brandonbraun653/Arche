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
#include <Arche/threads.hpp>

/* Chimera Includes */
#include <Chimera/chimera.hpp>
#include <Chimera/threading.hpp>

namespace Arche
{
  void threadGetID( void *argument )
  {
    Chimera::Threading::signalSetupComplete();

    for ( ;; )
    {
      Chimera::delayMilliseconds( 100 );
    }
  }
}