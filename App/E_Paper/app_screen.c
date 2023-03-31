/*
 * app_screen.c
 *
 *  Created on: 23 mar. 2023
 *      Author: Gonzo
 */

#include "app_screen.h"
#include "sys_conf.h"


Screen_context_t screen = {.Initialized = false};


static void tiny_printf(char *buf, uint32_t maxsize, const char *strFormat, ...);

uint32_t Screen_Init ( const unsigned char* image )
{
  uint32_t delay;

  if (!screen.Initialized)
  {
    screen.Initialized = true;

#ifdef DEBUGGER_ENABLED
    screen.info[STM32WL55JC].last_payload_num = 1;
    screen.info[STM32WL55JC].rssi = 175;
    screen.info[STM32WL55JC].snr[0] = 42;
    screen.info[STM32WL55JC].snr[1] = 43;
    screen.info[STM32WL55JC].snr[2] = 44;

    screen.info[MURATA_ABZ].last_payload_num = 1;
    screen.info[MURATA_ABZ].rssi = 169;
    screen.info[MURATA_ABZ].snr[0] = 39;
    screen.info[MURATA_ABZ].snr[1] = 40;
    screen.info[MURATA_ABZ].snr[2] = 41;

    screen.info[E22_900M22S].last_payload_num = 1;
    screen.info[E22_900M22S].rssi = 166;
    screen.info[E22_900M22S].snr[0] = 37;
    screen.info[E22_900M22S].snr[1] = 38;
    screen.info[E22_900M22S].snr[2] = 39;
#endif /* DEBUGGER_ENABLED */

	delay = WSEPD_Display_Init ( gImage_Frame );
  }

  return delay;
}

uint32_t Screen_Update_Info ( uint8_t module )
{
  uint8_t Xstart = MODULE_INFO_X_START;
  uint8_t Xend = MODULE_INFO_X_START + MODULE_INFO_LENGTH;
  uint8_t Ystart, Yend;
  uint16_t snr_median = 0;
  char line[9];

  //calculate Ystart and Yend depending on module selected
  if      (module == STM32WL55JC)  Ystart = MODULE1_Y_START;
  else if (module == MURATA_ABZ)   Ystart = MODULE2_Y_START;
  else if (module == E22_900M22S)  Ystart = MODULE3_Y_START;
  else  APP_LOG(TS_OFF, VLEVEL_L, EPD_TAG "Wrong module selected\r\n");
  Yend = Ystart + MODULE_INFO_HIGH;

  //Clear enough space in the screen buffer to fit new statistics info
  Paint_ClearWindows(Xstart, Ystart, Xend, Yend, WHITE);

  //Print last packet received info inside screen buffer
  tiny_printf(line, 8, " P%04d", screen.info[module].last_payload_num);
  Paint_DrawString_EN(Xstart, Ystart, line, &Font16, WHITE, BLACK);

  //Move to next line Xstart point
  Ystart = Ystart + INFO_LINE_SPACE;
  //Print last packet RSSI info inside screen buffer
  tiny_printf(line, 8, "-%ddBm", screen.info[module].rssi);
  Paint_DrawString_EN(Xstart, Ystart, line, &Font16, WHITE, BLACK);

  //Move to next line Xstart point
  Ystart = Ystart + INFO_LINE_SPACE;
  //Print last 3 packets SNR median info inside screen buffer
  if ((screen.info[module].snr[0] != 0) && (screen.info[module].snr[1] != 0) && (screen.info[module].snr[2] != 0))
  {
    snr_median = screen.info[module].snr[0] + screen.info[module].snr[1] + screen.info[module].snr[2];
    snr_median = snr_median / 3;
  }

  tiny_printf(line, 8, " %ddBm", snr_median);
  Paint_DrawString_EN(Xstart, Ystart, line, &Font16, WHITE, BLACK);
  WSEPD_Display_Update ( DISPLAY_NO_CHECK );

#ifdef DEBUGGER_ENABLED
  screen.info[module].last_payload_num += 1;
  screen.info[module].rssi += 1;
  screen.info[module].snr[0] += 1;
  screen.info[module].snr[1] += 1;
  screen.info[module].snr[2] += 1;
#endif /* DEBUGGER_ENABLED */

  return DISPLAY_PARTIALDELAY;
}

uint32_t Screen_Set_AppState ( uint8_t state, uint8_t mode )
{
  uint16_t delay;
  char line[4];

  if ( state == RANGETEST_OFF)
  {
	//Clear enough space in the screen buffer to fit new RangeTest state
	Paint_ClearWindows(STATEAREA_X_START, STATEAREA_Y_START, STATEAREA_X_END, STATEAREA_Y_END, WHITE);
	//Paint_DrawRectangle(STATEAREA_X_START, STATEAREA_Y_START, STATEAREA_X_END, STATEAREA_Y_END, WHITE, 1, true);
	line[0] = 'O';
	line[1] = 'F';
	line[2] = 'F';
	line[3] = '\0';
    Paint_DrawString_EN(STATE_OFF_X_START, STATE_Y_START, line, &Font16, WHITE, BLACK);
  }
  else if ( state == RANGETEST_SCANNING)
  {
	//Clear enough space in the screen buffer to fit new RangeTest state
	Paint_ClearWindows(STATEAREA_X_START, STATEAREA_Y_START, STATEAREA_X_END, STATEAREA_Y_END, WHITE);
	//Paint_DrawRectangle(STATEAREA_X_START, STATEAREA_Y_START, STATEAREA_X_END, STATEAREA_Y_END, WHITE, 1, true);
    line[0] = '.';
	line[1] = '.';
	line[2] = '.';
	line[3] = '\0';
    Paint_DrawString_EN(STATE_SCAN_X_START, STATE_Y_START, line, &Font16, WHITE, BLACK);
  }
  else if ( state == RANGETEST_ON)
  {
	Paint_DrawRectangle(STATEAREA_X_START, STATEAREA_Y_START, STATEAREA_X_END, STATEAREA_Y_END, BLACK, 1, true);
    line[0] = 'O';
	line[1] = 'N';
	line[2] = '\0';
    Paint_DrawString_EN(STATE_ON_X_START, STATE_Y_START, line, &Font16, BLACK, WHITE);
  }

  if (WSEPD_Get_Mode ( ) != DISPLAY_INIT)
  {
    WSEPD_Display_Update ( DISPLAY_NO_CHECK );
    //We need to trigger EPD_1IN54_V2_DisplayPart
    delay = DISPLAY_PARTIALDELAY;
  }
  else
  {
    WSEPD_Set_Mode ( DISPLAY_PARTIALMODE );
    //We need to trigger EPD_1IN54_V2_DisplayPartBaseImage and EPD_1IN54_V2_Init_Partial
    delay = DISPLAY_FULLDELAY + DISPLAY_INITPARTIALDELAY;
  }

  return delay;
}

uint32_t Screen_Force_Refresh ( void )
{
  uint32_t delay;

  //We need to trigger EPD_1IN54_V2_Init, EPD_1IN54_V2_Clear, EPD_1IN54_V2_DisplayPartBaseImage and EPD_1IN54_V2_Init_Partial
  delay = DISPLAY_INITDELAY + DISPLAY_CLEARDELAY + DISPLAY_FULLDELAY + DISPLAY_INITPARTIALDELAY;
  WSEPD_Display_Refresh ( );

  return delay;
}

uint32_t Screen_PowerOff ( bool mode )
{
  uint32_t delay;

  //We need to trigger EPD_1IN54_V2_Init, EPD_1IN54_V2_Clear and WSEPD_Module_Off
  if (mode == DISPLAY_EXIT)  delay = DISPLAY_INITDELAY + DISPLAY_CLEARDELAY + DISPLAY_SLEEPDELAY;
  //We only trigger WSEPD_Module_Off and keep image in the screen
  else                       delay = DISPLAY_SLEEPDELAY;
  WSEPD_Display_PowerOff ( mode, DISPLAY_KEEPBUFFER );

  return delay;
}

/* Private functions */
static void tiny_printf(char *buf, uint32_t maxsize, const char *strFormat, ...)
{
  /* USER CODE BEGIN tiny_snprintf_like_1 */

  /* USER CODE END tiny_snprintf_like_1 */
  va_list vaArgs;
  va_start(vaArgs, strFormat);
  tiny_vsnprintf_like(buf, maxsize, strFormat, vaArgs);
  va_end(vaArgs);
  /* USER CODE BEGIN tiny_snprintf_like_2 */

  /* USER CODE END tiny_snprintf_like_2 */
}
