#ifndef WS2812B_H
#define WS2812B_H

#include <stdint.h>
#include "stm32f4xx_hal.h"

#define BIT_0_TIME 32
#define BIT_1_TIME 64
#define RESET_LEN 60
#define LED_N 8

void ws2812b_init(void);

static void set_byte(uint32_t pos, uint8_t value);

void ws2812b_set_color(uint32_t led_n, uint8_t red, uint8_t green, uint8_t blue);

void ws2812b_set_color_gamma_corrected(uint32_t led_n, uint8_t red, uint8_t green, uint8_t blue);

void ws2812b_update(void);

#endif
