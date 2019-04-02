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
    A unique name for the thread
    ------------------------------------------------*/
    static const char* threadName = "heartbeat";

    /*------------------------------------------------
    System thread priority: Increasing values equals increasing priority
    ------------------------------------------------*/
    static constexpr uint8_t threadPriority = 2;

    /*------------------------------------------------
    Will point to the registered instance of the thread in FreeRTOS
    ------------------------------------------------*/
    extern TaskHandle_t threadHandle;
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
    A unique name for the thread
    ------------------------------------------------*/
    static const char* threadName = "iop_serial";

    /*------------------------------------------------
    System thread priority: Increasing values equals increasing priority
    ------------------------------------------------*/
    static constexpr uint8_t threadPriority = 5;

    /*------------------------------------------------
    Will point to the registered instance of the thread in FreeRTOS
    ------------------------------------------------*/
    extern TaskHandle_t threadHandle;
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
    A unique name for the thread
    ------------------------------------------------*/
    static const char* threadName = "sys_main";

    /*------------------------------------------------
    System thread priority: Increasing values equals increasing priority
    ------------------------------------------------*/
    static constexpr uint8_t threadPriority = 4;

    /*------------------------------------------------
    Will point to the registered instance of the thread in FreeRTOS
    ------------------------------------------------*/
    extern TaskHandle_t threadHandle;
  }
}

#endif /* !ARCHE_THREADS_HPP */
