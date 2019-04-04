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
#include <Chimera/chimera.hpp>
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
      Chimera::Threading::signalSetupComplete();

      led.init(HEARTBEAT_GPIO_PORT, HEARTBEAT_GPIO_PIN);
      led.setMode(Chimera::GPIO::Drive::OUTPUT_PUSH_PULL, false);
      led.setState(Chimera::GPIO::State::LOW);

      /*------------------------------------------------
      Simple little powerup flash "animation"
      ------------------------------------------------*/
      for ( uint8_t x = 4; x > 0; x-- )
      {
        led.setState( Chimera::GPIO::State::HIGH );
        Chimera::delayMilliseconds( 25 );

        led.setState( Chimera::GPIO::State::LOW );
        Chimera::delayMilliseconds( 75 );
      }
      Chimera::delayMilliseconds( 500 );

      /*------------------------------------------------
      Main loop, never returns
      ------------------------------------------------*/
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

