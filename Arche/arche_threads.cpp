/********************************************************************************
 *  File Name:
 *      threads.hpp
 *
 *  Description:
 *      Lists all the threads that are run on the Arche system
 *
 *  2019 | Brandon Braun | brandonbraun653@gmail.com
 ********************************************************************************/

#include <array>

#include <Arche/arche_threads.hpp>

namespace Arche
{
  static constexpr std::array<TaskHandle_t, numThreads> threadHandles{};

  const Chimera::Threading::Thread_t threads[ numThreads ] = {
    { HeartBeat::threadHeartbeat, threadHandles[ 0 ], HeartBeat::threadPriority, nullptr, HeartBeat::RTOS_STACK_DEPTH },
    { IOP::iopSerial, threadHandles[ 1 ], IOP::threadPriority, nullptr, IOP::RTOS_STACK_DEPTH },
    { SYS::sysMain, threadHandles[ 2 ], SYS::threadPriority, nullptr, SYS::RTOS_STACK_DEPTH }
  };
}
