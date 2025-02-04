#include "ws2812b.h"
#include "stm32f4xx_hal.h"

const uint16_t gamma8[] = {
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  1,  1,  1,
    1,  1,  1,  1,  1,  1,  1,  1,  1,  2,  2,  2,  2,  2,  2,  2,
    2,  3,  3,  3,  3,  3,  3,  3,  4,  4,  4,  4,  4,  5,  5,  5,
    5,  6,  6,  6,  6,  7,  7,  7,  7,  8,  8,  8,  9,  9,  9, 10,
   10, 10, 11, 11, 11, 12, 12, 13, 13, 13, 14, 14, 15, 15, 16, 16,
   17, 17, 18, 18, 19, 19, 20, 20, 21, 21, 22, 22, 23, 24, 24, 25,
   25, 26, 27, 27, 28, 29, 29, 30, 31, 32, 32, 33, 34, 35, 35, 36,
   37, 38, 39, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 50,
   51, 52, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 66, 67, 68,
   69, 70, 72, 73, 74, 75, 77, 78, 79, 81, 82, 83, 85, 86, 87, 89,
   90, 92, 93, 95, 96, 98, 99,101,102,104,105,107,109,110,112,114,
  115,117,119,120,122,124,126,127,129,131,133,135,137,138,140,142,
  144,146,148,150,152,154,156,158,160,162,164,167,169,171,173,175,
  177,180,182,184,186,189,191,193,196,198,200,203,205,208,210,213,
  215,218,220,223,225,228,231,233,236,239,241,244,247,249,252,255 };

static uint16_t led_buffer[RESET_LEN + 24 * LED_N + 1];

void DMA_Config(uint32_t src_addr, uint32_t dst_addr, uint16_t size) {
	DMA1_Stream4->NDTR = size;
	DMA1_Stream4->M0AR = src_addr;
	DMA1_Stream4->PAR = dst_addr;
}

static void set_byte(uint32_t pos, uint8_t value) {
	for (int i = 0; i < 8; i++) {
		if (value & 0x80) {
			led_buffer[pos + i] = BIT_1_TIME;
		} else {
			led_buffer[pos + i] = BIT_0_TIME;
		}

		value <<= 1;
	}
}

void ws2812b_set_color_gamma_corrected(uint32_t led_n, uint8_t red, uint8_t green, uint8_t blue) {
	ws2812b_set_color(led_n, gamma8[red], gamma8[green], gamma8[blue]);
}

void ws2812b_set_color(uint32_t led_n, uint8_t red, uint8_t green, uint8_t blue) {
	if (led_n < LED_N) {
		set_byte(RESET_LEN + 24 * led_n, green);
		set_byte(RESET_LEN + 24 * led_n + 8, red);
		set_byte(RESET_LEN + 24 * led_n + 16, blue);
	}
}

void ws2812b_update(void) {
	DMA1_Stream4->CR |= DMA_SxCR_EN;
}

void ws2812b_init(void) {
	DMA_Config((uint32_t)led_buffer, (uint32_t)&(TIM3->DMAR), RESET_LEN + 24 * LED_N + 1);

	for (int i = 0; i < RESET_LEN; i++) {
		led_buffer[i] = 0;
	}

	for (int i = 0; i < 24 * LED_N; i++) {
		led_buffer[RESET_LEN + i] = BIT_0_TIME;
	}

	led_buffer[RESET_LEN + 24 * LED_N] = 0;

	ws2812b_update();
}

