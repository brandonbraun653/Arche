/********************************************************************************
 *   File Name:
 *      sys_main.cpp
 *
 *   Description:
 *      Primary thread for executing the bootloader protocol
 *
 *   2019 | Brandon Braun | brandonbraun653@gmail.com
 ********************************************************************************/

/* Chimera Includes */
#include <Chimera/gpio.hpp>
#include <Chimera/threading.hpp>

/* Arche Includes */
#include <Arche/config/led.hpp>

namespace Arche
{
  namespace HeartBeat
  {
    void threadHeartbeat(void *argument)
    {
      Chimera::GPIO::GPIOClass led;
      Chimera::Threading::signalThreadSetupComplete();

      led.init(HEARTBEAT_GPIO_PORT, HEARTBEAT_GPIO_PIN);
      led.setMode(Chimera::GPIO::Drive::OUTPUT_PUSH_PULL, false);
      led.setState(Chimera::GPIO::State::LOW);

      for (;;)
      {
        led.setState(Chimera::GPIO::State::HIGH);
        Chimera::delayMilliseconds(HEARTBEAT_TIME_HI_MS);

        led.setState(Chimera::GPIO::State::LOW);
        Chimera::delayMilliseconds(HEARTBEAT_TIME_LO_MS);
      }
    }
  }
}

