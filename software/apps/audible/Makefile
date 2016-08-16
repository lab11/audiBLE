PROJECT_NAME = $(shell basename "$(realpath ./)")

APPLICATION_SRCS = $(notdir $(wildcard ./*.c))
APPLICATION_SRCS += softdevice_handler.c
APPLICATION_SRCS += ble_advdata.c
APPLICATION_SRCS += ble_conn_params.c

APPLICATION_SRCS += nrf_drv_common.c
APPLICATION_SRCS += nrf_drv_gpiote.c
APPLICATION_SRCS += app_gpiote.c
APPLICATION_SRCS += app_timer.c
APPLICATION_SRCS += app_error.c
APPLICATION_SRCS += nrf_drv_adc.c
APPLICATION_SRCS += app_util_platform.c

APPLICATION_SRCS += led.c

APPLICATION_SRCS += audible.c
APPLICATION_SRCS += simple_ble.c
APPLICATION_SRCS += eddystone.c
APPLICATION_SRCS += simple_adv.c
APPLICATION_SRCS += multi_adv.c

LIBRARY_PATHS += . ../../platform
SOURCE_PATHS += ../../platform

SDK_VERSION = 11
SOFTDEVICE_MODEL = s130
RAM_KB = 32
FLASH_KB = 256

ENABLE_WIRELESS_DFU = 1

NRF_BASE_PATH ?= ../../nrf5x-base
include $(NRF_BASE_PATH)/make/Makefile
