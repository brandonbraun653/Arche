/********************************************************************************
*   File Name:
*      iop_serial.cpp
*
*   Description:
*      Primary thread for executing the bootloader protocol
*
*   2019 | Brandon Braun | brandonbraun653@gmail.com
********************************************************************************/

/* Arche Includes */
#include <Arche/arche.hpp>
#include <Arche/arche_threads.hpp>
#include <Arche/config/serial.hpp>
#include <Arche/config/memory.hpp>

/* Chimera Includes */
#include <Chimera/chimera.hpp>
#include <Chimera/serial.hpp>
#include <Chimera/threading.hpp>

/* Boost Includes */
#include <boost/circular_buffer.hpp>

static const uint32_t baudrate                 = 115200;
static const Chimera::Serial::CharWid width    = Chimera::Serial::CharWid::CW_8BIT;
static const Chimera::Serial::Parity parity    = Chimera::Serial::Parity::PAR_NONE;
static const Chimera::Serial::StopBits stop    = Chimera::Serial::StopBits::SBITS_ONE;
static const Chimera::Serial::FlowControl flow = Chimera::Serial::FlowControl::FCTRL_NONE;

static constexpr size_t bufferSize = 100;

namespace Arche
{
  namespace IOP
  {
    void iopSerial(void *argument)
    {
      Chimera::Serial::IOPins pins;
      Chimera::Serial::SerialClass serial( bufferSize );
      SemaphoreHandle_t rxWakeupSignal = xSemaphoreCreateBinary();

      std::string helloWorld = "hello world\r\n";

      Chimera::Threading::signalSetupComplete();

      /*------------------------------------------------
      Initialize the GPIO pins
      ------------------------------------------------*/
      pins.tx.pin       = Config::Serial::TX_GPIO_PIN;
      pins.tx.port      = Config::Serial::TX_GPIO_PORT;
      pins.tx.pull      = Chimera::GPIO::Pull::PULL_UP;
      pins.tx.drive     = Config::Serial::TX_GPIO_DRIVE;
      pins.tx.alternate = Config::Serial::TX_GPIO_ALTERNATE;

      pins.rx.pin       = Config::Serial::RX_GPIO_PIN;
      pins.rx.port      = Config::Serial::RX_GPIO_PORT;
      pins.rx.pull      = Chimera::GPIO::Pull::PULL_UP;
      pins.rx.drive     = Config::Serial::RX_GPIO_DRIVE;
      pins.rx.alternate = Config::Serial::RX_GPIO_ALTERNATE;

      /*------------------------------------------------
      Configure the low level driver
      ------------------------------------------------*/
      serial.assignHW( Config::Serial::CHANNEL, pins );
      serial.configure( baudrate, width, parity, stop, flow );

      /*------------------------------------------------
      Turn on bufferring for both the RX and TX paths
      ------------------------------------------------*/
      boost::circular_buffer<uint8_t> rxBuffer( bufferSize );
      boost::circular_buffer<uint8_t> txBuffer( bufferSize );
      serial.enableBuffering( Chimera::Serial::SubPeripheral::TX, &txBuffer );
      serial.enableBuffering( Chimera::Serial::SubPeripheral::RX, &rxBuffer );

      /*------------------------------------------------
      Finally, take control of the hardware and start listening.
      ------------------------------------------------*/
      if( serial.reserve( 100 ) == Chimera::CommonStatusCodes::OK )
      {
        serial.begin( Chimera::Serial::Modes::DMA, Chimera::Serial::Modes::DMA );
        serial.attachEventNotifier( Chimera::Serial::Event::READ_COMPLETE, &rxWakeupSignal );
        watchdog.kick();
      }
      else
      {
        /* We have some issues. Wait for the watchdog to reset the board. */
        while(1){}
      }
      
      /*------------------------------------------------
      Main Loop
      ------------------------------------------------*/
      for (;;)
      {
        /*------------------------------------------------
        Only wake up this thread if we've received some data from the Serial port
        ------------------------------------------------*/
        if( xSemaphoreTake(rxWakeupSignal, portMAX_DELAY) == pdPASS )
        {
          serial.write(reinterpret_cast<const uint8_t*>(helloWorld.c_str()), helloWorld.length());
        }
      }
    }
  }
}

