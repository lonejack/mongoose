#pragma once

// See https://mongoose.ws/documentation/#build-options
#define MG_ARCH MG_ARCH_FREERTOS
#define MG_DEVICE MG_DEVICE_STM32H7

#define MG_ENABLE_TCPIP 1
#define MG_ENABLE_DRIVER_STM32H 1
#define MG_IO_SIZE 256
#define MG_ENABLE_CUSTOM_RANDOM 1
#define MG_ENABLE_PACKED_FS 1
#define MG_ENABLE_FILE 0
