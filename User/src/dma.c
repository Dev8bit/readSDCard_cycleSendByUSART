#include "dma.h"


void DMA_Tx_Config(DMA_Channel_TypeDef* DMA_CHx,u32 cpar,u32 cmar,u16 cndtr)
{
	DMA_InitTypeDef DMA_InitStructure;
	
 	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);					//ʹ��DMA����
	DMA_DeInit(DMA_CHx);   												//��DMA��ͨ��1�Ĵ�������Ϊȱʡֵ

	DMA_InitStructure.DMA_PeripheralBaseAddr = cpar;  					//DMA�������ַ
	DMA_InitStructure.DMA_MemoryBaseAddr = cmar;  						//DMA�ڴ����ַ
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;  				//���ݴ��䷽�򣬴��ڴ��ȡ���͵�����
	DMA_InitStructure.DMA_BufferSize = cndtr;  							//DMAͨ����DMA����Ĵ�С
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;  	//�����ַ�Ĵ�������
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;  			//�ڴ��ַ�Ĵ�������
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;  //���ݿ���Ϊ8λ
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte; 	//���ݿ���Ϊ8λ
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;  						//��������������ģʽ
	DMA_InitStructure.DMA_Priority = DMA_Priority_Medium; 				//DMAͨ�� xӵ��mid���ȼ� 
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;  						//DMAͨ��xû������Ϊ�ڴ浽�ڴ洫��
	DMA_Init(DMA_CHx, &DMA_InitStructure);  							//����DMA_InitStruct��ָ���Ĳ�����ʼ��DMA��ͨ��USART1_Tx_DMA_Channel����ʶ�ļĴ���  	
} 

//����һ��DMA����
void MyDMA_Enable(DMA_Channel_TypeDef* DMA_CHx, uint16_t num)
{ 
	DMA_Cmd(DMA_CHx, DISABLE);  			//�ر�USART1 TX DMA1 ��ָʾ��ͨ��      
 	DMA_SetCurrDataCounter(DMA_CHx, num);	//DMAͨ����DMA����Ĵ�С
 	DMA_Cmd(DMA_CHx, ENABLE);  				//ʹ��USART1 TX DMA1 ��ָʾ��ͨ�� 
}	

 
























