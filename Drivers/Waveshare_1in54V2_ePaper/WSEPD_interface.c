/*****************************************************************************
* | File      	:   DEV_Config.c
* | Author      :   Waveshare team
* | Function    :   Hardware underlying interface
* | Info        :
*                Used to shield the underlying layers of each master
*                and enhance portability
*----------------
* |	This version:   V2.0
* | Date        :   2018-10-30
* | Info        :
# ******************************************************************************
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documnetation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to  whom the Software is
# furished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in
# all copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS OR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
# THE SOFTWARE.
#
******************************************************************************/

/*
 * E_paper_if.c
 *
 *  Created on: 23 mar. 2023
 *      Author: Gonzo
 *
 *  Waveshare e-Paper display modules interface
 */

#include "WSEPD_interface.h"
#include "EPD_1in54_V2.h"
#include "timer_if.h"

display_context_t display; //Display context

void WSEPD_GPIO_Init(void);
int WSEPD_Module_On(void);
void WSEPD_Module_Off(void);
void WSEPD_Set_Time ( uint32_t period );

void WSEPD_Init ( void )
{
  DEV_SPI_Init();
  WSEPD_GPIO_Init();
}

void WSEPD_DeInit ( void )
{
  DEV_SPI_DeInit();
  //EPD_GPIO_DeInit();
}

//TODO: create a universal interface to handle all Waveshare e-Paper screens
uint32_t WSEPD_Display_Init ( const unsigned char* image )
{
#if DISPLAY_CARE
  uint8_t refresh_times = 5;
#else
  uint8_t refresh_times = 1;
#endif /* DISPLAY_CARE */
  uint32_t delay = DISPLAY_INITDELAY + (refresh_times * DISPLAY_FULLDELAY);

#if DISPLAY_USESCHEDULER
  display.command_active = true;
#endif /* DISPLAY_USESCHEDULER */

  APP_LOG(TS_ON, VLEVEL_L, EPD_TAG "e-Paper display Init\r\n");
  WSEPD_Module_On();
  display.mode = DISPLAY_INIT;
  display.available = true; // really needed???
  display.Xcursor = 0;
  display.Ycursor = 0;
  display.FullMode_Period = UTIL_TimerDriver.ms2Tick(DISPLAY_FULLDELAY);
  display.PartialMode_Period = UTIL_TimerDriver.ms2Tick(DISPLAY_PARTIALDELAY);

  WSEPD_Set_Time ( UTIL_TimerDriver.ms2Tick(DISPLAY_INITDELAY) + display.PartialMode_Period );

  APP_LOG(TS_ON, VLEVEL_L, EPD_TAG "e-Paper display Clear...\r\n");
  EPD_1IN54_V2_Init();
  for (int i = 0; i < refresh_times; i++)
  {
	EPD_1IN54_V2_Clear();
  }

  /* you have to edit the startup_stm32fxxx.s file and set a big enough heap size */
  UWORD Imagesize = ((EPD_1IN54_V2_WIDTH % 8 == 0)? (EPD_1IN54_V2_WIDTH / 8 ): (EPD_1IN54_V2_WIDTH / 8 + 1)) * EPD_1IN54_V2_HEIGHT;
  if((display.Framebuffer = (UBYTE *)malloc(Imagesize)) == NULL)
  {
    APP_LOG(TS_ON, VLEVEL_L, EPD_TAG "Failed to create display frame buffer. Please, modify Heap size...\r\n");
    assert_param(false);
  }
  APP_LOG(TS_ON, VLEVEL_L, EPD_TAG "Created display frame buffer\r\n");
  Paint_NewImage(display.Framebuffer, EPD_1IN54_V2_WIDTH, EPD_1IN54_V2_HEIGHT, 180, WHITE);

  APP_LOG(TS_ON, VLEVEL_L, EPD_TAG "Show initial display frame\r\n");
  //Paint_SelectImage(display.Framebuffer); // really needed??? already done inside Paint_NewImage( )
  //Paint_Clear(WHITE); // really needed??? (maybe if we only draw an image smaller than 200x200)
  Paint_DrawBitMap(image);

#if DISPLAY_USESCHEDULER
  display.command_active = false;
#endif /* DISPLAY_USESCHEDULER */
  return delay;
}

uint32_t WSEPD_Display_PowerOff ( bool mode, bool buffer )
{
#if DISPLAY_USEDELAY
  uint32_t time_to_refresh = WSEPD_Display_Delay ( );

  if (time_to_refresh != 0)  return time_to_refresh;
#endif /* DISPLAY_USEDELAY */

#if DISPLAY_USESCHEDULER
  display.command_active = true;
#endif /* DISPLAY_USESCHEDULER */

  if (mode == DISPLAY_EXIT)
  {
    APP_LOG(TS_ON, VLEVEL_L, "Clear...\r\n");
    EPD_1IN54_V2_Init();
    WSEPD_Set_Time ( display.FullMode_Period );
    EPD_1IN54_V2_Clear();
#if DISPLAY_USEDELAY
    DEV_Delay_ms(DISPLAY_CLEARDELAY);
#endif /* DISPLAY_USEDELAY */
  }

  APP_LOG(TS_ON, VLEVEL_L, "Goto Sleep...\r\n");
  WSEPD_Set_Time ( DISPLAY_SLEEPDELAY );
  EPD_1IN54_V2_Sleep();

  if (mode == DISPLAY_EXIT)
  {
    // close 5V
    APP_LOG(TS_ON, VLEVEL_L, "close 5V, Module enters 0 power consumption ...\r\n");
    WSEPD_Module_Off();
    WSEPD_DeInit();
  }

  display.available = true;

  if (buffer == DISPLAY_FREEBUFFER)  WSEPD_Freebuffer ( );

#if DISPLAY_USESCHEDULER
  display.command_active = false;
#endif /* DISPLAY_USESCHEDULER */
  return 0;
}

uint32_t WSEPD_Set_Mode ( uint8_t mode )
{
#if DISPLAY_USEDELAY
  uint32_t time_to_refresh;
#endif /* DISPLAY_USEDELAY */

  //If we are already in the mode requested do nothing
  if ( ((mode == DISPLAY_PARTIALMODE) && (display.mode == DISPLAY_PARTIALMODE)) ||
	   ((mode == DISPLAY_FULLMODE) && (display.mode == DISPLAY_FULLMODE)) )  return 0;

#if DISPLAY_USEDELAY
  //If the display can't be updated return how much time we need to wait for it
  time_to_refresh = WSEPD_Display_Delay ( );
  if (time_to_refresh != 0)  return time_to_refresh;
#endif /* DISPLAY_USEDELAY */

#if DISPLAY_USESCHEDULER
  display.command_active = true;
#endif /* DISPLAY_USESCHEDULER */

  if (mode == DISPLAY_PARTIALMODE)
  {
	//The image of the previous frame must be uploaded, otherwise the first few seconds will display an exception.
    EPD_1IN54_V2_DisplayPartBaseImage(display.Framebuffer);

    //Enter partial mode
    APP_LOG(TS_ON, VLEVEL_L, "Partial refresh\r\n");
    display.mode = DISPLAY_PARTIALMODE;
    WSEPD_Set_Time ( display.PartialMode_Period );
    EPD_1IN54_V2_Init_Partial();
  }
  else if (mode == DISPLAY_FULLMODE)
  {
    if (display.mode != DISPLAY_INIT)
    {
      EPD_1IN54_V2_Init();
      WSEPD_Set_Time ( display.FullMode_Period );
      EPD_1IN54_V2_Clear();
#if DISPLAY_USEDELAY
      DEV_Delay_ms(DISPLAY_CLEARDELAY);
#endif /* DISPLAY_USEDELAY */
    }
    display.mode = DISPLAY_FULLMODE;
    WSEPD_Set_Time ( display.FullMode_Period );
    EPD_1IN54_V2_Display(display.Framebuffer);
  }

#if DISPLAY_USESCHEDULER
  display.command_active = false;
#endif /* DISPLAY_USESCHEDULER */
  return 0;
}

uint8_t WSEPD_Get_Mode ( void )
{
  return display.mode;
}

uint32_t WSEPD_Display_Update ( bool option )
{
#if DISPLAY_USESCHEDULER
	display.command_active = true;
#endif /* DISPLAY_USESCHEDULER */
#if DISPLAY_USEDELAY
  if (option == DISPLAY_CHECK)
  {
	uint32_t time_to_refresh = WSEPD_Display_Delay ( );
    //If the display can't be updated return how much time we need to wait for it
    if (time_to_refresh != 0)  return time_to_refresh;
  }
#endif /* DISPLAY_USEDELAY */

  if (display.mode != DISPLAY_PARTIALMODE) // Init or Full Display mode
  {
	WSEPD_Set_Time ( display.FullMode_Period );
	EPD_1IN54_V2_Display(display.Framebuffer);
  }
  else // Partial Display mode
  {
	WSEPD_Set_Time ( display.PartialMode_Period );
    EPD_1IN54_V2_DisplayPart(display.Framebuffer);
  }

#if DISPLAY_USESCHEDULER
  display.command_active = false;
#endif /* DISPLAY_USESCHEDULER */
  return 0;
}

uint32_t WSEPD_Partial_Line ( uint16_t Xstart, uint16_t Ystart, const char *s, uint8_t font, uint8_t size, uint16_t font_color )
{
  UWORD Xend, Yend;
  sFONT *p_font;
#if DISPLAY_USEDELAY
  uint32_t time_to_refresh = WSEPD_Display_Delay ( );

  //If the display can't be updated return how much time we need to wait for it
  if (time_to_refresh != 0)  return time_to_refresh;
#endif /* DISPLAY_USEDELAY */

  //Check that we already are in Partial mode
  if(display.mode != DISPLAY_PARTIALMODE)  assert_param(false);

  //calculate Xend and Yend and select proper font pointer
  if (font == FONT8)
  {
    Xend = Xstart + Font8.Width * size;
    Yend = Ystart + Font8.Height * size;
    p_font = &Font8;
  }
  else if (font == FONT12)
  {
    Xend = Xstart + Font12.Width * size;
	Yend = Ystart + Font12.Height * size;
	p_font = &Font12;
  }
  else if (font == FONT16)
  {
    Xend = Xstart + Font16.Width * size;
	Yend = Ystart + Font16.Height * size;
	p_font = &Font16;
  }
  else if (font == FONT20)
  {
    Xend = Xstart + Font20.Width * size;
	Yend = Ystart + Font20.Height * size;
	p_font = &Font20;
  }
  else if (font == FONT24)
  {
    Xend = Xstart + Font24.Width * size;
	Yend = Ystart + Font24.Height * size;
	p_font = &Font24;
  }
  else
  {
    APP_LOG(TS_OFF, VLEVEL_L, EPD_TAG "Wrong font selected\r\n");
    assert_param(false);
  }

#if DISPLAY_USESCHEDULER
  display.command_active = true;
#endif /* DISPLAY_USESCHEDULER */

  //Clear enough space for the new line and insert the line inside display image buffer
  if (font_color == BLACK)
  {
    Paint_ClearWindows(Xstart, Ystart, Xend, Yend, WHITE);
    Paint_DrawString_EN(Xstart, Ystart, s, p_font, WHITE, BLACK);
  }
  else
  {
	Paint_ClearWindows(Xstart, Ystart, Xend, Yend, BLACK);
    Paint_DrawString_EN(Xstart, Ystart, s, p_font, BLACK, WHITE);
  }

  WSEPD_Display_Update ( DISPLAY_NO_CHECK ); // we already checked that display is ready

#if DISPLAY_USESCHEDULER
  display.command_active = false;
#endif /* DISPLAY_USESCHEDULER */
  return 0;
}

uint32_t WSEPD_Display_Refresh ( void )
{
#if DISPLAY_USEDELAY
  uint32_t time_to_refresh = WSEPD_Display_Delay ( );

  if (time_to_refresh != 0)  return time_to_refresh;
#endif /* DISPLAY_USEDELAY */

#if DISPLAY_USESCHEDULER
  display.command_active = true;
#endif /* DISPLAY_USESCHEDULER */

  APP_LOG(TS_ON, VLEVEL_L, EPD_TAG "Display clean to avoid burn-outs\r\n");
  EPD_1IN54_V2_Init(); // really needed?
  WSEPD_Set_Time ( display.FullMode_Period );
  EPD_1IN54_V2_Clear();
#if DISPLAY_USEDELAY
  DEV_Delay_ms(DISPLAY_CLEARDELAY);
#endif /* DISPLAY_USEDELAY */

  APP_LOG(TS_ON, VLEVEL_L, EPD_TAG "Restore previous display image and activate again partial refresh\r\n");
//  EPD_1IN54_V2_Display(Framebuffer); // Really needed??? EPD_1IN54_V2_DisplayPartBaseImage( ) will send the image again
  // The image of the previous frame must be uploaded, otherwise the first few seconds will display an exception.
  WSEPD_Set_Time ( display.FullMode_Period );
  EPD_1IN54_V2_DisplayPartBaseImage(display.Framebuffer);
  WSEPD_Set_Time ( display.PartialMode_Period );
  EPD_1IN54_V2_Init_Partial();

  APP_LOG(TS_ON, VLEVEL_L, EPD_TAG "Drawing initial values on the frame buffer\r\n");
  //Select Image
  //Paint_SelectImage(display.Framebuffer); // Really needed??? image was already selected inside Paint_NewImage( ) and we never changed it

#if DISPLAY_USESCHEDULER
  display.command_active = false;
#endif /* DISPLAY_USESCHEDULER */
  return 0;
}

#if DISPLAY_USEDELAY
uint32_t WSEPD_Display_Delay ( void )
{
  uint32_t time_to_refresh, maxtime;

  if(!display.available)
  {
	uint32_t now = UTIL_TimerDriver.GetTimerValue();

	if ((display.mode == DISPLAY_FULLMODE) || (display.mode == DISPLAY_INIT))  maxtime = display.FullMode_Period;
	else                                                                       maxtime = display.PartialMode_Period;

	if (now > display.time)  time_to_refresh = display.time + 0xFFFFFFFF - now;
	else                     time_to_refresh = display.time - now;
	if (time_to_refresh > maxtime)
	{
	  time_to_refresh = 0;     // we waited too long and there was a time overflow
	  display.available = true; // so display is available
	}
	else if (time_to_refresh == 0)  display.available = true;
  }
  else  time_to_refresh = 0; // display already available we can refresh it immediately

  return time_to_refresh;
}
#endif /* DISPLAY_USEDELAY */

void WSEPD_Freebuffer ( void )
{
  free(display.Framebuffer);
  display.Framebuffer = NULL;
}

/* Private functions */

void WSEPD_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  bool GPIOA_Port_used = false;
  bool GPIOB_Port_used = false;
  bool GPIOC_Port_used = false;

  /* GPIO Ports Clock Enable */
  if      (EPD_RST_GPIO_Port == GPIOA)  GPIOA_Port_used = true;
  else if (EPD_RST_GPIO_Port == GPIOB)  GPIOB_Port_used = true;
  else if (EPD_RST_GPIO_Port == GPIOC)  GPIOC_Port_used = true;

  if      (EPD_BUSY_GPIO_Port == GPIOA)  GPIOA_Port_used = true;
  else if (EPD_BUSY_GPIO_Port == GPIOB)  GPIOB_Port_used = true;
  else if (EPD_BUSY_GPIO_Port == GPIOC)  GPIOC_Port_used = true;

  if      (EPD_DC_GPIO_Port == GPIOA)  GPIOA_Port_used = true;
  else if (EPD_DC_GPIO_Port == GPIOB)  GPIOB_Port_used = true;
  else if (EPD_DC_GPIO_Port == GPIOC)  GPIOC_Port_used = true;

  if      (EPD_CS_GPIO_Port == GPIOA)  GPIOA_Port_used = true;
  else if (EPD_CS_GPIO_Port == GPIOB)  GPIOB_Port_used = true;
  else if (EPD_CS_GPIO_Port == GPIOC)  GPIOC_Port_used = true;

  if (GPIOA_Port_used)  __HAL_RCC_GPIOA_CLK_ENABLE();
  if (GPIOB_Port_used)  __HAL_RCC_GPIOB_CLK_ENABLE();
  if (GPIOC_Port_used)  __HAL_RCC_GPIOC_CLK_ENABLE();

  /*Configure Output Level for RESET, DC and CS GPIO pins */
  HAL_GPIO_WritePin(EPD_RST_GPIO_Port, EPD_RST_Pin, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(EPD_DC_GPIO_Port, EPD_DC_Pin, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(EPD_CS_GPIO_Port, EPD_CS_Pin, GPIO_PIN_RESET);

  /*Configure Output GPIO mode */
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;

  /*Configure RESET pin*/
  GPIO_InitStruct.Pin = EPD_RST_Pin;
  HAL_GPIO_Init(EPD_RST_GPIO_Port, &GPIO_InitStruct);

  /*Configure DC pin*/
  GPIO_InitStruct.Pin = EPD_DC_Pin;
  HAL_GPIO_Init(EPD_DC_GPIO_Port, &GPIO_InitStruct);

  /*Configure CS pin*/
  GPIO_InitStruct.Pin = EPD_CS_Pin;
  HAL_GPIO_Init(EPD_CS_GPIO_Port, &GPIO_InitStruct);

  /*Configure BUSY pin */
  GPIO_InitStruct.Pin = EPD_BUSY_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(EPD_BUSY_GPIO_Port, &GPIO_InitStruct);
}

int WSEPD_Module_On(void)
{
  DEV_Digital_Write(EPD_DC_PIN, 0);
  DEV_Digital_Write(EPD_CS_PIN, 0);
  DEV_Digital_Write(EPD_RST_PIN, 1);

  return 0;
}

void WSEPD_Module_Off(void)
{
  DEV_Digital_Write(EPD_DC_PIN, 0);
  DEV_Digital_Write(EPD_CS_PIN, 0);
  //close 5V
  DEV_Digital_Write(EPD_RST_PIN, 0);
}

void WSEPD_Set_Time ( uint32_t period )
{
  display.time =  UTIL_TimerDriver.GetTimerValue() + period;
  display.available = false;
}

