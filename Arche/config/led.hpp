/********************************************************************************
 * File Name:
 *	  led.hpp
 *
 * Description:
 *	  Provides configuration options for system LEDs
 *
 * 2019 | Brandon Braun | brandonbraun653@gmail.com
 ********************************************************************************/

#pragma once
#ifndef ARCHE_CONFIG_LED_HPP
#define ARCHE_CONFIG_LED_HPP

/* C++ Includes */
#include <cstdint>

/* User Includes */
#include "arche_user_config.hpp"

/* Chimera Includes */
#include <Chimera/interface.hpp>

namespace Arche
{
  namespace Config
  {
    namespace LED
    {
      /*------------------------------------------------
      Amount of time the LED spends with a logical HIGH signal applied to the GPIO
      ------------------------------------------------*/
      static constexpr uint8_t HEARTBEAT_TIME_HI_MS = 250;

      /*------------------------------------------------
      Amount of time the LED spends with a logical LOW signal applied to the GPIO
      ------------------------------------------------*/
      static constexpr uint8_t HEARTBEAT_TIME_LO_MS = 250;

      /*------------------------------------------------
      User GPIO configuration options for the heartbeat
      ------------------------------------------------*/
#if defined( ARCHE_HEARTBEAT_ENABLED )
      static constexpr uint8_t HEARTBEAT_GPIO_PIN              = USER_HEARTBEAT_GPIO_PIN;
      static constexpr Chimera::GPIO::Port HEARTBEAT_GPIO_PORT = USER_HEARTBEAT_GPIO_PORT;
#endif /* ARCHE_HEARTBEAT_ENABLED */
    }  // namespace LED
  }    // namespace Config
}  // namespace Arche

#endif /* !ARCHE_CONFIG_LED_HPP */
