/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    radio_conf.h
  * @author  MCD Application Team
  * @brief   Header of Radio configuration
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
#ifndef __RADIO_CONF_H__
#define __RADIO_CONF_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "platform.h"
#include "subghz.h"
#include "stm32_mem.h"       /* RADIO_MEMSET8 def in this file */
#include "mw_log_conf.h"     /* mw trace conf */
#include "radio_board_if.h"  /* low layer api (bsp) */
#include "utilities_def.h"  /* low layer api (bsp) */
#include "sys_debug.h"
/* USER CODE BEGIN include */

/* USER CODE END include */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported defines ----------------------------------------------------------*/
#if defined(USE_BSP_DRIVER)
/* code generated by STM32CubeMX does not support BSP */
/* In order to use BSP driver, add the correspondent files in the IDE workspace */
/* and define USE_BSP_DRIVER in the preprocessor definitions  or in platform.h */

#else
/* USER CODE BEGIN Exported PinMapping */
#warning user to provide its board definitions pins
/* USER CODE END Exported PinMapping */

/* Indicates the type of switch between the ones proposed by CONFIG Constants */
#define RBI_CONF_RFO                RBI_CONF_RFO_LP_HP

/* Indicates whether or not TCXO is supported by the board */
#define IS_TCXO_SUPPORTED           1U

/**
  * @brief Radio maximum wakeup time (in ms)
  * @note override the default configuration of radio_driver.c
  */
#define RF_WAKEUP_TIME              10UL // TODO: This can be lowered to 3ms but needs to be tested

/* Indicates whether or not DCDC is supported by the board */
#define IS_DCDC_SUPPORTED           1U

/**
  * @brief DCDC is enabled
  * @remark this define is only used if the DCDC is present on the board
  * @note override the default configuration of radio_driver.c
  */
#define DCDC_ENABLE                 1UL

/* USER CODE BEGIN Exported Parameters_2 */

/* USER CODE END Exported Parameters_2 */
#endif  /* USE_BSP_DRIVER  */

/**
  * @brief Set RX pin to high or low level
  */
#define DBG_GPIO_RADIO_RX(set_rst) /*PROBE_GPIO_##set_rst##_LINE(PROBE_LINE1_PORT, PROBE_LINE1_PIN);*/

/**
  * @brief Set TX pin to high or low level
  */
#define DBG_GPIO_RADIO_TX(set_rst) /*PROBE_GPIO_##set_rst##_LINE(PROBE_LINE2_PORT, PROBE_LINE2_PIN);*/

/**
  * @brief drive value used anytime radio is NOT in TX low power mode
  * @note override the default configuration of radio_driver.c
  */
#define SMPS_DRIVE_SETTING_DEFAULT  SMPS_DRV_40

/**
  * @brief drive value used anytime radio is in TX low power mode
  *        TX low power mode is the worst case because the PA sinks from SMPS
  *        while in high power mode, current is sunk directly from the battery
  * @note override the default configuration of radio_driver.c
  */
#define SMPS_DRIVE_SETTING_MAX      SMPS_DRV_60

/**
  * @brief Provides the frequency of the chip running on the radio and the frequency step
  * @remark These defines are used for computing the frequency divider to set the RF frequency
  * @note override the default configuration of radio_driver.c
  */
#define XTAL_FREQ                   ( 32000000UL )

/**
  * @brief in XO mode, set internal capacitor (from 0x00 to 0x2F starting 11.2pF with 0.47pF steps)
  * @note override the default configuration of radio_driver.c
  */
#define XTAL_DEFAULT_CAP_VALUE      ( 0x20UL )

/**
  * @brief voltage of vdd tcxo.
  * @note override the default configuration of radio_driver.c
  */
#define TCXO_CTRL_VOLTAGE           TCXO_CTRL_1_7V

/* USER CODE BEGIN EC */
/**
  * @brief Configures RADIO trace logs
  */
//#define RFW_MW_LOG_ENABLE
/* USER CODE END EC */

/* External variables --------------------------------------------------------*/
/* USER CODE BEGIN EV */

/* USER CODE END EV */

/* Exported macros -----------------------------------------------------------*/
#ifndef CRITICAL_SECTION_BEGIN
/**
  * @brief macro used to enter the critical section
  */
#define CRITICAL_SECTION_BEGIN( )      UTILS_ENTER_CRITICAL_SECTION( )
#endif /* !CRITICAL_SECTION_BEGIN */
#ifndef CRITICAL_SECTION_END
/**
  * @brief macro used to exit the critical section
  */
#define CRITICAL_SECTION_END( )        UTILS_EXIT_CRITICAL_SECTION( )
#endif /* !CRITICAL_SECTION_END */

/* Function mapping */
/**
  * @brief SUBGHZ interface init to radio Middleware
  */
#define RADIO_INIT                              MX_SUBGHZ_Init

/**
  * @brief Delay interface to radio Middleware
  */
#define RADIO_DELAY_MS                          HAL_Delay

/**
  * @brief Memset utilities interface to radio Middleware
  */
#define RADIO_MEMSET8( dest, value, size )      UTIL_MEM_set_8( dest, value, size )

/**
  * @brief Memcpy utilities interface to radio Middleware
  */
#define RADIO_MEMCPY8( dest, src, size )        UTIL_MEM_cpy_8( dest, src, size )

/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

#ifdef __cplusplus
}
#endif

#endif /* __RADIO_CONF_H__*/

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
