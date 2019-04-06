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
 #include <Arche/commands.hpp>
 #include <Arche/threads.hpp>

namespace Arche
{
  namespace Command
  {
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
  }
}