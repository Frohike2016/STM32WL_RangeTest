/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    sys_debug.h
  * @author  MCD Application Team
  * @brief   Configuration of the debug.c instances
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __SYS_DEBUG_H__
#define __SYS_DEBUG_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "sys_conf.h"
#include "platform.h"

/* USER CODE BEGIN Includes */
#ifdef SEGGER_RTT
#include "SEGGER_RTT.h"
#endif /* SEGGER_RTT */
/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */
#ifdef SEGGER_RTT
typedef void cb_debug_timestamp(uint8_t *pData, uint16_t *Size);
#endif /* SEGGER_RTT */

typedef enum
{
  APP_T  = 0,	// Application Terminal
  IO_T   = 1,	// Gpio Terminal
  WDG_T  = 2,	// Watchdog Terminal
  RTC_T	 = 3,	// RTC Terminal
  TIM_T  = 4,	// TIMER Terminal
  ADC_T	 = 5,	// ADC Terminal
  UART_T = 6,	// UART Terminal
  SPI_T  = 7,	// SPI Terminal
  I2C_T  = 8,	// I2C Terminal
  DMA_T  = 9,	// DMA Terminal
  CRYP_T = 10,	// Cryptographic Hardware Terminal
  MDDW_T = 11,	// Middleware Terminal
  WARN_T = 12,	// Warning Terminal
  LOG_T  = 13,	// LOG Terminal
  TERMINAL_SIZE,
} Segger_terminal_t;

// Used to save the state of the pins
typedef enum
{
  OUTPUT,
  OUTPUT_PULLUP,
  OUTPUT_PULLDOWN,
  INPUT_DIGITAL,
  INPUT_ANALOG,
  UART,
  PORT_CLOCK_ON,
  PORT_CLOCK_OFF,
} Pin_mode_t;

//TODO: create the struct to save the state of the pins
//  .Pin_mode
//  .Pin_clock

//TODO: implement uint8 SWBUG array variable to notify unwanted code implementations
 typedef enum
 {
   NO_SWBUG,      				/*!< There was no SWBUG                                            */
   WWDG_SWBUG,      			/*!< Window WatchDog Interrupt                                     */
   PVD_SWBUG,					/*!< PVD through EXTI Line detect Interrupt                        */
   RTC_SWBUG,					/*!< RTC through EXTI Line Interrupt                               */
   FLASH_SWBUG,					/*!< FLASH Interrupt                                               */
   RCC_SWBUG,					/*!< RCC Interrupt                                                 */
   EXTI0_1_SWBUG,				/*!< EXTI Line 0 and 1 Interrupts                                  */
   EXTI2_3_SWBUG,				/*!< EXTI Line 2 and 3 Interrupts                                  */
   EXTI4_15_SWBUG,				/*!< EXTI Line 4 to 15 Interrupts                                  */
   DMA1_Channel1_SWBUG,			/*!< DMA1 Channel 1 Interrupt                                      */
   DMA1_Channel2_3_SWBUG,		/*!< DMA1 Channel 2 and Channel 3 Interrupts                       */
   DMA1_Channel4_5_6_7_SWBUG,	/*!< DMA1 Channel 4, Channel 5, Channel 6 and Channel 7 Interrupts */
   ADC1_SWBUG,					/*!< A function that needs ADC was called before HW_ADCInit( )     */
   LPTIM1_SWBUG,				/*!< LPTIM1 Interrupt                                              */
   TIM2_SWBUG,					/*!< TIM2 Interrupt                                                */
   TIM21_SWBUG,					/*!< TIM21 Interrupt                                               */
   TIM22_SWBUG,					/*!< TIM22 Interrupt                                               */
   I2C1_SWBUG,					/*!< I2C1 Interrupt                                                */
   SPI1_SWBUG,					/*!< SPI1 Interrupt                                                */
   USART2_SWBUG,				/*!< USART2 Interrupt                                              */
   LPUART1_SWBUG,				/*!< LPUART1 Interrupt                                             */
 } SWBUG_Type;
/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* Pin defines */

/**  Definition for Debug Line 1   **/
/**
  * @brief Pin of Debug Line 1
  */
#define DGB_LINE1_PIN                           GPIO_PIN_12

/**
  * @brief Port of Debug Line 1
  */
#define DGB_LINE1_PORT                          GPIOB

/**
  * @brief Enable GPIOs clock of Debug Line 1
  */
#define DGB_LINE1_CLK_ENABLE()                  __HAL_RCC_GPIOB_CLK_ENABLE()

/**
  * @brief Disable GPIOs clock of Debug Line 1
  */
#define DGB_LINE1_CLK_DISABLE()                 __HAL_RCC_GPIOB_CLK_DISABLE()

/**  Definition for Debug Line 2   **/
/**
  * @brief Pin of Debug Line 2
  */
#define DGB_LINE2_PIN                           GPIO_PIN_13

/**
  * @brief Port of Debug Line 2
  */
#define DGB_LINE2_PORT                          GPIOB

/**
  * @brief Enable GPIOs clock of Debug Line 2
  */
#define DGB_LINE2_CLK_ENABLE()                  __HAL_RCC_GPIOB_CLK_ENABLE()

/**
  * @brief Disable GPIOs clock of Debug Line 2
  */
#define DGB_LINE2_CLK_DISABLE()                 __HAL_RCC_GPIOB_CLK_DISABLE()

/**  Definition for Debug Line 3   **/
/**
  * @brief Pin of Debug Line 3
  */
#define DGB_LINE3_PIN                           GPIO_PIN_14

/**
  * @brief Port of Debug Line 3
  */
#define DGB_LINE3_PORT                          GPIOB

/**
  * @brief Enable GPIOs clock of Debug Line 3
  */
#define DGB_LINE3_CLK_ENABLE()                  __HAL_RCC_GPIOB_CLK_ENABLE()

/**
  * @brief Disable GPIOs clock of Debug Line 3
  */
#define DGB_LINE3_CLK_DISABLE()                 __HAL_RCC_GPIOB_CLK_DISABLE()

/**  Definition for Debug Line 4   **/
/**
  * @brief Pin of Debug Line 4
  */
#define DGB_LINE4_PIN                           GPIO_PIN_10

/**
  * @brief Port of Debug Line 4
  */
#define DGB_LINE4_PORT                          GPIOB

/**
  * @brief Enable GPIOs clock of Debug Line 4
  */
#define DGB_LINE4_CLK_ENABLE()                  __HAL_RCC_GPIOB_CLK_ENABLE()

/**
  * @brief Disable GPIOs clock of Debug Line 4
  */
#define DGB_LINE4_CLK_DISABLE()                 __HAL_RCC_GPIOB_CLK_DISABLE()

/* USER CODE BEGIN EC */
#define TS_OFF 			0 // Log without TimeStamp
#define TS_ON 			1 // Log with TimeStamp

#ifdef SEGGER_RTT
/* There are 16 virtual terminals, the remaining 5 ones can be used for other purposes and defined externally */
#define TERMINAL_APP	'0'  // Application will use Terminal 0 (and also LOG buffer for events that occurs only once)
#define TERMINAL_IO		'1'
#define TERMINAL_WDG	'2'
#define TERMINAL_RTC	'3'
#define TERMINAL_TIM	'4'
#define TERMINAL_ADC	'5'
#define TERMINAL_UART	'6'
#define TERMINAL_SPI	'7'
#define TERMINAL_I2C	'8'
#define TERMINAL_DMA	'9'
#define TERMINAL_CRYP	'A'
#endif /* SEGGER_RTT */

#define APP_TAG			"|APP| "
#define IO_TAG			"|GPIO| "
#define WDG_TAG			"|WDG| "
#define RTC_TAG			"|RTC| "
#define TIM_TAG			"|TIM| "
#define ADC_TAG			"|ADC| "
#define UART_TAG		"|UART| "
#define SPI_TAG			"|SPI| "
#define I2C_TAG			"|I2C| "
#define DMA_TAG			"|DMA| "
#define CRYP_TAG		"|CRYP| "
#define TSRV_TAG		"|TSRV| "
#define EPD_TAG			"|EPD| "
#define ERROR_TAG		"Terminal code error"
#define _NL_			"\r\n"
/* USER CODE END EC */

/* External variables --------------------------------------------------------*/
/* USER CODE BEGIN EV */

/* USER CODE END EV */

/* Exported macro ------------------------------------------------------------*/
#if defined (PROBE_PINS_ENABLED) && (PROBE_PINS_ENABLED == 1)

#define PROBE_GPIO_WRITE( gpio, n, x )  HAL_GPIO_WritePin( gpio, n, (GPIO_PinState)(x) )

/**
  * @brief Set pin to high level
  */
#define PROBE_GPIO_SET_LINE( gpio, n )       LL_GPIO_SetOutputPin( gpio, n )

/**
  * @brief Set pin to low level
  */
#define PROBE_GPIO_RST_LINE( gpio, n )       LL_GPIO_ResetOutputPin ( gpio, n )

/*enabling RTC_OUTPUT should be set via STM32CubeMX GUI because MX_RTC_Init is now entirely generated */

#elif defined (PROBE_PINS_ENABLED) && (PROBE_PINS_ENABLED == 0) /* PROBE_PINS_OFF */

#define PROBE_GPIO_WRITE( gpio, n, x )

/**
  * @brief not usable
  */
#define PROBE_GPIO_SET_LINE( gpio, n )

/**
  * @brief not usable
  */
#define PROBE_GPIO_RST_LINE( gpio, n )

/*disabling RTC_OUTPUT should be set via STM32CubeMX GUI because MX_RTC_Init is now entirely generated */

#else
#error "PROBE_PINS_ENABLED not defined or out of range <0,1>"
#endif /* PROBE_PINS_ENABLED */

/* USER CODE BEGIN EM */
#ifdef SEGGER_RTT
#define DBG_PRINTF(TS, TERMINAL, ...)   SEGGER_PRINTF( TS, TERMINAL, __VA_ARGS__ )
#endif /* SEGGER_RTT */
/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
/**
  * @brief Disable debugger (serial wires pins)
  */
void DBG_Disable(void);

/**
  * @brief Config debugger when working in Low Power Mode
  * @note  When in Dual Core DbgMcu pins should be better disable only after CM0+ is started
  */
void DBG_ConfigForLpm(uint8_t enableDbg);

/**
  * @brief Initializes the SW probes pins and the monitor RF pins via Alternate Function
  */
void DBG_ProbesInit(void);

#if defined (DEBUGGER_ENABLED) && (DEBUGGER_ENABLED == 1)
/**
  * @brief Initializes the debug
  */
void DBG_Init(void);
#endif /* DEBUGGER_ENABLED */

/* USER CODE BEGIN EFP */
#if defined(USE_FULL_ASSERT) && !defined(ASSERT_FILE_LOG)
void DBG_Incr_AssertFailed ( void );
uint16_t DBG_Get_AssertFailed ( void );
#endif /* USE_FULL_ASSERT && !ASSERT_FILE_LOG */
#ifdef SEGGER_RTT
void UTIL_DEBUG_RegisterTimeStampFunction(cb_debug_timestamp *cb);
void SEGGER_PRINTF( bool timeStamp, Segger_terminal_t tID, const char * sFormat, ... );
#endif /* SEGGER_RTT */
/* USER CODE END EFP */

#ifdef __cplusplus
}
#endif

#endif /* __SYS_DEBUG_H__ */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
