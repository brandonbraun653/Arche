/********************************************************************************
 *  File Name:
 *    memory.hpp
 *
 *  Description:
 *    Defines configuration options for memory regions in Arche
 *
 *  2019 | Brandon Braun | brandonbraun653@gmail.com
 ********************************************************************************/

#pragma once
#ifndef ARCHE_CONFIG_MEMORY_HPP
#define ARCHE_CONFIG_MEMORY_HPP

/* C++ Includes */
#include <cstdlib>

static constexpr uint32_t BOOTLOADER_EXPECTED_RGN_SIZE = 64 * 1024;

static constexpr uint32_t BOOTLOADER_START_ADDR = 0x08000000;
static constexpr uint32_t BOOTLOADER_END_ADDR   = 0x0800FFFF;
static constexpr uint32_t BOOTLOADER_RGN_SIZE   = BOOTLOADER_END_ADDR - BOOTLOADER_START_ADDR + 1;
static_assert(BOOTLOADER_RGN_SIZE == BOOTLOADER_EXPECTED_RGN_SIZE, "Bootloader region size incorrect");


static constexpr uint32_t SYSTEM_IMAGE_START_ADDR = 0x08010000;
static constexpr uint32_t SYSTEM_IMAGE_END_ADDR = 0x0807FFFF;
static constexpr uint32_t SYSTEM_IMAGE_SIZE = SYSTEM_IMAGE_END_ADDR - SYSTEM_IMAGE_START_ADDR + 1;

static_assert(SYSTEM_IMAGE_START_ADDR > BOOTLOADER_END_ADDR, "System image start address lies in bootloader allocation region");


#endif /* !ARCHE_CONFIG_MEMORY_HPP */