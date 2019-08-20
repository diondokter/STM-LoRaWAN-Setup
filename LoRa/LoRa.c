#include "LoRa.h"
#include "LoRaWAN/Core/lora.h"
#include "util_console.h"
#include "hw.h"

/* call back when server needs endNode to send a frame*/
static void LORA_TxNeeded ( void );
/* tx timer callback function*/
static void LoraMacProcessNotify( void );


static LoraFlagStatus LoraMacProcessRequest = LORA_RESET;
static LoRaMainCallback_t LoRaMainCallbacks = { 0 };
static LoRaParam_t LoRaParamInit = { 0 };
static void LoRa_HasJoined();

static void (*join_callback)(void) = NULL;

void LoRa_initialize(int use_ADR, int initial_datarate, int is_public_network, void (*LORA_HasJoined)(void), void (*LORA_RxData )(lora_AppData_t *AppData), void (*LORA_ConfirmClass) (DeviceClass_t Class))
{
	HW_SPI_Init();
	HW_RTC_Init();
	Radio.IoInit();

	LoRaMainCallbacks.BoardGetBatteryLevel = HW_GetBatteryLevel;
	LoRaMainCallbacks.BoardGetTemperatureLevel = HW_GetTemperatureLevel;
	LoRaMainCallbacks.BoardGetUniqueId = HW_GetUniqueId;
	LoRaMainCallbacks.BoardGetRandomSeed = HW_GetRandomSeed;
	LoRaMainCallbacks.LORA_RxData = LORA_RxData;
	LoRaMainCallbacks.LORA_HasJoined = LoRa_HasJoined;
	LoRaMainCallbacks.LORA_ConfirmClass = LORA_ConfirmClass;
	LoRaMainCallbacks.LORA_TxNeeded = LORA_TxNeeded;
	LoRaMainCallbacks.MacProcessNotify = LoraMacProcessNotify;

	LoRaParamInit.AdrEnable = use_ADR;
	LoRaParamInit.TxDatarate = initial_datarate;
	LoRaParamInit.EnablePublicNetwork = is_public_network;

	join_callback = LORA_HasJoined;

	LORA_Init(&LoRaMainCallbacks, &LoRaParamInit);
}

void LoRa_joinNetwork()
{
	LORA_Join();
}

void LoRa_joinNetworkBlocking()
{
	LoRa_joinNetwork();

	uint32_t last_time = SysTimeGetMcuTime().Seconds;

	while(!LoRa_isJoined())
	{
		LoRa_process();

		SysTime_t time = SysTimeGetMcuTime();

		// In the worst case we can send a join request every ~3 minutes
		if (time.Seconds - last_time > 180)
		{
			last_time = time.Seconds;
			LoRa_joinNetwork();
		}

	}
}

int LoRa_isJoined()
{
	return LORA_JoinStatus();
}

int LoRa_canSend()
{
	return !LoRaMacIsBusy();
}

void LoRa_sendDataBlocking(uint8_t* data, uint8_t data_length, uint8_t port, LoraConfirm_t confirmation)
{
	while(!LoRa_canSend() || !LoRa_sendData(data, data_length, port, confirmation))
	{
		LoRa_process();
	}
}

int LoRa_sendData(uint8_t* data, uint8_t data_length, uint8_t port, LoraConfirm_t confirmation)
{
	lora_AppData_t appdata = { data, data_length, port };
	return !LORA_send(&appdata, confirmation);
}

void LoRa_changeClass(DeviceClass_t class)
{
	LORA_RequestClass(class);
}

void LoRa_process()
{
	if (LoraMacProcessRequest)
	{
		LoRaMacProcess();
	}
}

static void LoraMacProcessNotify(void)
{
  LoraMacProcessRequest = LORA_SET;
}

static void LORA_TxNeeded ( void )
{
	lora_AppData_t appdata = { NULL, 0, 1 };

	LORA_send(&appdata, LORAWAN_UNCONFIRMED_MSG);
}

static void LoRa_HasJoined()
{
	LoRa_changeClass(CLASS_A);
	join_callback();
}
