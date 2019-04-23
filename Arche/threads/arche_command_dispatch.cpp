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
#include <Arche/commands.hpp>
#include <Arche/threads.hpp>
#include <Arche/config/serial.hpp>
#include <Arche/config/memory.hpp>

/* Chimera Includes */
#include <Chimera/chimera.hpp>
#include <Chimera/exception.hpp>
#include <Chimera/serial.hpp>
#include <Chimera/threading.hpp>

/* Boost Includes */
#include <boost/circular_buffer.hpp>

static const uint32_t baudrate                 = 115200;
static const Chimera::Serial::CharWid width    = Chimera::Serial::CharWid::CW_8BIT;
static const Chimera::Serial::Parity parity    = Chimera::Serial::Parity::PAR_NONE;
static const Chimera::Serial::StopBits stop    = Chimera::Serial::StopBits::SBITS_ONE;
static const Chimera::Serial::FlowControl flow = Chimera::Serial::FlowControl::FCTRL_NONE;

Chimera::Serial::SerialClass serial( Arche::Config::Serial::BUFFER_SIZE );

static boost::circular_buffer<uint8_t> rxBuffer( Arche::Config::Serial::BUFFER_SIZE );
static boost::circular_buffer<uint8_t> txBuffer( Arche::Config::Serial::BUFFER_SIZE );

namespace Arche
{
  void commandDispatch( void *argument )
  {
    Chimera::Serial::IOPins pins;
    SemaphoreHandle_t rxWakeupSignal = xSemaphoreCreateBinary();

    Chimera::Threading::signalSetupComplete();

    /*------------------------------------------------
    Assign the GPIO pin configuration
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
    Configure the hardware driver
    ------------------------------------------------*/
    if ( serial.reserve( 100 ) == Chimera::CommonStatusCodes::OK )
    {
      /*------------------------------------------------
      Must be assigned first. The backend driver could chooose to
      instantiate the serial object from either UART or USART. Calls
      to other functions before this is set up is potentially undefined behavior.

      Reserve can be called because it is inherited behavior.
      ------------------------------------------------*/
      serial.assignHW( Config::Serial::CHANNEL, pins );

      /*------------------------------------------------
      Turn on bufferring for both the RX and TX paths
      ------------------------------------------------*/
      serial.enableBuffering( Chimera::Serial::SubPeripheral::TX, &txBuffer );
      serial.enableBuffering( Chimera::Serial::SubPeripheral::RX, &rxBuffer );

      /*------------------------------------------------
      Configure the low level driver
      ------------------------------------------------*/
      serial.configure( baudrate, width, parity, stop, flow );
      serial.begin( Chimera::Serial::Modes::DMA, Chimera::Serial::Modes::DMA );
      serial.release();
    }
    else
    {
      /*------------------------------------------------
      We have some issues and cannot continue. Wait for 
      the watchdog to reset the board.
      ------------------------------------------------*/
      vTaskSuspendAll();
      while ( 1 )
      {
      }
    }

    uint32_t taskMessage = 0u;
    for ( ;; )
    {
      /*------------------------------------------------
      Wait for another thread to wake us up to do work
      ------------------------------------------------*/
      taskMessage = ulTaskNotifyTake( pdTRUE, portMAX_DELAY );
      if ( taskMessage == TaskMessage::WAKEUP )
      {
        /*------------------------------------------------
        Make sure we immediately get exclusive access to the serial driver
        ------------------------------------------------*/
        if ( serial.reserve( 0 ) == Chimera::CommonStatusCodes::OK )
        {
          Command::Command_t command = 0u;
          TaskHandle_t dispatchThread = nullptr;

          /*------------------------------------------------
          Start the listening operation with a clean slate
          ------------------------------------------------*/
          serial.flush( Chimera::Serial::SubPeripheral::RX );
          serial.attachEventNotifier( Chimera::Serial::Event::READ_COMPLETE, &rxWakeupSignal );

          /*------------------------------------------------
          Wait for data to arrive, then parse it into the correct command
          ------------------------------------------------*/
          do
          {
            /*------------------------------------------------
            Ensure we received the expected number of bytes
            ------------------------------------------------*/
            if ( xSemaphoreTake( rxWakeupSignal, portMAX_DELAY ) == pdPASS 
              && serial.available() 
              && serial.readAsync(reinterpret_cast<uint8_t*>(&command), sizeof(Command::Command_t)) == Chimera::CommonStatusCodes::OK )
            {
              /*------------------------------------------------
              If the lookup fails, boost will throw an exception.
              ------------------------------------------------*/
              uint8_t index = Command::threadIndex.at( command );
              assert( index < threads.size() );

              if ( Chimera::Exception::out_of_range_thrown() && command )
              {
                Chimera::Exception::out_of_range_clear();
              }
              else
              {
                dispatchThread = threads[ index ].handle;
              }
            }
            
            if( !dispatchThread )
            {
              Command::sendNACKByte();
            }
          } while ( !dispatchThread );

          /*------------------------------------------------
          Switch over to the thread which will actually handle the command 
          ------------------------------------------------*/
          serial.release();
          serial.removeEventNotifier( Chimera::Serial::Event::READ_COMPLETE, &rxWakeupSignal );
          xTaskNotify( dispatchThread, TaskMessage::WAKEUP, eSetValueWithOverwrite );
        }
        else
        {
          // TODO: log an assert here
        }
      }

    }
  }
}  // namespace Arche
