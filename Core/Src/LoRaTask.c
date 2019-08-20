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
	// Initialize the library
	LoRa_initialize(1, DR_0, 1, &hasJoined, &receivedData, &confirmedClass);

	// Join the network
	LoRa_joinNetworkBlocking();

	while(1)
	{
		static uint32_t last_time;
		SysTime_t time = SysTimeGetMcuTime();

		// We only want to send something every 30 seconds
		if (time.Seconds % 30 == 0 && time.Seconds != last_time)
		{
			last_time = time.Seconds;

			// Send the data (which is just our time)
			LoRa_sendDataBlocking((uint8_t*)&time.Seconds, 4, 1, 0);
		}

		// We need to keep calling this very often, so this is just put here in the loop
		LoRa_process();
	}
}

