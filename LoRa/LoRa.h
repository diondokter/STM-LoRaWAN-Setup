#ifndef LORA_LORA_H_
#define LORA_LORA_H_

#include "LoRaWAN/Core/lora.h"

/**
 * Initializes the LoRaWAN stack. Includes hardware initialization
 * \param LORA_HasJoined Callback that will be called when the device has joined the network
 * \param LORA_RxData Callback that will be called when data has been received
 * \param LORA_ConfirmClass Callback that will be called when the class of the device has changed.
 */
void LoRa_initialize(int use_ADR, int initial_datarate, int is_public_network, void (*LORA_HasJoined)(void), void (*LORA_RxData )(lora_AppData_t *AppData), void (*LORA_ConfirmClass) (DeviceClass_t Class ));

void LoRa_joinNetwork();
void LoRa_joinNetworkBlocking();
int LoRa_isJoined();

void LoRa_sendData(uint8_t* data, uint8_t data_length, uint8_t port, LoraConfirm_t confirmation);

void LoRa_changeClass(DeviceClass_t class);

void LoRa_setDatarate(int datarate);

// Must be called in infinite loop
void LoRa_process();

#endif /* LORA_LORA_H_ */
