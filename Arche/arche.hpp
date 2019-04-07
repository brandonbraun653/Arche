/********************************************************************************
*   File Name:
*     arche.hpp
*
*   Description:
*     Common header for implementing Arche functionality in a project
*
*   2019 | Brandon Braun | brandonbraun653@gmail.com
********************************************************************************/
#pragma once
#ifndef ARCHE_HPP
#define ARCHE_HPP

/* Chimera Includes */
#include <Chimera/serial.hpp>
#include <Chimera/threading.hpp>
#include <Chimera/watchdog.hpp>

extern Chimera::Serial::SerialClass serial;
extern Chimera::Watchdog::WatchdogClass watchdog;

namespace Arche
{
  static constexpr uint8_t BOOTLOADER_VERSION = 0x10; /* 1.0 */
}
#endif /* !ARCHE_HPP */
