#ifndef _TIMER_H_
#define _TIMER_H_
#include <stm32f10x.h>
/***************************************************************
*	功能：定时器3初始化为每隔Nms中断一次的方式。
*	参数：Period中断时间的长度，如果Period=N*10，那么每隔N毫秒中断一次。
**************************************************************/
void Timer2Init(u16 Period, FunctionalState State);
void Timer3Init(u16 Period, FunctionalState State);

int Delay50ms(void);

#endif
