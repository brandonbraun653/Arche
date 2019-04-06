/********************************************************************************
 *  File Name:
 *      threads.hpp
 *
 *  Description:
 *      Lists all the threads that are run on the Arche system
 *
 *  2019 | Brandon Braun | brandonbraun653@gmail.com
 ********************************************************************************/

/* C++ Includes */
#include <array>

/* Arche Includes */
#include <Arche/threads.hpp>

#define RTOS_STACK_DEPTH( x ) ( x / sizeof( configSTACK_DEPTH_TYPE ) )

namespace Arche
{
  /* clang-format off */
  std::array<Chimera::Threading::Thread_t, 15> threads = {{
    /* Thread Function     |  Sys Handle  |  Priority  |    Arg    |    Stack Depth In Bytes    |   Thread Name     */
    { threadHeartbeat,          nullptr,        2,        nullptr,    RTOS_STACK_DEPTH( 1000 ),   "heartbeat"       }, // 0
    { commandDispatch,          nullptr,        5,        nullptr,    RTOS_STACK_DEPTH( 1000 ),   "cmdDispatch"     }, // 1
    { sysMain,                  nullptr,        4,        nullptr,    RTOS_STACK_DEPTH( 1000 ),   "sysMain"         }, // 2
    { threadGet,                nullptr,        3,        nullptr,    RTOS_STACK_DEPTH( 250 ),    "Get"             }, // 3
    { threadGetVer,             nullptr,        3,        nullptr,    RTOS_STACK_DEPTH( 250 ),    "GetVer"          }, // 4
    { threadGetID,              nullptr,        3,        nullptr,    RTOS_STACK_DEPTH( 250 ),    "GetID"           }, // 5
    { threadReadMem,            nullptr,        3,        nullptr,    RTOS_STACK_DEPTH( 250 ),    "ReadMem"         }, // 6
    { threadGo,                 nullptr,        3,        nullptr,    RTOS_STACK_DEPTH( 250 ),    "Go"              }, // 7
    { threadWriteMem,           nullptr,        3,        nullptr,    RTOS_STACK_DEPTH( 250 ),    "WriteMem"        }, // 8
    { threadErase,              nullptr,        3,        nullptr,    RTOS_STACK_DEPTH( 250 ),    "Erase"           }, // 9
    { threadExtendedErase,      nullptr,        3,        nullptr,    RTOS_STACK_DEPTH( 250 ),    "ExtendedErase"   }, // 10
    { threadWriteProtect,       nullptr,        3,        nullptr,    RTOS_STACK_DEPTH( 250 ),    "WriteProtect"    }, // 11
    { threadWriteUnprotect,     nullptr,        3,        nullptr,    RTOS_STACK_DEPTH( 250 ),    "WriteUnprotect"  }, // 12
    { threadReadoutProtect,     nullptr,        3,        nullptr,    RTOS_STACK_DEPTH( 250 ),    "ReadProtect"     }, // 13
    { threadReadoutUnprotect,   nullptr,        3,        nullptr,    RTOS_STACK_DEPTH( 250 ),    "ReadUnprotect"   }  // 14
  }};
  /* clang-format on */

  /*------------------------------------------------
  Easily look up thread names for interacting with FreeRTOS
  (index must match threads table above)
  ------------------------------------------------*/
  const char *threadName_heartbeat        = &threads[ 0 ].name[ 0 ];
  const char *threadName_commandDispatch  = &threads[ 1 ].name[ 0 ];
  const char *threadName_sysMain          = &threads[ 2 ].name[ 0 ];
  const char *threadName_get              = &threads[ 3 ].name[ 0 ];
  const char *threadName_getVer           = &threads[ 4 ].name[ 0 ];
  const char *threadName_getID            = &threads[ 5 ].name[ 0 ];
  const char *threadName_readMem          = &threads[ 6 ].name[ 0 ];
  const char *threadName_go               = &threads[ 7 ].name[ 0 ];
  const char *threadName_writeMem         = &threads[ 8 ].name[ 0 ];
  const char *threadName_erase            = &threads[ 9 ].name[ 0 ];
  const char *threadName_extendedErase    = &threads[ 10 ].name[ 0 ];
  const char *threadName_writeProtect     = &threads[ 11 ].name[ 0 ];
  const char *threadName_writeUnprotect   = &threads[ 12 ].name[ 0 ];
  const char *threadName_readoutProtect   = &threads[ 13 ].name[ 0 ];
  const char *threadName_readoutUnprotect = &threads[ 14 ].name[ 0 ];
}
