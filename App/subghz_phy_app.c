/*!
 * \file      subghz_phy_app.c
 *
 * \brief     Ping-Pong implementation
 *
 * \copyright Revised BSD License, see section \ref LICENSE.
 *
 * \code
 *                ______                              _
 *               / _____)             _              | |
 *              ( (____  _____ ____ _| |_ _____  ____| |__
 *               \____ \| ___ |    (_   _) ___ |/ ___)  _ \
 *               _____) ) ____| | | || |_| ____( (___| | | |
 *              (______/|_____)_|_|_| \__)_____)\____)_| |_|
 *              (C)2013-2017 Semtech
 *
 * \endcode
 *
 * \author    Miguel Luis ( Semtech )
 *
 * \author    Gregory Cristian ( Semtech )
 */
/**
  ******************************************************************************
  *
  *          Portions COPYRIGHT 2020 STMicroelectronics
  *
  * @file    subghz_phy_app.c
  * @author  MCD Application Team
  * @brief   Application of the SubGHz_Phy Middleware
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "platform.h"
#include "stm32_timer.h"
#include "sys_app.h"
#include "subghz_phy_app.h"
#include "radio.h"
#include "stm32_seq.h"
#include "utilities_def.h"
#include "app_version.h"
#include "adc_if.h"
#include <stdlib.h>

/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* External variables ---------------------------------------------------------*/
/* USER CODE BEGIN EV */

/* USER CODE END EV */

/* Private typedef -----------------------------------------------------------*/
typedef enum
{
  RX_DONE,
  RX_TO,
  RX_ERR,
  TX_START,
  TX_DONE,
  RX_START,
  TX_TO,
} States_t;

/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

uint16_t BufferSize = BUFFER_SIZE;
uint8_t Buffer[BUFFER_SIZE];

States_t State = TX_START;

uint16_t FrameSentCnt = 0;
uint16_t FrameAckCnt = 0;

int8_t RssiValue = 0;
int8_t SnrValue = 0;

/* Led Timers objects*/
//static  UTIL_TIMER_Object_t timerLed;
static  UTIL_TIMER_Object_t timerLedTx;
static  UTIL_TIMER_Object_t timerLedRx;
static  UTIL_TIMER_Object_t timerLedError;
static  UTIL_TIMER_Object_t timerTx;

/* RF collisions avoidance */
bool isChannelFree = true;

/* Radio events function pointer */
static RadioEvents_t RadioEvents;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
/*!
 * @brief  Function executed on when led timer elapses
 * @param  LED context
 * @retval none
 */
//static void OnledEvent(void *context);
static void OnLedTxEvent(void *context);
static void OnLedRxEvent(void *context);
static void OnLedErrorEvent(void *context);
static void OnTimerTxEvent(void *context);

/*!
 * @brief Function to be executed on Radio Tx Done event
 * @param  none
 * @retval none
 */
static void OnTxDone(void);

/**
  * @brief Function to be executed on Radio Rx Done event
  * @param  payload ptr of buffer received
  * @param  size payload size
  * @param  rssi
  * @param  LoraSnr_FskCfo
  */
static void OnRxDone(uint8_t *payload, uint16_t size, int16_t rssi, int8_t LoraSnr_FskCfo);

/**
  * @brief Function executed on Radio Tx Timeout event
  * @param  none
  * @retval none
  */
static void OnTxTimeout(void);

/**
  * @brief Function executed on Radio Rx Timeout event
  * @param  none
  * @retval none
  */
static void OnRxTimeout(void);

/**
  * @brief Function executed on Radio Rx Error event
  * @param  none
  * @retval none
  */
static void OnRxError(void);

/*!
 * @brief PingPong state machine implementation
 * @param  none
 * @retval none
 */
static void Sensor_Process(void);

/* USER CODE BEGIN PFP */

static uint32_t powInt(uint32_t base, uint32_t exp);

/* USER CODE END PFP */

/* Exported functions ---------------------------------------------------------*/
void Sensor_Init(void)
{
  /* USER CODE BEGIN SubghzApp_Init_1 */

  /* USER CODE END SubghzApp_Init_1 */

  /* TX timer */
  UTIL_TIMER_Create(&timerTx,0xFFFFFFFF,UTIL_TIMER_ONESHOT,OnTimerTxEvent,NULL);
  UTIL_TIMER_SetPeriod(&timerTx,TX_PERIOD_MS);
  //UTIL_TIMER_Start(&timerTx);

  /* LED TX timer */
  UTIL_TIMER_Create(&timerLedTx,0xFFFFFFFF,UTIL_TIMER_ONESHOT,OnLedTxEvent,NULL);
  UTIL_TIMER_SetPeriod(&timerLedTx, LED_PERIOD_MS);

  /* LED ERROR timer */
  UTIL_TIMER_Create(&timerLedError,0xFFFFFFFF,UTIL_TIMER_ONESHOT,OnLedErrorEvent,NULL);
  UTIL_TIMER_SetPeriod(&timerLedError,LED_ERROR_PERIOD_MS);

  /* LED RX timer */
  UTIL_TIMER_Create(&timerLedRx,0xFFFFFFFF,UTIL_TIMER_ONESHOT,OnLedRxEvent,NULL);
  UTIL_TIMER_SetPeriod(&timerLedRx,LED_PERIOD_MS);

  /* ADC measurement */
  SYS_InitMeasurement();

  /* Radio initialization */
  RadioEvents.TxDone = OnTxDone;
  RadioEvents.RxDone = OnRxDone;
  RadioEvents.TxTimeout = OnTxTimeout;
  RadioEvents.RxTimeout = OnRxTimeout;
  RadioEvents.RxError = OnRxError;

  Radio.Init(&RadioEvents);
  srand(Radio.Random());

#if (( USE_MODEM_LORA == 1 ) && ( USE_MODEM_FSK == 0 ))
  Radio.SetTxConfig(MODEM_LORA, TX_OUTPUT_POWER, 0, LORA_BANDWIDTH,
                    LORA_SPREADING_FACTOR, LORA_CODINGRATE,
                    LORA_PREAMBLE_LENGTH, LORA_FIX_LENGTH_PAYLOAD_ON,
                    true, 0, 0, LORA_IQ_INVERSION_ON, TX_TIMEOUT_VALUE);

  Radio.SetRxConfig(MODEM_LORA, LORA_BANDWIDTH, LORA_SPREADING_FACTOR,
                    LORA_CODINGRATE, 0, LORA_PREAMBLE_LENGTH,
                    LORA_SYMBOL_TIMEOUT, LORA_FIX_LENGTH_PAYLOAD_ON,
                    0, true, 0, 0, LORA_IQ_INVERSION_ON, true);

  Radio.SetMaxPayloadLength(MODEM_LORA, BUFFER_SIZE);

#elif (( USE_MODEM_LORA == 0 ) && ( USE_MODEM_FSK == 1 ))

  Radio.SetTxConfig(MODEM_FSK, TX_OUTPUT_POWER, FSK_FDEV, 0,
                    FSK_DATARATE, 0,
                    FSK_PREAMBLE_LENGTH, FSK_FIX_LENGTH_PAYLOAD_ON,
                    true, 0, 0, 0, TX_TIMEOUT_VALUE);

  Radio.SetRxConfig(MODEM_FSK, FSK_BANDWIDTH, FSK_DATARATE,
                    0, FSK_AFC_BANDWIDTH, FSK_PREAMBLE_LENGTH,
                    0, FSK_FIX_LENGTH_PAYLOAD_ON, 0, true,
                    0, 0, false, true);

  Radio.SetMaxPayloadLength(MODEM_FSK, BUFFER_SIZE);

#else
#error "Please define a frequency band in the sys_conf.h file."
#endif /* USE_MODEM_LORA | USE_MODEM_FSK */

  Radio.SetChannel(RF_FREQUENCY);

  SYS_LED_Init(SYS_LED_BLUE);
  SYS_LED_Init(SYS_LED_GREEN);
  SYS_LED_Init(SYS_LED_RED);

  Radio.Rx(RX_TIMEOUT_VALUE);

  UTIL_SEQ_RegTask((1 << CFG_SEQ_Task_Sensor_Process), 0, Sensor_Process);

  OnTimerTxEvent(NULL);

  APP_LOG(TS_OFF, VLEVEL_L, "\r\nSensor start -> ");
  APP_LOG(TS_OFF,VLEVEL_L," ID=%02X",NODE_ID);
  APP_LOG(TS_OFF, VLEVEL_L, " , RF=%uMHz , SF=%u",RF_FREQUENCY,LORA_SPREADING_FACTOR);
  APP_LOG(TS_OFF,VLEVEL_L," , CS=%ddBm",RF_CHANNEL_FREE_RSSI_TRESHOLD);
  APP_LOG(TS_OFF, VLEVEL_L, "\r\n");

  /* USER CODE BEGIN SubghzApp_Init_2 */

  /* USER CODE END SubghzApp_Init_2 */
}

/* USER CODE BEGIN EF */

/* USER CODE END EF */

/* Private functions ---------------------------------------------------------*/
static void Sensor_Process(void)
{
  uint32_t i,backoffTime,carrierSenseTime;
  int16_t rssi = 0;
  bool isChannelFree = true;

  switch (State)
  {
    case TX_START:
	  i = 0;
#if 1  /* Byte data format */
	  Buffer[i++] = NODE_ID;
	  Buffer[i++] = (uint8_t)(GetTemperatureLevel());
	  Buffer[i++] = GetBatteryLevel();
	  Buffer[i++] = (FrameSentCnt>>8) & 0xFF;
	  Buffer[i++] = FrameSentCnt & 0xFF;
	  Buffer[i++] = (FrameAckCnt>>8) & 0xFF;
	  Buffer[i++] = FrameAckCnt & 0xFF;
#endif
#if 0  /* ASCII data format */
	  Buffer[i++] = 'J';
	  Buffer[i++] = 'o';
	  Buffer[i++] = 'h';
	  Buffer[i++] = 'n';
	  Buffer[i++] = ' ';
	  Buffer[i++] = 'B';
	  Buffer[i++] = 'r';
	  Buffer[i++] = 'o';
	  Buffer[i++] = 'w';
	  Buffer[i++] = 'n';
#endif
	  BufferSize = i;
	  /* RF collisions avoidance and TX section */
	  Radio.SetChannel(RF_FREQUENCY);
	  Radio.Rx(0);
	  HAL_Delay(Radio.GetWakeupTime());
	  for (i=0;i<RF_CHANNEL_FREE_TRIALS_MAX;i++)
	  {
	    APP_LOG(TS_ON,VLEVEL_L,"RF Channel Sensing #%u ...",i+1);
		isChannelFree = true;
		carrierSenseTime = UTIL_TIMER_GetCurrentTime();
		while( UTIL_TIMER_GetElapsedTime(carrierSenseTime) < RSSI_SENSING_TIME)
		{
		  rssi = Radio.Rssi(MODEM_LORA);
		  if (rssi > RF_CHANNEL_FREE_RSSI_TRESHOLD) { isChannelFree = false; break; }
		}
		carrierSenseTime = UTIL_TIMER_GetElapsedTime(carrierSenseTime);
		APP_LOG(TS_OFF,VLEVEL_L," CS: %ddBm , CS time: %ums\r\n",rssi,carrierSenseTime);
		if (isChannelFree)
		{
		  break; //RF collisions avoidance loop
		}
		else
		{
		  if (i<RF_CHANNEL_FREE_TRIALS_MAX-1)
		  {
		    backoffTime = CS_BACKOFF_TIME_UNIT * (1 + (rand() % powInt(2,i+1)));
		    APP_LOG(TS_ON,VLEVEL_L,"RF channel is busy, next attempt after %ums...\r\n",backoffTime);
		    HAL_Delay(backoffTime);
		  }
		}
      }
	  /* TX data over the air */
	  if (isChannelFree)
	  {
	    Radio.SetChannel(RF_FREQUENCY);
	    HAL_Delay(Radio.GetWakeupTime() + TCXO_WORKAROUND_TIME_MARGIN);
	    Radio.Send(Buffer,BufferSize);  // to be filled by attendee
	    APP_LOG(TS_ON,VLEVEL_L,"TX...\r\n");
	  }
	  else
	  {
	    UTIL_TIMER_Start(&timerLedError);
		SYS_LED_On(SYS_LED_RED);
		APP_LOG(TS_ON, VLEVEL_L, "RF channel: %uHz is BUSY\r\n\r\n",RF_FREQUENCY);
	  }
	  break;  //case

	case RX_START:
	  FrameSentCnt++;
	  Radio.SetChannel(RF_FREQUENCY);
	  Radio.Rx(RX_TIMEOUT_VALUE);
	  APP_LOG(TS_ON, VLEVEL_L, "RX...\r\n");
	  break;

	case RX_DONE:
	  Radio.Sleep();
	  UTIL_TIMER_Start(&timerLedRx);
	  SYS_LED_On(SYS_LED_GREEN);
	  APP_LOG(TS_ON,VLEVEL_L,"RX hex: ");
	  for(uint16_t i=0;i<BufferSize;i++) APP_LOG(TS_OFF,VLEVEL_L,"%02X|",Buffer[i]);
	  APP_LOG(TS_OFF,VLEVEL_L,"\r\nRSSI=%ddBm , SNR=%ddB\r\n\r\n",RssiValue,SnrValue);
	  if ((Buffer[0]=='A') && (Buffer[1]=='C') && (Buffer[2]=='K'))
	  {
	    FrameAckCnt++;
	  }
	  break;

	case RX_TO:
	  Radio.Sleep();
	  APP_LOG(TS_ON, VLEVEL_L, "RX timeout\r\n\r\n");
	  break;

	case RX_ERR:
	  Radio.Sleep();
	  UTIL_TIMER_Start(&timerLedError);
	  SYS_LED_On(SYS_LED_RED);
	  APP_LOG(TS_ON, VLEVEL_L, "RX error\r\n");
	  break;

	case TX_TO:
	  Radio.Sleep();
	  UTIL_TIMER_Start(&timerLedError);
	  SYS_LED_On(SYS_LED_RED);
	  APP_LOG(TS_ON, VLEVEL_L, "TX timeout\r\n");
	  break;

	default:
      break;
  }
}

static void OnTxDone(void)
{
  State = RX_START;
  UTIL_SEQ_SetTask((1 << CFG_SEQ_Task_Sensor_Process), CFG_SEQ_Prio_0);
}

static void OnRxDone(uint8_t *payload, uint16_t size, int16_t rssi, int8_t LoraSnr_FskCfo)
{
  BufferSize = size;
  memcpy(Buffer, payload, BufferSize);
  RssiValue = rssi;
  SnrValue = LoraSnr_FskCfo;

  State = RX_DONE;
  UTIL_SEQ_SetTask((1 << CFG_SEQ_Task_Sensor_Process), CFG_SEQ_Prio_0);
}

static void OnTxTimeout(void)
{
  State = TX_TO;
  UTIL_SEQ_SetTask((1 << CFG_SEQ_Task_Sensor_Process), CFG_SEQ_Prio_0);
}

static void OnRxTimeout(void)
{
  State = RX_TO;
  UTIL_SEQ_SetTask((1 << CFG_SEQ_Task_Sensor_Process), CFG_SEQ_Prio_0);
}

static void OnRxError(void)
{
  State = RX_ERR;
  UTIL_SEQ_SetTask((1 << CFG_SEQ_Task_Sensor_Process), CFG_SEQ_Prio_0);
}

static void OnLedTxEvent(void *context)
{
  SYS_LED_Off(SYS_LED_BLUE) ;
}

static void OnLedRxEvent(void *context)
{
  SYS_LED_Off(SYS_LED_GREEN) ;
}

static void OnLedErrorEvent(void *context)
{
  SYS_LED_Off(SYS_LED_RED) ;
}

static void OnTimerTxEvent(void *context)
{
  State = TX_START;
  UTIL_TIMER_Start(&timerTx);
  UTIL_TIMER_Start(&timerLedTx);
  SYS_LED_On(SYS_LED_BLUE);
  UTIL_SEQ_SetTask((1 << CFG_SEQ_Task_Sensor_Process), CFG_SEQ_Prio_0);
}

static uint32_t powInt(uint32_t base, uint32_t exp)
{
    uint32_t result = 1;
    while(exp) { result *= base; exp--; }
    return result;
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
