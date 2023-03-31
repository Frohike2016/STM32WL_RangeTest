/*
 * app_screen.h
 *
 *  Created on: 23 mar. 2023
 *      Author: Gonzo
 */

#ifndef E_PAPER_APP_SCREEN_H_
#define E_PAPER_APP_SCREEN_H_

#include "WSEPD_interface.h"
#include "EPD_1in54_V2.h"
#include "stm32_tiny_vsnprintf.h"
#include "imagedata.h"
#include <stdint.h>

#define SCREEN_SCHEDULER_ENABLED

#define MODULE_INFO_HIGH	46
#define MODULE_INFO_LENGTH	64
#define MODULE_INFO_X_START	118
#define INFO_LINE_SPACE		17

#define MODULE1_Y_START		30
#define MODULE2_Y_START		88
#define MODULE3_Y_START		149

#define RANGETEST_OFF		0
#define RANGETEST_SCANNING  1
#define RANGETEST_ON		2
#define STATE_OFF_X_START	72
#define STATE_SCAN_X_START	73
#define STATE_ON_X_START	78
#define STATE_Y_START		5
#define STATEAREA_X_START	70
#define STATEAREA_Y_START	0
#define STATEAREA_X_END		108
#define STATEAREA_Y_END		22

#define STM32WL55JC			0
#define MURATA_ABZ			1
#define E22_900M22S			2

#ifdef SCREEN_SCHEDULER_ENABLED
typedef enum
{
  S_NONE,
  S_INIT,
  S_UPDATE,
  S_APPSTATE,
  S_EXIT,
} Screen_command_t;

typedef struct
{
  const unsigned char* image;
  uint8_t module;
  uint8_t state;
  uint8_t mode;
} Screen_comm_param_t;

typedef struct
{
  Screen_command_t command;
  Screen_comm_param_t parameter;
  uint8_t step;
} Screen_comm_ctx_t;
#endif /* SCREEN_SCHEDULER_ENABLED */

typedef struct
{
  uint16_t last_payload_num;
  uint8_t rssi;
  uint8_t snr[3]; // we keep SNR from the last 3 messages received
} Screen_module_data_t;

typedef struct
{
  bool Initialized;
#ifdef SCREEN_SCHEDULER_ENABLED
  Screen_comm_ctx_t running;
  Screen_comm_ctx_t next;
#endif /* SCREEN_SCHEDULER_ENABLED */
  bool screen_off_flag;
  bool screen_refresh_flag;
  Screen_module_data_t info[3];
} Screen_context_t;

uint32_t Screen_Init ( const unsigned char* image );
uint32_t Screen_Update_Info ( uint8_t module );
uint32_t Screen_Set_AppState ( uint8_t state, uint8_t mode );
uint32_t Screen_Force_Refresh ( void );
uint32_t Screen_PowerOff ( bool mode );

#endif /* E_PAPER_APP_SCREEN_H_ */
