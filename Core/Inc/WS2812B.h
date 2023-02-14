#ifndef WS2812B_H_
#define WS2812B_H_
#include "stm32wb55xx.h"                  // Device header
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "main.h"
#include <string.h>

#define BitIsSet(reg, bit) ((reg & (1<<bit)) != 0)
#define DELAY_LENGTH  48
#define NUM_OF_LEDS  150
#define BUFF_LENGTH (DELAY_LENGTH+(NUM_OF_LEDS*24))
#define ONE  52
#define ZERO 26
void WS2812B_clear(void);
void WS2812B_pixel_rgb_to_BUFF_DMA(uint8_t Rpixel , uint8_t Gpixel, uint8_t Bpixel, uint32_t posX);
void WS2812B_light(void);
void WS2812B_setValue(void);
void WS2812B_prepareValue (uint8_t r00, uint8_t g00, uint8_t b00,
                           uint8_t r01, uint8_t g01, uint8_t b01,
													 uint8_t r02, uint8_t g02, uint8_t b02,
													 uint8_t r03, uint8_t g03, uint8_t b03,
													 uint8_t r04, uint8_t g04, uint8_t b04);
void ledsEffect_1(void);
void ledsEffect_2(void);
void ledsEffect_3(void);
void run_static_leds(void);
void set_colour_in_tape(uint8_t red,uint8_t green,uint8_t blue,uint8_t led_num);
#endif
