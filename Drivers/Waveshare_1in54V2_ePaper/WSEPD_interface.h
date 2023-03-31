/*
 * E_paper_if.h
 *
 *  Created on: 23 mar. 2023
 *      Author: Gonzo
 */

#ifndef E_PAPER_E_PAPER_IF_H_
#define E_PAPER_E_PAPER_IF_H_

#include "GUI_Paint.h" // move to E_paper_if.c
#include "main.h"
#include <stdlib.h> // malloc() free()
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>


#define DISPLAY_SCHEDULER_ENABLED

#define FONT8					1
#define FONT12					2
#define FONT16					3
#define FONT20					4
#define FONT24					5

#define DISPLAY_NO_CHECK		0
#define DISPLAY_CHECK			1

#define DISPLAY_SLEEP			0
#define DISPLAY_EXIT			1
#define DISPLAY_FREEBUFFER		0
#define DISPLAY_KEEPBUFFER		1

#define DISPLAY_SUBTASK			0
#define DISPLAY_TASK			1

#define DISPLAY_INIT			0
#define DISPLAY_FULLMODE		1
#define DISPLAY_PARTIALMODE		2
#define DISPLAY_KEEPMODE		3

#define DISPLAY_FULLDELAY			2000 // 2 seconds for full display update
#define DISPLAY_PARTIALDELAY		500  // 0.5 seconds for partial display update
#define DISPLAY_INITDELAY			300
#define DISPLAY_INITPARTIALDELAY	200
#define DISPLAY_CLEARDELAY			2000 // 2 seconds of delay after clear display
#define DISPLAY_SLEEPDELAY			100  // 0.1 seconds of delay after triggering reset pin

#define DISPLAY_CARE			0    // If not 0 we will trigger several display clears at display initialization
                                     // to avoid burns in the display

typedef struct
{
  uint8_t mode; // full_display update or partial mode
  //We keep last position we painted
  uint16_t Xcursor;
  uint16_t Ycursor;
  uint8_t *Framebuffer; // memory to paint on before updating the display
  uint32_t time; // RTC time where the display will be available for a new refresh
  uint16_t FullMode_Period;
  uint16_t PartialMode_Period;
  bool available;
#ifdef DISPLAY_SCHEDULER_ENABLED
  bool command_active;
#endif /* DISPLAY_SCHEDULER_ENABLED */
}display_context_t;

void WSEPD_Init ( void );
void WSEPD_DeInit ( void );
uint32_t WSEPD_Set_Mode ( uint8_t mode );
uint8_t WSEPD_Get_Mode ( void );
uint32_t WSEPD_Display_Init ( const unsigned char* image );
uint32_t WSEPD_Display_PowerOff ( bool mode, bool buffer );
uint32_t WSEPD_Display_Update ( bool option );
uint32_t WSEPD_Display_Refresh ( void );
uint32_t WSEPD_Partial_Line ( uint16_t Xstart, uint16_t Ystart, const char *s, uint8_t font, uint8_t size, uint16_t font_color );
uint32_t WSEPD_Display_Delay ( void );
void WSEPD_Freebuffer ( void );

#endif /* E_PAPER_E_PAPER_IF_H_ */
