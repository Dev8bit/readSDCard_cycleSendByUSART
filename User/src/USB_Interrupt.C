#include "usb_lib.h"
#include "usb_istr.h"

/*************************************************************
*	功能  ：USB低优先级中断请求
************************************************************/
void USB_LP_CAN1_RX0_IRQHandler(void)
{
  USB_Istr();
}

/***********************************************************
*	功能  ：USB高优先级中断请求 或发送中断
 ********************************************************/
void USB_HP_CAN1_TX_IRQHandler(void)
{
  CTR_HP();
}


