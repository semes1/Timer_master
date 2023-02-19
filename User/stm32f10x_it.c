/**
  ******************************************************************************
  * @file    Project/STM32F10x_StdPeriph_Template/stm32f10x_it.c 
  * @author  MCD Application Team
  * @version V3.5.0
  * @date    08-April-2011
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and 
  *          peripherals interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x_it.h"
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "bsp_exti.h"
#include "bsp_timer.h"

extern SemaphoreHandle_t  BinarySem1_Handle;
extern SemaphoreHandle_t  BinarySem2_Handle;

vu32 Time3value = 0;
vu16 Time1value = 0;
vu8 Timer1flag = 0;
vu8 EXTIX_Flag = 0;

vu8 ek = 0;

/** @addtogroup STM32F10x_StdPeriph_Template
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M3 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
//void SVC_Handler(void)
//{
//}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
//void PendSV_Handler(void)
//{
//}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
extern void xPortSysTickHandler(void);
//systick中断服务函数
void SysTick_Handler(void)
{	
#if (INCLUDE_xTaskGetSchedulerState  == 1 )
  if (xTaskGetSchedulerState() != taskSCHEDULER_NOT_STARTED)
  {
#endif  /* INCLUDE_xTaskGetSchedulerState */  
    
    xPortSysTickHandler();
    
#if (INCLUDE_xTaskGetSchedulerState  == 1 )
  }
#endif  /* INCLUDE_xTaskGetSchedulerState */
}


/*      外部中断服务函数    */
void EXTIX_IRQHandler(void)                                                     /* 1 */
{
	static uint8_t exti_i = 0;
	BaseType_t pxHigherPriorityTaskWoken,xResult;
	if(EXTI_GetITStatus(EXTI_LineX) != RESET)
	{
//		ek++;
		EXTI_ClearITPendingBit(EXTI_LineX);
		exti_i++;
		pxHigherPriorityTaskWoken = pdFALSE;
		if(exti_i == 10)
		{
			exti_i = 0;
			xResult = xSemaphoreGiveFromISR(BinarySem1_Handle ,&pxHigherPriorityTaskWoken);
			if(xResult != pdFAIL)
			{
	
				portYIELD_FROM_ISR(pxHigherPriorityTaskWoken);
			}
			EXTIX_Flag = 1;
		}
	}
}

/*     串口1中断服务程序     */

void USART1_IRQHandler(void)            	
{
	BaseType_t pxHigherPriorityTaskWoken,xResult;
	if(USART_GetITStatus(USART2,USART_IT_RXNE) != RESET)
	{
		USART_ClearITPendingBit(USART2, USART_IT_RXNE);
		pxHigherPriorityTaskWoken = pdFALSE;
		xResult = xSemaphoreGiveFromISR(BinarySem2_Handle ,&pxHigherPriorityTaskWoken);
		if(xResult != pdFAIL)
		{
			portYIELD_FROM_ISR(pxHigherPriorityTaskWoken);
		}
	}
}



//定时器3中断服务函数
void TIM3_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM3,TIM_FLAG_Update)!=RESET){
		TIM_ClearITPendingBit(TIM3,TIM_FLAG_Update);
		Time3value++;
		if(Time3value >= 50000)                                                //防止变量溢出程序跑飞.（50000/100/60）八分钟
		{
			Time3value = 0;
		}
	}
}


void ADVANCE_TIM1_CCx_IRQHandler(void)
{
	BaseType_t pxHigherPriorityTaskWoken,xResult;
	static uint8_t exti_i = 0;
	if (TIM_GetITStatus(ADVANCE_TIMEX, ADVANCE_TIM1_IT_CCx) != 0) //通道1捕获
	{
		TIM_ClearITPendingBit(ADVANCE_TIMEX, ADVANCE_TIM1_IT_CCx);
		exti_i++;
		pxHigherPriorityTaskWoken = pdFALSE;
		if(exti_i == 2)
		{
			exti_i = 0;
			xResult = xSemaphoreGiveFromISR(BinarySem1_Handle ,&pxHigherPriorityTaskWoken);
			if(xResult != pdFAIL)
			{
	
				portYIELD_FROM_ISR(pxHigherPriorityTaskWoken);
			}
			EXTIX_Flag = 1;
		}
	}
}


void TIM4_IRQHandler(void)
{
	BaseType_t pxHigherPriorityTaskWoken,xResult;
	if(TIM_GetITStatus(TIM4,TIM_FLAG_Update)!=RESET)
	{
		TIM_ClearITPendingBit(TIM4,TIM_FLAG_Update);
		Time1value++;
		if(Time1value >= 10)                                                //防止变量溢出程序跑飞
		{
			pxHigherPriorityTaskWoken = pdFALSE;
			xResult = xSemaphoreGiveFromISR(BinarySem2_Handle ,&pxHigherPriorityTaskWoken);
			if(xResult != pdFAIL)
			{
				portYIELD_FROM_ISR(pxHigherPriorityTaskWoken);
			}
			Timer1flag = 1;
			Time1value = 0;
		}
	}	
}

/******************************************************************************/
/*                 STM32F10x Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f10x_xx.s).                                            */
/******************************************************************************/

/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval None
  */
/*void PPP_IRQHandler(void)
{
}*/

/**
  * @}
  */ 


/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
