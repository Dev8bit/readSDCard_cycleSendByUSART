#include "KEY.H"
Switch switch2,switch3;
void KeyInit(void)//�������IO��ʼ��
{
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOE | RCC_APB2Periph_GPIOD, ENABLE);//ʹ��GPIOD,GPIOEʱ��

    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;//��������
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;		
    GPIO_Init(GPIOE, &GPIO_InitStructure);//��ʼ��GPIOE0

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;		
    GPIO_Init(GPIOD, &GPIO_InitStructure);//��ʼ��



	switch2.PressedState=0;
	switch2.CheckLowTimes  =0;
	switch3.PressedState=0;
	switch3.CheckLowTimes  =0;	
}

