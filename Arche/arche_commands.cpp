/********************************************************************************
 *   File Name:
 *    commands.cpp
 *
 *   Description:
 *    Implements various resources associated with Arche commands
 *
 *   2019 | Brandon Braun | brandonbraun653@gmail.com
 ********************************************************************************/

/* C++ Includes */

/* Arche Includes */
#include <Arche/arche.hpp>
#include <Arche/commands.hpp>
#include <Arche/threads.hpp>
#include <Arche/config/serial.hpp>

namespace Arche
{
  namespace Command
  {
    /*------------------------------------------------
    The DMA hardware is usually too fast for the serial port and
    will overload it. This small time buffer proved useful in
    stabilizing the system.
    ------------------------------------------------*/
    static constexpr uint32_t DMADelay_mS = 5;
    
    /* clang-format off */
    const boost::container::flat_map<Command_t, uint8_t> threadIndex = { 
      { GET,                    3  },
      { GET_VER,                4  },
      { GET_ID,                 5  },
      { READ_MEM,               6  },
      { GO,                     7  },
      { WRITE_MEM,              8  },
      { ERASE,                  9  },
      { EXTENDED_ERASE,         10 },
      { WRITE_PROTECT,          11 },
      { WRITE_UNPROTECT,        12 },
      { READOUT_PROTECT,        13 },
      { READOUT_UNPROTECT,      14 }
    };
    /* clang-format on */

    void sendACKByte()
    {
      serial.write( &Arche::Config::Serial::ACK_BYTE, sizeof( Arche::Config::Serial::ACK_BYTE ) );
      Chimera::delayMilliseconds( DMADelay_mS );
    }

    void sendNACKByte()
    {
      serial.write( &Arche::Config::Serial::NACK_BYTE, sizeof( Arche::Config::Serial::NACK_BYTE ) );
      Chimera::delayMilliseconds( DMADelay_mS );
    }
  }
}