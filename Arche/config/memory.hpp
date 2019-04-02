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

/* User Generated Includes */
#include "arche_user_config.hpp"


/*------------------------------------------------
Validates that the user has configured the bootloader region size correctly
------------------------------------------------*/
static_assert(BOOTLOADER_RGN_SIZE == BOOTLOADER_EXPECTED_RGN_SIZE, "Bootloader region size incorrect");

/*------------------------------------------------
Validates that the user has configured the start of the system image region size correctly
------------------------------------------------*/
static_assert(SYSTEM_IMAGE_START_ADDR > BOOTLOADER_END_ADDR, "System image start address lies in bootloader allocation region");


#endif /* !ARCHE_CONFIG_MEMORY_HPP */
