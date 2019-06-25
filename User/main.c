#include "stm32f10x.h"
#include "sdio_sdcard.h"
#include "hw_config.h" 
#include "usb_lib.h"
#include "usb_pwr.h"
#include "RS232.h"	
#include "ff.h"
#include "KEY.H"
#include "Timer.h"
#include "DataProcessFun.h"
#include "dma.h"
#include "string.h"

int res, retCount = 0;  
FIL fdst;
FATFS fs;
UINT br, bw;

char packBuffer[1024];                      		//�洢��ȡ��������
volatile unsigned int packBufferLen = 0;
volatile char sendPackFlag = 0; 

#define CtrlLed3(x) (x)?GPIO_ResetBits(GPIOA, GPIO_Pin_8):GPIO_SetBits(GPIOA, GPIO_Pin_8)
#define CtrlLed4(x) (x)?GPIO_ResetBits(GPIOC, GPIO_Pin_7):GPIO_SetBits(GPIOC, GPIO_Pin_7)
static void InitSignalLed()
{
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOA |  RCC_APB2Periph_GPIOC, ENABLE);//ʹ��GPIOAʱ��
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;//����ģʽ
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//����50M  
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;		
	GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��		 

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;		
	GPIO_Init(GPIOC, &GPIO_InitStructure);//��ʼ��	

    CtrlLed3(0);
    CtrlLed4(0);
}

int main(void)
{
	/* USART1 config */
	KeyInit();
	Timer2Init(100, ENABLE);
	Timer3Init(5, ENABLE);
	RS232_Init(115200);
    InitSignalLed();
    
    DMA_Tx_Config(DMA1_Channel4,(u32)&USART1->DR,(u32)packBuffer, packBufferLen);

   	res = f_mount(&fs,"0:",1);
    
	Set_USBClock();
	USB_Interrupts_Config();
 	USB_Init();
    
    CtrlLed4(1); 		//USB״ָ̬ʾ����Ϊ�������ã�����3�������USB
 	while (bDeviceState != CONFIGURED) //�ȴ��������
	{
		if(1 == switch3.PressedState) 
		{
			switch3.PressedState = 0;
			break;
		}
	} 
    CtrlLed4(0); 
	
	CtrlLed3(1);		//�ļ�����ָʾ����Ϊ��ֹ��ȡ
	while (1) //�ȴ��������
	{
		if(1 == switch3.PressedState) 
		{
			PowerOff();	//�ر�USB
			switch3.PressedState = 0;
			break;
		}
	} 
	CtrlLed3(0);	
	
	res = f_open(&fdst, "0:NMEA_data.log", FA_OPEN_EXISTING | FA_READ);//ֻ����ʽ���ļ�					

	while(1)
	{
		if(1 == switch2.PressedState)		//����״̬��λ���ݲ���λ
		{
			retCount = 0;
            res = 0;
			while (0 == res && retCount < 3)		//����3�зǷ��ַ�����!!!������������!!!
			{
				res = ReadOnePack(packBuffer, 1024, &fdst);
				++retCount;
			}
			if(-2 == res)			//��ȡ�ļ���������ֹ����
			{
				f_close(&fdst);
                CtrlLed3(1);		//�ļ�����ָʾ����Ϊ��ȡ���
				while(1);
			}
			
            packBufferLen = strlen(packBuffer);
			sendPackFlag = 1;
			while(1 == sendPackFlag);
		}
	}
}

