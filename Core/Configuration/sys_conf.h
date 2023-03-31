/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    sys_conf.h
  * @author  MCD Application Team
  * @brief   Applicative configuration, e.g. : debug, trace, low power, sensors
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
#ifndef __SYS_CONF_H__
#define __SYS_CONF_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/

/**
  * @brief  Verbose level for all trace logs
  */
#define VERBOSE_LEVEL               VLEVEL_M

/**
  * @brief Enable APP trace logs
  */
#define APP_LOG_ENABLED             1

/**
  * @brief Enable MCU Debugger pins (dbg serial wires, dbg spi, etc)
  * @note  1:ON it enables the debbugger, 0:OFF the debugger is OFF (lower consumption)
  */
//TODO: remove this define and move it to DEBUG compilation config
#define DEBUGGER_ENABLED            1

/**
  * @brief Enable four wires usable as probes (two of them PROBE1 and PROBE2 used by the MW)
  */
#define PROBE_PINS_ENABLED          0

/**
  * @brief Disable Low Power mode
  * @note  0: LowPowerMode enabled. MCU enters stop2 mode, 1: LowPowerMode disabled. MCU enters sleep mode only
  */
#define LOW_POWER_DISABLE           0

/* USER CODE BEGIN EC */
#define USE_E_PAPER_SCREEN          1 // Waveshare 1.54 inch E-Paper Screen
//#define USE_LPUART1                 1 // Used to communicate with B-L072Z-LRWAN1 board (Murata or Ebyte radio)
//#define USE_USART1                  1 // Used to communicate with RAK3172 board (and also STM32WLE5 board using another pins)
/**
  * @brief Enable or disable LOGs using COM port
  * @note  If enabled SEGGER RTT Debugging will be disabled, if disabled we can still LOG using SEGGER RTT
  */
#define TRACE_ENABLED               1 // Enable LOG in COM port

#if DEBUGGER_ENABLED
 #if !(TRACE_ENABLED)
#define SEGGER_RTT                    // Enable SEGGER RTT Debugging LOG through SWD
 #endif /* !(TRACE_ENABLED) */
//#define DEBUG_PINS
#endif /* DEBUGGER_ENABLED */

#define MW_LOG_ENABLED                // Enable Middleware LOG
#define RTIF_DEBUG                    // Enable RTC Interface LOG
/* USER CODE END EC */

/* External variables --------------------------------------------------------*/
/* USER CODE BEGIN EV */

/* USER CODE END EV */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

#ifdef __cplusplus
}
#endif

#endif /* __SYS_CONF_H__ */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
