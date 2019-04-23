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

/* C++ Includes */
#include <array>

/* Chimera Includes */
#include <Chimera/memory.hpp>
#include <Chimera/serial.hpp>
#include <Chimera/threading.hpp>
#include <Chimera/watchdog.hpp>

extern Chimera::Memory::InternalFlash flash;
extern Chimera::Serial::SerialClass serial;
extern Chimera::Watchdog::WatchdogClass watchdog;

namespace Arche
{
  static constexpr uint8_t BOOTLOADER_VERSION = 0x10; /* 1.0 */

  static std::array<uint8_t, 1024> systemBuffer;
}
#endif /* !ARCHE_HPP */
