// Copyright (c) 2022-2023 Cesanta Software Limited
// All rights reserved
//
// Datasheet: RM0433, devboard manual: UM2407
// https://www.st.com/resource/en/reference_manual/rm0433-stm32h742-stm32h743753-and-stm32h750-value-line-advanced-armbased-32bit-mcus-stmicroelectronics.pdf
// Alternate functions: https://www.st.com/resource/en/datasheet/stm32h743vi.pdf

#pragma once

#include <stm32h755xx.h>

static inline char chiprev(void) {
  uint16_t rev = (uint16_t) (((uint32_t) DBGMCU->IDCODE) >> 16);
  if (rev == 0x1003) return 'Y';
  if (rev == 0x2003) return 'V';
  return '?';
}

#define UUID ((uint8_t *) UID_BASE)  // Unique 96-bit chip ID. TRM 61.1

// Helper macro for MAC generation
#define GENERATE_LOCALLY_ADMINISTERED_MAC()                        \
  {                                                                \
    2, UUID[0] ^ UUID[1], UUID[2] ^ UUID[3], UUID[4] ^ UUID[5],    \
        UUID[6] ^ UUID[7] ^ UUID[8], UUID[9] ^ UUID[10] ^ UUID[11] \
  }

// For internal testing purposes
#ifdef UART_DEBUG
#define BIT(x) (1UL << (x))
static inline void test_init(void) {
  USART_TypeDef *uart = USART1;  // hardcode to USART1 PA9,10
  __HAL_RCC_USART1_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  RCC->D2CCIP2R = (RCC->D2CCIP2R & ~(7 << 3)) | (3 << 3);  // use HSI for UART1
  GPIO_InitTypeDef GPIO_InitStruct = {.Pin = GPIO_PIN_10 | GPIO_PIN_9,
                                      .Mode = GPIO_MODE_AF_PP,
                                      .Pull = GPIO_NOPULL,
                                      .Speed = GPIO_SPEED_FREQ_VERY_HIGH,
                                      .Alternate = GPIO_AF7_USART1};
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
  uart->CR1 = 0;                          // Disable this UART
  uart->BRR = 64000000 / 115200;          // Set baud rate
  uart->CR1 |= BIT(0) | BIT(2) | BIT(3);  // Set UE, RE, TE
}
static inline void uart_write_byte(USART_TypeDef *uart, uint8_t byte) {
  uart->TDR = byte;
  while ((uart->ISR & BIT(7)) == 0) (void) 0;
}
static inline void uart_write_buf(USART_TypeDef *uart, char *buf, size_t len) {
  while (len-- > 0) uart_write_byte(uart, *(uint8_t *) buf++);
}
#else
#define test_init()
#endif
