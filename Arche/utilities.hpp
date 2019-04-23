/********************************************************************************
 *   File Name:
 *    utilities.hpp
 *
 *   Description:
 *    Declares utility functions for Arche
 *
 *   2019 | Brandon Braun | brandonbraun653@gmail.com
 ********************************************************************************/

/* C++ Includes */
#include <cstdint>

namespace Arche
{
  /**
   *	Computes an XOR checksum over a range of data
   *
   *	@param[in]	data        The data to compute the checksum on
   *	@param[in]	start       Start offset
   *	@param[in]	end         End offset
   *	@return uint8_t
   */
  uint8_t XORCheckSum( const uint8_t *const data, const uint32_t start, const uint32_t end );

  /**
   *	Checks with the backend driver if the given address is valid
   * for the particular embedded system.
   *
   *	@param[in]	address     The address to be checked
   *	@return bool
   */
  bool isAddressValid( const uint32_t address );

  /**
   *	Checks if the address is the start of the option byte region
   *
   *	@param[in]	address   The address to be checked
   *	@return bool
   */
  bool isAddressOptionBytes( const uint32_t address );

  /**
   *	Checks if read protection is active
   *
   *	@return bool
   */
  bool isRDPActive();
}