/********************************************************************************
 *   File Name:
 *    arche_utilities.cpp
 *
 *   Description:
 *    Implements utility functions for Arche
 *
 *   2019 | Brandon Braun | brandonbraun653@gmail.com
 ********************************************************************************/


/* Arche Includes */
#include <Arche/arche.hpp>
#include <Arche/utilities.hpp>



namespace Arche
{
  uint8_t XORCheckSum( const uint8_t *const data, const uint32_t start, const uint32_t end )
  {
    uint8_t result = 0x00;
    
    for ( uint8_t x = start; x <= end; x++ )
    {
      result ^= data[ x ];
    }

    return result;
  }

  bool isAddressValid( const uint32_t address )
  {
    // Guess you need to implement this huh?
    return false;
  }

  bool isAddressOptionBytes( const uint32_t address )
  {
    // Guess you need to implement this huh?
    return false;
  }
}