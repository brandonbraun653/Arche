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

#include <cstdint>

#include <Chimera/threading.hpp>

namespace Arche
{
  static constexpr uint8_t numThreads = 3;
  extern const Chimera::Threading::Thread_t threads[ numThreads ];

  namespace HeartBeat
  {
    /*------------------------------------------------
    Thread function
    ------------------------------------------------*/
    void threadHeartbeat( void *argument );

    /*------------------------------------------------
    Sets the number of bytes allocated for the thread
    ------------------------------------------------*/
    static constexpr uint32_t STACK_BYTES = 1000;
    static_assert(STACK_BYTES % sizeof(configSTACK_DEPTH_TYPE) == 0, "Stack size must be divisible by the system word size");

    /*------------------------------------------------
    Configures stack bytes size into the number of words FreeRTOS expects
    ------------------------------------------------*/
    static constexpr configSTACK_DEPTH_TYPE RTOS_STACK_DEPTH = STACK_BYTES / sizeof(configSTACK_DEPTH_TYPE);

    /*------------------------------------------------
    System thread priority: Increasing values equals increasing priority
    ------------------------------------------------*/
    static constexpr uint8_t threadPriority = 2;
  }

  namespace IOP
  {
    /*------------------------------------------------
    Thread function
    ------------------------------------------------*/
    void iopSerial(void *argument);

    /*------------------------------------------------
    Sets the number of bytes allocated for the thread
    ------------------------------------------------*/
    static constexpr uint32_t STACK_BYTES = 1000;
    static_assert(STACK_BYTES % sizeof(configSTACK_DEPTH_TYPE) == 0, "Stack size must be divisible by the system word size");

    /*------------------------------------------------
    Configures stack bytes size into the number of words FreeRTOS expects
    ------------------------------------------------*/
    static constexpr configSTACK_DEPTH_TYPE RTOS_STACK_DEPTH = STACK_BYTES / sizeof(configSTACK_DEPTH_TYPE);

    /*------------------------------------------------
    System thread priority: Increasing values equals increasing priority
    ------------------------------------------------*/
    static constexpr uint8_t threadPriority = 5;
  }

  namespace SYS
  {
    /*------------------------------------------------
    Primary thread for implementing the bootloader protocol
    ------------------------------------------------*/
    void sysMain(void *argument);

    /*------------------------------------------------
    Sets the number of bytes allocated for the thread
    ------------------------------------------------*/
    static constexpr uint32_t STACK_BYTES = 1000;
    static_assert(STACK_BYTES % sizeof(configSTACK_DEPTH_TYPE) == 0, "Stack size must be divisible by the system word size");

    /*------------------------------------------------
    Configures stack bytes size into the number of words FreeRTOS expects
    ------------------------------------------------*/
    static constexpr configSTACK_DEPTH_TYPE RTOS_STACK_DEPTH = STACK_BYTES / sizeof(configSTACK_DEPTH_TYPE);

    /*------------------------------------------------
    System thread priority: Increasing values equals increasing priority
    ------------------------------------------------*/
    static constexpr uint8_t threadPriority = 4;
  }
}

#endif /* !ARCHE_THREADS_HPP */
