/********************************************************************************
 *  File Name:
 *      threads.hpp
 *
 *  Description:
 *      Lists all the threads that are run on the Arche system by FreeRTOS
 *
 *  2019 | Brandon Braun | brandonbraun653@gmail.com
 ********************************************************************************/

#pragma once
#ifndef ARCHE_THREADS_HPP
#define ARCHE_THREADS_HPP

/* C++ Includes */
#include <array>
#include <cstdint>

/* Chimera Includes */
#include <Chimera/threading.hpp>

namespace Arche
{
  /*------------------------------------------------
  Thread function prototypes
  ------------------------------------------------*/
  void threadGet( void *argument );
  void threadGetVer( void *argument );
  void threadGetID( void *argument );
  void threadReadMem( void *argument );
  void threadGo( void *argument );
  void threadWriteMem( void *argument );
  void threadErase( void *argument );
  void threadExtendedErase( void *argument );
  void threadWriteProtect( void *argument );
  void threadWriteUnprotect( void *argument );
  void threadReadoutProtect( void *argument );
  void threadReadoutUnprotect( void *argument );
  void threadHeartbeat( void *argument );
  void commandDispatch(void *argument);
  void sysMain(void *argument);

  /*------------------------------------------------
  Registry of threads booting with the system
  ------------------------------------------------*/
  extern std::array<Chimera::Threading::Thread_t, 15> threads;

  /*------------------------------------------------
  Easily look up thread names for interacting with FreeRTOS
  ------------------------------------------------*/
  extern const char *threadName_heartbeat;
  extern const char *threadName_commandDispatch;
  extern const char *threadName_sysMain;
  extern const char *threadName_get;
  extern const char *threadName_getVer;
  extern const char *threadName_getID;
  extern const char *threadName_readMem;
  extern const char *threadName_go;
  extern const char *threadName_writeMem;
  extern const char *threadName_erase;
  extern const char *threadName_extendedErase;
  extern const char *threadName_writeProtect;
  extern const char *threadName_writeUnprotect;
  extern const char *threadName_readoutProtect;
  extern const char *threadName_readoutUnprotect;

  /*------------------------------------------------
  Task Messages
  ------------------------------------------------*/
  namespace TaskMessage
  {
    static constexpr uint32_t WAKEUP         = 0x719c69a9;
    static constexpr uint32_t YIELD_RESOURCE = 0x0b2ec090;
  }

}  // namespace Arche

#endif /* !ARCHE_THREADS_HPP */
