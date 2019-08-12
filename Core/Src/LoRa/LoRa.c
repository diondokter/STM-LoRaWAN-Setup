/*
 * LoRa.c
 *
 *  Created on: Aug 12, 2019
 *      Author: diond
 */

#include "LoRa/LoRa.h"
#include "lora.h"

/*!
 * Defines the application data transmission duty cycle. 5s, value in [ms].
 */
#define APP_TX_DUTYCYCLE                            10000
/*!
 * LoRaWAN Adaptive Data Rate
 * @note Please note that when ADR is enabled the end-device should be static
 */
#define LORAWAN_ADR_STATE LORAWAN_ADR_ON
/*!
 * LoRaWAN Default data Rate Data Rate
 * @note Please note that LORAWAN_DEFAULT_DATA_RATE is used only when ADR is disabled
 */
#define LORAWAN_DEFAULT_DATA_RATE DR_0
/*!
 * LoRaWAN application port
 * @note do not use 224. It is reserved for certification
 */
#define LORAWAN_APP_PORT                            2
/*!
 * LoRaWAN default endNode class port
 */
#define LORAWAN_DEFAULT_CLASS                       CLASS_A
/*!
 * LoRaWAN default confirm state
 */
#define LORAWAN_DEFAULT_CONFIRM_MSG_STATE           LORAWAN_UNCONFIRMED_MSG
/*!
 * User application data buffer size
 */
#define LORAWAN_APP_DATA_BUFF_SIZE                           64
/*!
 * User application data
 */
static uint8_t AppDataBuff[LORAWAN_APP_DATA_BUFF_SIZE];

/*!
 * User application data structure
 */
lora_AppData_t AppData={ AppDataBuff,  0 ,0 };

/* call back when LoRa endNode has received a frame*/
static void LORA_RxData( lora_AppData_t *AppData);
/* call back when LoRa endNode has just joined*/
static void LORA_HasJoined( void );
/* call back when LoRa endNode has just switch the class*/
static void LORA_ConfirmClass ( DeviceClass_t Class );
/* call back when server needs endNode to send a frame*/
static void LORA_TxNeeded ( void );
/* tx timer callback function*/
static void LoraMacProcessNotify( void );

static uint8_t HW_GetBatteryLevel() { return 255; }
static uint16_t HW_GetTemperatureLevel() { return 0; }

/*!
 *  \brief Unique Devices IDs register set ( STM32L1xxx )
 */
#define         ID1                                 ( 0x1FF80050 )
#define         ID2                                 ( 0x1FF80054 )
#define         ID3                                 ( 0x1FF80064 )

/**
  * @brief This function return a random seed
  * @note based on the device unique ID
  * @param None
  * @retval see
  */
uint32_t HW_GetRandomSeed( void )
{
  return ( ( *( uint32_t* )ID1 ) ^ ( *( uint32_t* )ID2 ) ^ ( *( uint32_t* )ID3 ) );
}

/**
  * @brief This function return a unique ID
  * @param unique ID
  * @retval none
  */
void HW_GetUniqueId( uint8_t *id )
{
    id[7] = ( ( *( uint32_t* )ID1 )+ ( *( uint32_t* )ID3 ) ) >> 24;
    id[6] = ( ( *( uint32_t* )ID1 )+ ( *( uint32_t* )ID3 ) ) >> 16;
    id[5] = ( ( *( uint32_t* )ID1 )+ ( *( uint32_t* )ID3 ) ) >> 8;
    id[4] = ( ( *( uint32_t* )ID1 )+ ( *( uint32_t* )ID3 ) );
    id[3] = ( ( *( uint32_t* )ID2 ) ) >> 24;
    id[2] = ( ( *( uint32_t* )ID2 ) ) >> 16;
    id[1] = ( ( *( uint32_t* )ID2 ) ) >> 8;
    id[0] = ( ( *( uint32_t* )ID2 ) );
}

static LoRaMainCallback_t LoRaMainCallbacks = { HW_GetBatteryLevel,
                                                HW_GetTemperatureLevel,
                                                HW_GetUniqueId,
                                                HW_GetRandomSeed,
                                                LORA_RxData,
                                                LORA_HasJoined,
                                                LORA_ConfirmClass,
                                                LORA_TxNeeded,
                                                LoraMacProcessNotify};

static  LoRaParam_t LoRaParamInit= {LORAWAN_ADR_STATE,
                                    LORAWAN_DEFAULT_DATA_RATE,
                                    LORAWAN_PUBLIC_NETWORK};

LoraFlagStatus LoraMacProcessRequest = LORA_RESET;

void LoRaTaskRun(void *argument)
{
	HW_SPI_Init();
	HW_RTC_Init();
	Radio.IoInit();
	uint32_t random = Radio.Random();
	LORA_Init(&LoRaMainCallbacks, &LoRaParamInit);

	LORA_Join();

	while (1)
	{
		if (LoraMacProcessRequest==LORA_SET)
		{
		  /*reset notification flag*/
		  LoraMacProcessRequest=LORA_RESET;
		  LoRaMacProcess( );
		}

		//osDelay(1000);
	}
}

void LoraMacProcessNotify(void)
{
  LoraMacProcessRequest = LORA_SET;
}

static void LORA_HasJoined( void )
{
  LORA_RequestClass( LORAWAN_DEFAULT_CLASS );
}


static void LORA_RxData( lora_AppData_t *AppData )
{

}

static void LORA_ConfirmClass ( DeviceClass_t Class )
{

}

static void LORA_TxNeeded ( void )
{
  AppData.BuffSize = 0;
  AppData.Port = LORAWAN_APP_PORT;

  LORA_send(&AppData, LORAWAN_UNCONFIRMED_MSG);
}
