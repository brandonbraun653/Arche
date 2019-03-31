/********************************************************************************
 *  File Name:
 *      threads.hpp
 *
 *  Description:
 *      Lists all the threads that are run on the Arche system
 *
 *  2019 | Brandon Braun | brandonbraun653@gmail.com
 ********************************************************************************/

/* Chimera Includes */
#include <Chimera/threading.hpp>
#include <Chimera/serial.hpp>
#include <Chimera/gpio.hpp>

#include <Arche/threads.hpp>

static const uint32_t ledChangeDelay_mS = 250;

void led_heartbeat( void *argument )
{
  Chimera::GPIO::GPIOClass led;
  Chimera::Threading::signalThreadSetupComplete();

  led.init( Chimera::GPIO::Port::PORTC, 8 );
  led.setMode( Chimera::GPIO::Drive::OUTPUT_PUSH_PULL, false );
  led.setState( Chimera::GPIO::State::LOW );

  for ( ;; )
  {
    led.setState( Chimera::GPIO::State::HIGH );
    Chimera::delayMilliseconds( ledChangeDelay_mS );
    led.setState( Chimera::GPIO::State::LOW );
    Chimera::delayMilliseconds( ledChangeDelay_mS );
  }
}
