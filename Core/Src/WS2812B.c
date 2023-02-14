#include "WS2812B.h"
#include "stm32wb55xx.h"
#include "stm32wbxx_hal.h"
#include <stdlib.h>

extern TIM_HandleTypeDef htim1;
uint32_t BUFF_DMA [BUFF_LENGTH] = {0};
uint8_t rgb_temp[5][3] = {0};
uint32_t OneLed_DMA_BUF_TEMP[24]={0};
uint8_t leds_data[4] = {0};


void WS2812B_clear(void)
{
	int i=0;
  for(i=DELAY_LENGTH;i<BUFF_LENGTH;i++) 
			BUFF_DMA[i] = ZERO;
	 WS2812B_light();
}

void WS2812B_pixel_rgb_to_BUFF_DMA(uint8_t Rpixel , uint8_t Gpixel, uint8_t Bpixel, uint32_t posX)
{
  uint32_t pos=0;
  for(pos=0;pos<8;pos++)
  {
    if (BitIsSet(Rpixel,(7-pos)) == 1)
      BUFF_DMA[DELAY_LENGTH+posX*24+pos+8] = ONE;
    else
      BUFF_DMA[DELAY_LENGTH+posX*24+pos+8] = ZERO;
		
    if (BitIsSet(Gpixel,(7-pos)) == 1)
      BUFF_DMA[DELAY_LENGTH+posX*24+pos+0] = ONE;
    else
      BUFF_DMA[DELAY_LENGTH+posX*24+pos+0] = ZERO;
		
    if (BitIsSet(Bpixel,(7-pos)) == 1)
      BUFF_DMA[DELAY_LENGTH+posX*24+pos+16] = ONE;
    else
      BUFF_DMA[DELAY_LENGTH+posX*24+pos+16] = ZERO;
  }
}

void WS2812B_prepareValue (uint8_t r00, uint8_t g00, uint8_t b00,
                           uint8_t r01, uint8_t g01, uint8_t b01,
													 uint8_t r02, uint8_t g02, uint8_t b02,
													 uint8_t r03, uint8_t g03, uint8_t b03,
													 uint8_t r04, uint8_t g04, uint8_t b04)
{
  rgb_temp[0][0]=r00; rgb_temp[0][1]=g00; rgb_temp[0][2]=b00;
  rgb_temp[1][0]=r01; rgb_temp[1][1]=g01; rgb_temp[1][2]=b01;
  rgb_temp[2][0]=r02; rgb_temp[2][1]=g02; rgb_temp[2][2]=b02;
  rgb_temp[3][0]=r03; rgb_temp[3][1]=g03; rgb_temp[3][2]=b03;
  rgb_temp[4][0]=r04; rgb_temp[4][1]=g04; rgb_temp[4][2]=b04;
}

void run_static_leds(void){
	WS2812B_light();
}

void set_static_numLeds(uint8_t num_of_Leds){
	uint8_t n = 0;
	if(num_of_Leds>0)
		for(n=0;n<num_of_Leds;n++)
			WS2812B_pixel_rgb_to_BUFF_DMA( rgb_temp[0][0], rgb_temp[0][1], rgb_temp[0][2], n);
}

void set_colour_in_tape(uint8_t red,uint8_t green,uint8_t blue,uint8_t led_num)
{
			if(red<=128 && green<=128 && blue<=128 && led_num <= 60)
			{
				WS2812B_prepareValue(red         ,green       ,blue        ,
								     0		  	 ,0           ,0           ,
								     0		   	 ,0           ,0           ,
								     0		   	 ,0           ,0           ,
								     0		   	 ,0           ,0          );
				set_static_numLeds(led_num);
			}
}


void WS2812B_setValue(void)
{
  uint8_t n=0;
  for(n=0;n<30;n++){
    WS2812B_pixel_rgb_to_BUFF_DMA( rgb_temp[0][0], rgb_temp[0][1], rgb_temp[0][2], n*5);
    WS2812B_pixel_rgb_to_BUFF_DMA( rgb_temp[1][0], rgb_temp[1][1], rgb_temp[1][2], n*5+1);
    WS2812B_pixel_rgb_to_BUFF_DMA( rgb_temp[2][0], rgb_temp[2][1], rgb_temp[2][2], n*5+2);
    WS2812B_pixel_rgb_to_BUFF_DMA( rgb_temp[3][0], rgb_temp[3][1], rgb_temp[3][2], n*5+3);
    WS2812B_pixel_rgb_to_BUFF_DMA( rgb_temp[4][0], rgb_temp[4][1], rgb_temp[4][2], n*5+4);
	}
}

void WS2812B_light(void){
  HAL_TIM_PWM_Start_DMA(&htim1,TIM_CHANNEL_2,BUFF_DMA,(uint16_t)BUFF_LENGTH);
}

void ledsEffect_1(void)
{
	  uint8_t pos,count;
		WS2812B_prepareValue(				     0, 32, 128,
											     128, 0,128,
												 0, 128, 64,
												 0, 0, 0,
												 0, 0,0);	
		WS2812B_setValue();	
		for(count=0;count<50;count++)
		{
			memcpy((void*)OneLed_DMA_BUF_TEMP,(void*)(BUFF_DMA+48),96);
			for(pos=0;pos<149;pos++)
			{
					memcpy((void*)(pos*24+BUFF_DMA+48),(void*)(pos*24+BUFF_DMA+72),96);
			}
			memcpy((void*)(BUFF_DMA+48+3576),(void*)OneLed_DMA_BUF_TEMP,96);
			WS2812B_light();
			HAL_Delay(500);
		}	
}

void ledsEffect_2(void)
{
	  float count;
	  for(count=1;count<=16;count++){
				WS2812B_prepareValue(8*count,0,0, 
														 0,132-8*count,0,
														 0,0,8*count,
														 0,0,0,
														 0,0,0);	
				WS2812B_setValue();		
				WS2812B_light();
			  HAL_Delay(50);
		}
		for(count=16;count>0;count--){
				WS2812B_prepareValue(8*count,0,0, 
														 0,132-8*count,0,
														 0,0,8*count,
														 0,0,0,
														 0,0,0);			
				WS2812B_setValue();		
				WS2812B_light();
			  HAL_Delay(50);
		}
}

void ledsEffect_3(void){
		uint8_t count,
							red,
						green,
						 blue,
				 rand_num;
		for(count=0;count<150;count++){
			rand_num=rand()%6;
			if(rand_num==1){
				red=128;
				green=0;
				blue=0;
			}
			else if(rand_num==2){
				red=64;
			  green=64;
			  blue=0;
			}
			else if(rand_num==3){
				red=0;
				green=0;
				blue=128;
			}
			else if(rand_num==0){
				red=0;
				green=0;
				blue=128;
			}
			else if(rand_num==5){
				red=43;
				green=43;
				blue=43;
			}
			else if(rand_num==6){
				red=96;
				green=43;
				blue=43;
			}
			WS2812B_pixel_rgb_to_BUFF_DMA(red,green,blue,count);
			WS2812B_light();
			HAL_Delay(10);
	  }
		//WS2812B_clear();
		for(count=149;count;count--){
			  red=0;
			  green=0;
			  blue=0;
//			rand_num=rand()%6;
//			if(rand_num==1){
//				red=128;
//				green=0;
//				blue=0;
//			}
//			else if(rand_num==2){
//				red=64;
//			  green=64;
//			  blue=0;
//			}
//			else if(rand_num==3){
//				red=0;
//				green=0;
//				blue=128;
//			}
//			else if(rand_num==0){
//				red=0;
//				green=0;
//				blue=128;
//			}
//			else if(rand_num==5){
//				red=43;
//				green=43;
//				blue=43;
//			}
//			else if(rand_num==6){
//				red=96;
//				green=43;
//				blue=43;
//			}
//			else{
//				red=64;
//				green=0;
//				blue=64;
//			}
			WS2812B_pixel_rgb_to_BUFF_DMA(red,green,blue,count);
			WS2812B_light();
			HAL_Delay(10);
	}
		WS2812B_clear();
}

