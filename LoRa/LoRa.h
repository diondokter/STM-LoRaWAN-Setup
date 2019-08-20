#ifndef LORA_LORA_H_
#define LORA_LORA_H_

#include "LoRaWAN/Core/lora.h"

/**
 * Initializes the LoRaWAN stack. Includes hardware initialization
 * \param use_ADR 1 to use Adaptive Data Rate, 0 to not use it. (You probably want to use it if your device will not move position)
 * \param initial_datarate The datarate the system will start with
 * \param is_public_network Unclear what this does for the library...
 * \param LORA_HasJoined Callback that will be called when the device has joined the network
 * \param LORA_RxData Callback that will be called when data has been received
 * \param LORA_ConfirmClass Callback that will be called when the class of the device has changed.
 */
void LoRa_initialize(int use_ADR, int initial_datarate, int is_public_network, void (*LORA_HasJoined)(void), void (*LORA_RxData )(lora_AppData_t *AppData), void (*LORA_ConfirmClass) (DeviceClass_t Class ));

/**
 * The device will try to join the network. This may fail.
 */
void LoRa_joinNetwork();

/**
 * The device will try to join the network until it has succeeded
 */
void LoRa_joinNetworkBlocking();

/**
 * Returns 1 if the device has joined the network and 0 if not
 */
int LoRa_isJoined();

/**
 * Returns 1 if the library says that it is clear to send a new message (It might not really be though...) and 0 if not
 */
int LoRa_canSend();

/**
 * Sends a new message over LoRaWAN. If it's not possible to send, it will block until it can.
 * Sometimes this will include the dutycycle and sometimes not, which seems like a bug in the library
 * \param data A pointer to the data you want to send
 * \param data_length The size of the data in byte count. The maximum depends on the state of the connection and the datarate. The absolute max is 242, a common max is 51 and a safe max is 35.
 * \param port The port to send it on. Valid range is 1-223
 * \param confirmation LORAWAN_CONFIRMED_MSG will send the message confirmed and LORAWAN_UNCONFIRMED_MSG will send it unconfirmed
 */
void LoRa_sendDataBlocking(uint8_t* data, uint8_t data_length, uint8_t port, LoraConfirm_t confirmation);

/**
 * Tries to send a new message over LoRaWAN
 * Returns 1 if data has been sent and 0 if not
 * \param data A pointer to the data you want to send
 * \param data_length The size of the data in byte count. The maximum depends on the state of the connection and the datarate. The absolute max is 242, a common max is 51 and a safe max is 35.
 * \param port The port to send it on. Valid range is 1-223
 * \param confirmation LORAWAN_CONFIRMED_MSG will send the message confirmed and LORAWAN_UNCONFIRMED_MSG will send it unconfirmed
 */
int LoRa_sendData(uint8_t* data, uint8_t data_length, uint8_t port, LoraConfirm_t confirmation);

/**
 * Requests the library to switch to a different class. (Class B needs to be enable through a define and both B and C need to be supported by the network)
 * \param class The class you want to change to
 */
void LoRa_changeClass(DeviceClass_t class);

/**
 * Must be called in infinite loop
 */
void LoRa_process();

#endif /* LORA_LORA_H_ */
