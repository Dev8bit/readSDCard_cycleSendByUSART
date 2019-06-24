#ifndef _KEY_H_
#define _KEY_H_
#include "stm32f10x.h"

#define S2 GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_0)
#define S3 GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_6)

typedef struct{
	volatile u8 PressedState;
	volatile u8 CheckLowTimes;
}Switch;

extern Switch switch2,switch3;



void KeyInit(void);//按键相关IO初始化



#endif
