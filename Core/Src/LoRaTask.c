#include "LoRa.h"

void hasJoined()
{

}

void receivedData(lora_AppData_t *AppData)
{

}

void confirmedClass(DeviceClass_t Class)
{

}

void LoRaTaskRun(void *argument)
{
	LoRa_initialize(1, DR_3, 1, &hasJoined, &receivedData, &confirmedClass);

	LoRa_joinNetworkBlocking();

	while(1)
	{
		static uint32_t last_time;
		SysTime_t time = SysTimeGetMcuTime();

		if (time.Seconds % 30 == 0 && time.Seconds != last_time)
		{
			last_time = time.Seconds;

			LoRa_sendData((uint8_t*)&time.Seconds, 4, 1, 0);
			LoRa_sendData((uint8_t*)&time.Seconds, 4, 2, 0);
		}

		LoRa_process();
	}
}

