#ifndef AUDIBLE_H
#define AUDIBLE_H

#ifndef DEVICE_NAME
#define DEVICE_NAME "audiBLE"
#endif /*DEVICE_NAME*/

#include <stdint.h>

#define PLATFORM_ID_BYTE 0x30

// Address is written here in flash if the ID command is used
#define ADDRESS_FLASH_LOCATION 0x0003fff8

// FTDI chip for bootloader
#define BOOTLOADER_CTRL_PIN  3
#define BOOTLOADER_CTRL_PULL NRF_GPIO_PIN_PULLUP
#define BOOTLOADER_RX_PIN 28
#define BOOTLOADER_TX_PIN 29

// Blue led on squall
#define SQUALL_LED_PIN 13

// LDO Enable pin
#define AUDIBLE_LDO_EN_PIN 22

// LTC1966 RMS-DC Enable pin
#define LTC1966_EN_PIN 23

// Integrator Drain
#define INT_DRAIN_PIN 24

#endif /*AUDIBLE_H*/

