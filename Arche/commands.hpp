/********************************************************************************
 *  File Name:
 *      commands.hpp
 *
 *  Description:
 *      Describes the various commands available with the Arche Bootloader
 *
 *  2019 | Brandon Braun | brandonbraun653@gmail.com
 ********************************************************************************/

#pragma once
#ifndef ARCHE_COMMANDS_HPP
#define ARCHE_COMMANDS_HPP

/* C++ Includes */
#include <cstdint>
#include <array>

/* Boost Includes */
#include <boost/container/flat_map.hpp>

/* Chimera Includes */
#include <Chimera/threading.hpp>

/* Arche Includes */
#include <Arche/threads.hpp>

namespace Arche
{
  namespace Command
  {
    using Command_t = uint16_t;

#if defined( CHIMERA_BIG_ENDIAN)
    static constexpr Command_t GET               = 0x00FF;
    static constexpr Command_t GET_VER           = 0x01FE;
    static constexpr Command_t GET_ID            = 0x02FD;
    static constexpr Command_t READ_MEM          = 0x11EE;
    static constexpr Command_t GO                = 0x21DE;
    static constexpr Command_t WRITE_MEM         = 0x31CE;
    static constexpr Command_t ERASE             = 0x43BC;
    static constexpr Command_t EXTENDED_ERASE    = 0x44BB;
    static constexpr Command_t WRITE_PROTECT     = 0x639C;
    static constexpr Command_t WRITE_UNPROTECT   = 0x738C;
    static constexpr Command_t READOUT_PROTECT   = 0x8272;
    static constexpr Command_t READOUT_UNPROTECT = 0x926D;
#elif defined( CHIMERA_LITTLE_ENDIAN)
    static constexpr Command_t GET               = 0xFF00;
    static constexpr Command_t GET_VER           = 0xFE01;
    static constexpr Command_t GET_ID            = 0xFD02;
    static constexpr Command_t READ_MEM          = 0xEE11;
    static constexpr Command_t GO                = 0xDE21;
    static constexpr Command_t WRITE_MEM         = 0xCE31;
    static constexpr Command_t ERASE             = 0xBC43;
    static constexpr Command_t EXTENDED_ERASE    = 0xBB44;
    static constexpr Command_t WRITE_PROTECT     = 0x9C63;
    static constexpr Command_t WRITE_UNPROTECT   = 0x8C73;
    static constexpr Command_t READOUT_PROTECT   = 0x7282;
    static constexpr Command_t READOUT_UNPROTECT = 0x6D92;
#endif

    extern const boost::container::flat_map<Command_t, uint8_t> threadIndex;

  }  // namespace Command
}  // namespace Arche

#endif /* !ARCHE_COMMANDS_HPP */