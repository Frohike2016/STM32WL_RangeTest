/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    sys_debug.c
  * @author  MCD Application Team
  * @brief   Enables 4 debug pins for internal signals RealTime debugging
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "platform.h"
#include "sys_debug.h"

/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* External variables ---------------------------------------------------------*/
/* USER CODE BEGIN EV */

/* USER CODE END EV */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
typedef struct {
#if defined(USE_FULL_ASSERT) && !defined(ASSERT_FILE_LOG)
  uint16_t numOfAssertFailed;
#endif /* USE_FULL_ASSERT && !ASSERT_FILE_LOG */
#ifdef SEGGER_RTT
  cb_debug_timestamp *timestamp_func;
  uint8_t logBuff[32]; /* buffer for channel 1: log */
#endif /* SEGGER_RTT */
} DEBUG_Context;
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define MAX_TIMESTAMP_SIZE	16
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN PV */
DEBUG_Context DEBUG_Ctx;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Exported functions --------------------------------------------------------*/

/**
  * @brief Disable debugger (serial wires pins)
  */
void DBG_Disable(void)
{
  /* USER CODE BEGIN DBG_Init_1 */

  /* USER CODE END DBG_Init_1 */

#if defined (DEBUGGER_ENABLED) && ( DEBUGGER_ENABLED == 0 ) /* DEBUGGER_DISABLED */
  /* Put the debugger pin PA13 and P14 in analog for LowPower*/
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  GPIO_InitStruct.Mode   = GPIO_MODE_ANALOG;
  GPIO_InitStruct.Pull   = GPIO_NOPULL;
  GPIO_InitStruct.Pin    = (GPIO_PIN_13 | GPIO_PIN_14);
  /* make sure clock is enabled before setting the pins with HAL_GPIO_Init() */
  __HAL_RCC_GPIOA_CLK_ENABLE() ;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

#elif !defined (DEBUGGER_ENABLED)
#error "DEBUGGER_ENABLED not defined or out of range <0,1>"
#endif /* DEBUGGER_DISABLED */

  /* Disabled HAL_DBGMCU_  */
  DBG_ConfigForLpm(false);

  /* USER CODE BEGIN DBG_Init_Last */
#if defined(USE_FULL_ASSERT) && !defined(ASSERT_FILE_LOG)
  DEBUG_Ctx.numOfAssertFailed = 0;
#endif /* USE_FULL_ASSERT && !ASSERT_FILE_LOG */
  /* USER CODE END DBG_Init_Last */
}

#if defined (DEBUGGER_ENABLED) && ( DEBUGGER_ENABLED == 1 )
void DBG_Init(void)
{
GPIO_InitTypeDef  GPIO_InitStruct = {0};

  /* Enable the GPIO Clock */
  DGB_LINE1_CLK_ENABLE();
  DGB_LINE2_CLK_ENABLE();
  DGB_LINE3_CLK_ENABLE();
  DGB_LINE4_CLK_ENABLE();

  /* Configure the GPIO pin */
  GPIO_InitStruct.Mode   = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull   = GPIO_PULLUP;
  GPIO_InitStruct.Speed  = GPIO_SPEED_FREQ_VERY_HIGH;

  GPIO_InitStruct.Pin    = DGB_LINE1_PIN;
  HAL_GPIO_Init(DGB_LINE1_PORT, &GPIO_InitStruct);
  GPIO_InitStruct.Pin    = DGB_LINE2_PIN;
  HAL_GPIO_Init(DGB_LINE2_PORT, &GPIO_InitStruct);
  GPIO_InitStruct.Pin    = DGB_LINE3_PIN;
  HAL_GPIO_Init(DGB_LINE3_PORT, &GPIO_InitStruct);
  GPIO_InitStruct.Pin    = DGB_LINE4_PIN;
  HAL_GPIO_Init(DGB_LINE4_PORT, &GPIO_InitStruct);

  /* Reset debug Pins */
  HAL_GPIO_WritePin(DGB_LINE1_PORT, DGB_LINE1_PIN, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(DGB_LINE2_PORT, DGB_LINE2_PIN, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(DGB_LINE3_PORT, DGB_LINE3_PIN, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(DGB_LINE4_PORT, DGB_LINE4_PIN, GPIO_PIN_RESET);

  /*spi dbg*/
  GPIO_InitStruct.Mode   = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull   = GPIO_NOPULL;
  GPIO_InitStruct.Pin    = (GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7);
  GPIO_InitStruct.Alternate  = GPIO_AF13_DEBUG_SUBGHZSPI;
  __HAL_RCC_GPIOA_CLK_ENABLE();
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /* Busy */
  GPIO_InitStruct.Mode   = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull   = GPIO_NOPULL;
  GPIO_InitStruct.Pin    = (GPIO_PIN_12);
  GPIO_InitStruct.Alternate  = GPIO_AF6_RF_BUSY;
  __HAL_RCC_GPIOA_CLK_ENABLE() ;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /******** MCO OUT Config on PA8 *****/
  HAL_RCC_MCOConfig(RCC_MCO1, RCC_MCO1SOURCE_SYSCLK, RCC_MCODIV_16);

  /*Debug power up request wakeup CBDGPWRUPREQ*/
  LL_EXTI_EnableIT_32_63(LL_EXTI_LINE_46);

  /* lowpower DBGmode: just needed for CORE_CM4 */
  DBG_ConfigForLpm(true);

  /* USER CODE BEGIN DBG_Init_Last */
#if defined(USE_FULL_ASSERT) && !defined(ASSERT_FILE_LOG)
  DEBUG_Ctx.numOfAssertFailed = 0;
#endif /* USE_FULL_ASSERT && !ASSERT_FILE_LOG */
#ifdef SEGGER_RTT
  /* initialize the SEGGER RTT Control Block and the Channel 0 */
  SEGGER_RTT_Init();
  /* add channel 1: log (In no Blocking mode) */
  SEGGER_RTT_ConfigUpBuffer(1, "Log", DEBUG_Ctx.logBuff, sizeof(DEBUG_Ctx.logBuff), SEGGER_RTT_MODE_NO_BLOCK_TRIM);
#endif /* SEGGER_RTT */
  /* USER CODE END DBG_Init_Last */
}
#endif /* DEBUGGER_ENABLED */

/**
  * @brief Config debugger when working in Low Power Mode
  * @note  When in Dual Core DbgMcu pins should be better disable only after Cm0 is started
  */
void DBG_ConfigForLpm(uint8_t enableDbg)
{
  uint8_t enable_dbg = enableDbg;
  /* USER CODE BEGIN DBG_ConfigForLpm_1 */

  /* USER CODE END DBG_ConfigForLpm_1 */

#if defined (DEBUGGER_ENABLED) && ( DEBUGGER_ENABLED == 0 ) /* DEBUGGER_DISABLED */
  enable_dbg = 0;
#elif !defined (DEBUGGER_ENABLED)
#error "DEBUGGER_ENABLED not defined or out of range <0,1>"
#endif /* DEBUGGER_DISABLED */

  if (enable_dbg == 1)
  {
    HAL_DBGMCU_EnableDBGSleepMode();
    HAL_DBGMCU_EnableDBGStopMode();
    HAL_DBGMCU_EnableDBGStandbyMode();
  }
  else
  {
    HAL_DBGMCU_DisableDBGSleepMode();
    HAL_DBGMCU_DisableDBGStopMode();
    HAL_DBGMCU_DisableDBGStandbyMode();
  }

  /* USER CODE BEGIN DBG_ConfigForLpm_Last */

  /* USER CODE END DBG_ConfigForLpm_Last */
}

#if defined(USE_FULL_ASSERT) && !defined(ASSERT_FILE_LOG)
void DBG_Incr_AssertFailed ( void )
{
  DEBUG_Ctx.numOfAssertFailed++;
}
uint16_t DBG_Get_AssertFailed ( void )
{
  return DEBUG_Ctx.numOfAssertFailed;
}
#endif /* USE_FULL_ASSERT && !ASSERT_FILE_LOG */

/* USER CODE BEGIN EF */
#ifdef SEGGER_RTT
void UTIL_DEBUG_RegisterTimeStampFunction(cb_debug_timestamp *cb)
{
  DEBUG_Ctx.timestamp_func = *cb;
}

void SEGGER_PRINTF( bool timeStamp, Segger_terminal_t tID, const char * sFormat, ... )
{
  int error = 0;
  uint8_t bufferID = 0; // Later on we will change if terminal is LOG
  va_list ParamList;

  if (tID >= TERMINAL_SIZE)
  {
 #ifdef USE_FULL_ASSERT
  #ifdef ASSERT_FILE_LOG
   assert_failed((uint8_t *)__FILE__, __LINE__);
  #else
   assert_failed ( );
  #endif /* ASSERT_FILE_LOG */
 #endif /* USE_FULL_ASSERT */
  }
  else if (tID == LOG_T) bufferID = 1; // Set the SEGGER UP-Buffer to use LOG Buffer

  if (timeStamp) // We will print Timestamp at the beginning of the message
  {
	uint8_t tsBuffer[MAX_TIMESTAMP_SIZE];
	uint16_t tsSize;

	//Get Timestamp
	DEBUG_Ctx.timestamp_func(tsBuffer, &tsSize);

	if (tID == LOG_T)  error = SEGGER_RTT_Write(1, tsBuffer, tsSize); // print Timestamp
	else
	{
      error = SEGGER_RTT_SetTerminal(tID); // Configure the proper terminal
	  if (error >= 0)  error = SEGGER_RTT_Write(0, tsBuffer, tsSize); // print Timestamp
	}
  }

  if (error >= 0)
  {
    va_start(ParamList, sFormat);
  	SEGGER_RTT_vprintf(bufferID, sFormat, &ParamList);
  	va_end(ParamList);
  }
}
#endif /* SEGGER_RTT */
/* USER CODE END EF */

/* Private Functions Definition -----------------------------------------------*/
void DBG_ProbesInit(void)
{
  /* USER CODE BEGIN DBG_ProbesInit_1 */

  /* USER CODE END DBG_ProbesInit_1 */

  /* SW probes */

#if defined (PROBE_PINS_ENABLED) && ( PROBE_PINS_ENABLED == 1 )
  GPIO_InitTypeDef  GPIO_InitStruct = {0};

  /* Configure the GPIO pin */
  GPIO_InitStruct.Mode   = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull   = GPIO_PULLUP;
  GPIO_InitStruct.Speed  = GPIO_SPEED_FREQ_VERY_HIGH;

  /* Enable the GPIO Clock */

  /* Reset probe Pins */

  /* USER CODE BEGIN DBG_ProbesInit_2 */

  /* USER CODE END DBG_ProbesInit_2 */
  /* HW alternate functions for monitoring RF */

  /* Configure the GPIO pin */
  GPIO_InitStruct.Speed  = GPIO_SPEED_FREQ_VERY_HIGH;

  /*spi dbg*/
  GPIO_InitStruct.Mode   = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull   = GPIO_NOPULL;
  GPIO_InitStruct.Pin    = (GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7);
  GPIO_InitStruct.Alternate  = GPIO_AF13_DEBUG_SUBGHZSPI;
  __HAL_RCC_GPIOA_CLK_ENABLE();
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /* Busy */
  GPIO_InitStruct.Mode   = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull   = GPIO_NOPULL;
  GPIO_InitStruct.Pin    = (GPIO_PIN_12);
  GPIO_InitStruct.Alternate  = GPIO_AF6_RF_BUSY;
  __HAL_RCC_GPIOA_CLK_ENABLE() ;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /* LDO_rdy & BUCK_rdy (SMPS) */
  GPIO_InitStruct.Mode   = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull   = GPIO_NOPULL;
  GPIO_InitStruct.Pin    = (GPIO_PIN_2);
  GPIO_InitStruct.Alternate  = GPIO_AF13_DEBUG_RF;
  __HAL_RCC_GPIOB_CLK_ENABLE();
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  GPIO_InitStruct.Mode   = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull   = GPIO_NOPULL;
  GPIO_InitStruct.Pin    = (GPIO_PIN_4);
  GPIO_InitStruct.Alternate  = GPIO_AF13_DEBUG_RF;
  __HAL_RCC_GPIOB_CLK_ENABLE();
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

#elif !defined (PROBE_PINS_ENABLED)
#error "PROBE_PINS_ENABLED not defined or out of range <0,1>"
#endif /* PROBE_PINS_ENABLED */

  /* USER CODE BEGIN DBG_ProbesInit_3 */

  /* USER CODE END DBG_ProbesInit_3 */

#if defined(CORE_CM4)
#if defined (DEBUGGER_ENABLED) && ( DEBUGGER_ENABLED == 1 )
  /*Debug power up request wakeup CBDGPWRUPREQ*/
  LL_EXTI_EnableIT_32_63(LL_EXTI_LINE_46);
#elif !defined (DEBUGGER_ENABLED)
#error "DEBUGGER_ENABLED not defined or out of range <0,1>"
#endif /* DEBUGGER_ENABLED */
#endif /* CORE_CM4 */

  /* USER CODE BEGIN DBG_ProbesInit_Last */

  /* USER CODE END DBG_ProbesInit_Last */
}
/* USER CODE BEGIN PrFD */

/* USER CODE END PrFD */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
