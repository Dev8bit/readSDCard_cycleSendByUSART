#include "Timer.h"
#include "KEY.H"
#include "dma.h"

extern volatile char sendPackFlag;
extern volatile unsigned int packBufferLen;
/***************************************************************
*	���ܣ���ʱ��2��ʼ��Ϊÿ��Nms�ж�һ�εķ�ʽ��
*	������Period�ж�ʱ��ĳ��ȣ����Period=N*10����ôÿ��N�����ж�һ�Ρ�
**************************************************************/
void Timer2Init(u16 Period, FunctionalState State)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE); //ʱ��ʹ��

	TIM_TimeBaseStructure.TIM_Period = Period * 10 - 1; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ	 ������N*10ΪNms
	TIM_TimeBaseStructure.TIM_Prescaler =(7200-1); //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ  10Khz�ļ���Ƶ��  
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure); //����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
 
	TIM_ClearITPendingBit(TIM2, TIM_IT_Update);		//���TIM2�����жϱ�־�������Ͳ�����������
	TIM_ITConfig(  //ʹ�ܻ���ʧ��ָ����TIM�ж�
		TIM2, //TIM2
		TIM_IT_Update   //TIM �ж�Դ
		| TIM_IT_Trigger,   //TIM �����ж�Դ 
		ENABLE  //ʹ��
		);
	/*�����ж����ȼ�*/
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;  //TIM3�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //��ռ���ȼ�0��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;  //�����ȼ�1��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQͨ����ʹ��
	NVIC_Init(&NVIC_InitStructure);  //����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ���

	TIM_Cmd(TIM2, State);  //ʹ��TIMx����	 
}


void TIM2_IRQHandler(void)   //TIM2�жϷ������
{
	if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET) //���ָ����TIM�жϷ������ 
	{
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update  );  //���TIMx���жϱ�־λ
		MyDMA_Enable(DMA1_Channel4, packBufferLen);	
		while(DMA_GetFlagStatus(DMA1_FLAG_TC4) == RESET);
		DMA_ClearFlag(DMA1_FLAG_TC4);
		sendPackFlag = 0;
		TIM_Cmd(TIM2, DISABLE);
	}	
}

/***************************************************************
*	���ܣ���ʱ��3��ʼ��Ϊÿ��Nms�ж�һ�εķ�ʽ��
*	������Period�ж�ʱ��ĳ��ȣ����Period=N*10����ôÿ��N�����ж�һ�Ρ�
**************************************************************/
void Timer3Init(u16 Period, FunctionalState State)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); //ʱ��ʹ��

	TIM_TimeBaseStructure.TIM_Period = Period * 10 - 1; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ	 ������N*10ΪNms
	TIM_TimeBaseStructure.TIM_Prescaler =(7200-1); //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ  10Khz�ļ���Ƶ��  
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
 
	TIM_ITConfig(  //ʹ�ܻ���ʧ��ָ����TIM�ж�
		TIM3, //TIM2
		TIM_IT_Update  |  //TIM �ж�Դ
		TIM_IT_Trigger,   //TIM �����ж�Դ 
		ENABLE  //ʹ��
		);
	/*�����ж����ȼ�*/
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;  //TIM3�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //��ռ���ȼ�0��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  //�����ȼ�3��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQͨ����ʹ��
	NVIC_Init(&NVIC_InitStructure);  //����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ���

	TIM_Cmd(TIM3, State);  //ʹ��TIMx����
							 
}

__IO long timTicks = 0;
void TIM3_IRQHandler(void)   //TIM3�жϷ������
{
	if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET) //���ָ����TIM�жϷ������ 
	{
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update  );  //���TIMx���жϱ�־λ
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
			if(switch3.CheckLowTimes == 4)//���������⵽�͵�ƽ�Ĵ���=4�Σ�����Ϊ�������ɹ�����
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
