#ifndef _TIMER_H_
#define _TIMER_H_
#include <stm32f10x.h>
/***************************************************************
*	���ܣ���ʱ��3��ʼ��Ϊÿ��Nms�ж�һ�εķ�ʽ��
*	������Period�ж�ʱ��ĳ��ȣ����Period=N*10����ôÿ��N�����ж�һ�Ρ�
**************************************************************/
void Timer2Init(u16 Period, FunctionalState State);
void Timer3Init(u16 Period, FunctionalState State);

int Delay50ms(void);

#endif
