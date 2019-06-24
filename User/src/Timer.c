#include "Timer.h"
#include "KEY.H"
#include "dma.h"

extern volatile char sendPackFlag;
extern volatile unsigned int packBufferLen;
/***************************************************************
*	功能：定时器2初始化为每隔Nms中断一次的方式。
*	参数：Period中断时间的长度，如果Period=N*10，那么每隔N毫秒中断一次。
**************************************************************/
void Timer2Init(u16 Period, FunctionalState State)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE); //时钟使能

	TIM_TimeBaseStructure.TIM_Period = Period * 10 - 1; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值	 计数到N*10为Nms
	TIM_TimeBaseStructure.TIM_Prescaler =(7200-1); //设置用来作为TIMx时钟频率除数的预分频值  10Khz的计数频率  
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位
 
	TIM_ClearITPendingBit(TIM2, TIM_IT_Update);		//清除TIM2更新中断标志，这样就不会立即更新
	TIM_ITConfig(  //使能或者失能指定的TIM中断
		TIM2, //TIM2
		TIM_IT_Update   //TIM 中断源
		| TIM_IT_Trigger,   //TIM 触发中断源 
		ENABLE  //使能
		);
	/*配置中断优先级*/
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;  //TIM3中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //先占优先级0级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;  //从优先级1级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
	NVIC_Init(&NVIC_InitStructure);  //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器

	TIM_Cmd(TIM2, State);  //使能TIMx外设	 
}


void TIM2_IRQHandler(void)   //TIM2中断服务程序
{
	if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET) //检查指定的TIM中断发生与否 
	{
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update  );  //清除TIMx的中断标志位
		MyDMA_Enable(DMA1_Channel4, packBufferLen);	
		while(DMA_GetFlagStatus(DMA1_FLAG_TC4) == RESET);
		DMA_ClearFlag(DMA1_FLAG_TC4);
		sendPackFlag = 0;
		TIM_Cmd(TIM2, DISABLE);
	}	
}

/***************************************************************
*	功能：定时器3初始化为每隔Nms中断一次的方式。
*	参数：Period中断时间的长度，如果Period=N*10，那么每隔N毫秒中断一次。
**************************************************************/
void Timer3Init(u16 Period, FunctionalState State)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); //时钟使能

	TIM_TimeBaseStructure.TIM_Period = Period * 10 - 1; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值	 计数到N*10为Nms
	TIM_TimeBaseStructure.TIM_Prescaler =(7200-1); //设置用来作为TIMx时钟频率除数的预分频值  10Khz的计数频率  
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位
 
	TIM_ITConfig(  //使能或者失能指定的TIM中断
		TIM3, //TIM2
		TIM_IT_Update  |  //TIM 中断源
		TIM_IT_Trigger,   //TIM 触发中断源 
		ENABLE  //使能
		);
	/*配置中断优先级*/
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;  //TIM3中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //先占优先级0级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  //从优先级3级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
	NVIC_Init(&NVIC_InitStructure);  //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器

	TIM_Cmd(TIM3, State);  //使能TIMx外设
							 
}

__IO long timTicks = 0;
void TIM3_IRQHandler(void)   //TIM3中断服务程序
{
	if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET) //检查指定的TIM中断发生与否 
	{
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update  );  //清除TIMx的中断标志位
		if(S2==0)
		{
			if(switch2.CheckLowTimes == 4)
			{
				switch2.PressedState = 1;
			}else if(switch2.CheckLowTimes < 4)
				switch2.CheckLowTimes += 1;
		}
		else
			switch2.CheckLowTimes=0;

		if(S3==0)
		{
			if(switch3.CheckLowTimes == 4)//如果连续检测到低电平的次数=4次，就认为按键被成功按下
			{
				switch3.PressedState = 1;
			}else if(switch3.CheckLowTimes < 4)
				switch3.CheckLowTimes += 1;
		}
		else
			switch3.CheckLowTimes=0;

        timTicks += 5;
	}	
}

int Delay50ms(void)
{
    long currentTime = timTicks;
    
    while(timTicks - currentTime < 50);
    
    return 0;
}
