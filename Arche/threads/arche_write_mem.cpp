/********************************************************************************
 *   File Name:
 *    arche_write_mem.cpp
 *
 *   Description:
 *    Implements the bootloader Write Memory command
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
  void threadWriteMem( void *argument )
  {
    Chimera::Threading::signalSetupComplete();

    for ( ;; )
    {
      Chimera::delayMilliseconds( 100 );
    }
  }
}