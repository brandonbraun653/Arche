/********************************************************************************
 * File Name:
 *	  serial.hpp
 *
 * Description:
 *	  Serial configuration options for Arche
 *
 * 2019 | Brandon Braun | brandonbraun653@gmail.com
 ********************************************************************************/

#pragma once
#ifndef ARCHE_CONFIG_SERIAL_HPP
#define ARCHE_CONFIG_SERIAL_HPP

/* C++ Includes */
#include <cstdint>

/* Arche Includes */
#include "arche_user_config.hpp"

namespace Arche
{
  namespace Config
  {
    namespace Serial
    {
      /*------------------------------------------------
      TX GPIO Config
      ------------------------------------------------*/
      static constexpr Chimera::GPIO::Port TX_GPIO_PORT   = USER_SERIAL_TX_GPIO_PORT;
      static constexpr Chimera::GPIO::Drive TX_GPIO_DRIVE = USER_SERIAL_TX_GPIO_DRIVE;
      static constexpr uint32_t TX_GPIO_ALTERNATE         = USER_SERIAL_TX_GPIO_ALTERNATE;
      static constexpr uint8_t TX_GPIO_PIN                = USER_SERIAL_TX_GPIO_PIN;

      /*------------------------------------------------
      RX GPIO Config
      ------------------------------------------------*/
      static constexpr Chimera::GPIO::Port RX_GPIO_PORT   = USER_SERIAL_RX_GPIO_PORT;
      static constexpr Chimera::GPIO::Drive RX_GPIO_DRIVE = USER_SERIAL_RX_GPIO_DRIVE;
      static constexpr uint32_t RX_GPIO_ALTERNATE         = USER_SERIAL_RX_GPIO_ALTERNATE;
      static constexpr uint8_t RX_GPIO_PIN                = USER_SERIAL_RX_GPIO_PIN;

      /*------------------------------------------------
      Peripheral Config
      ------------------------------------------------*/
      static constexpr uint8_t CHANNEL = USER_SERIAL_CHANNEL;
      static constexpr uint32_t BAUD   = static_cast<uint32_t>( Chimera::Serial::BaudRate::SERIAL_BAUD_115200 );
      static constexpr float MS_PER_BYTE = 1000.0f / static_cast<float>( BAUD / 8 );
      static constexpr uint32_t BUFFER_SIZE = 512;

      /*------------------------------------------------
      Protocol Config
      ------------------------------------------------*/
      static constexpr uint8_t ACK_BYTE  = 0x79;
      static constexpr uint8_t NACK_BYTE = 0x1F;
    }
  }
}


#endif /* !ARCHE_CONFIG_SERIAL_HPP */
