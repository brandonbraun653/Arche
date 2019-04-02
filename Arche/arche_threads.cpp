/********************************************************************************
 *  File Name:
 *      threads.hpp
 *
 *  Description:
 *      Lists all the threads that are run on the Arche system
 *
 *  2019 | Brandon Braun | brandonbraun653@gmail.com
 ********************************************************************************/

 #include <Arche/arche_threads.hpp>

namespace Arche
{
  namespace HeartBeat
  {
    TaskHandle_t threadHandle;
  }

  namespace IOP
  {
    TaskHandle_t threadHandle;
  }

  namespace SYS
  {
    TaskHandle_t threadHandle;
  }
}
