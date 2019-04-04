/********************************************************************************
 * File Name:
 *	  watchdog.hpp
 *
 * Description:
 *	  Watchdog configuration options for Arche
 *
 * 2019 | Brandon Braun | brandonbraun653@gmail.com
 ********************************************************************************/

#pragma once
#ifndef ARCHE_CONFIG_WATCHDOG_HPP
#define ARCHE_CONFIG_WATCHDOG_HPP

/* C++ Includes */
#include <cstdint>

/* Arche Includes */
#include "arche_user_config.hpp"

namespace Arche
{
  namespace Config
  {
    namespace Watchdog
    {
      static constexpr uint32_t TIMEOUT_MS = USER_WATCHDOG_TIMEOUT_MS;

    }  // namespace Serial
  }    // namespace Config
}  // namespace Arche


#endif /* !ARCHE_CONFIG_SERIAL_HPP */
