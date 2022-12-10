/***********************************************************
 * General includes
 ***********************************************************/
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <common/code_utils.hpp>
/***********************************************************
 * OpenThread includes
 ***********************************************************/
#include <openthread/cli.h>
#include <openthread/dataset_ftd.h>
#include <openthread/instance.h>
#include <openthread/thread.h>
#include <openthread/udp.h>

/***********************************************************
 * Peripheral Includes
 ***********************************************************/
#include "sl_button.h"
#include "sl_simple_button.h"

/***********************************************************
 * Custom Includes
 ***********************************************************/
#include "sensors.h"

/***********************************************************
 * Macros
 ***********************************************************/

#define ADDR "fd69:29a9:2e20:2:0:0::c0a8:8901"
#define PORT 12345

/***********************************************************
 * Forward Declarations
 ***********************************************************/
otInstance *otGetInstance(void);
void        ReceiveCallback(void *aContext, otMessage *aMessage, const otMessageInfo *aMessageInfo);
extern void otSysEventSignalPending(void);

/***********************************************************
 * Global variables
 ***********************************************************/
static bool         ButtonPressed = false;
static otUdpSocket  Socket;
static uint16_t     rloc16;


/***********************************************************
 * Function
 ***********************************************************/
void programInit(void)
{
  otCliOutputFormat("Program started.\r\n");
  sensorInit();
}

void programDeInit(void)
{
  sensorDeInit();
}


void sl_button_on_change(const sl_button_t *handle)
{
    if (sl_button_get_state(handle) == SL_SIMPLE_BUTTON_PRESSED)
    {
        ButtonPressed = true;
        otSysEventSignalPending();
    }
}

void applicationTick(void)
{
      otMessageInfo    messageInfo;
      otMessage *      message = NULL;

      uint32_t humidity;
      int32_t temperature;
      float lux;
      float uvi;
      float pressure;
      uint16_t eco2;
      uint16_t tvoc;
      float sound_level;
      char values[9][50];
      sl_service_rht_get(&humidity, &temperature);
      sl_service_light_get(&lux, &uvi);
      sl_service_pressure_get(&pressure);
      sl_service_gas_get(&eco2, &tvoc);
      sl_service_sound_get(&sound_level);

      sprintf(values[0], "%d,", rloc16);
      sprintf(values[1], "%lu,", humidity);
      sprintf(values[2], "%ld,", temperature);
      sprintf(values[3], "%lu,", (uint32_t)(lux * 100));
      sprintf(values[4], "%d,", (uint8_t)uvi);
      sprintf(values[5], "%lu,", (uint32_t)(pressure * 1000.0f));
      sprintf(values[6], "%d,", eco2);
      sprintf(values[7], "%d,", tvoc);
      sprintf(values[8], "%d\n", (int16_t)(sound_level * 100.0f));

      for(int i = 1; i <= 8; i++)
        {
           strcat(values[0], values[i]);
        }

      const char * payload = values[0];

      otCliOutputFormat(payload);
      // Get a message buffer
      VerifyOrExit((message = otUdpNewMessage(otGetInstance(), NULL)) != NULL);

      // Setup messageInfo
      memset(&messageInfo, 0, sizeof(messageInfo));
      SuccessOrExit(otIp6AddressFromString(ADDR, &messageInfo.mPeerAddr));
      messageInfo.mPeerPort = PORT;

      // Append the MESSAGE payload to the message buffer
      SuccessOrExit(otMessageAppend(message, payload, (uint16_t)strlen(payload)));

      // Send the button press message
      SuccessOrExit(otUdpSend(otGetInstance(), &Socket, message, &messageInfo));

      // Set message pointer to NULL so it doesn't get free'd by this function.
      // otUdpSend() executing successfully means OpenThread has taken ownership
      // of the message buffer.
      message = NULL;
      exit:
          if (message != NULL)
          {
              otMessageFree(message);
          }
          return;
}

void ReceiveCallback(void *aContext, otMessage *aMessage, const otMessageInfo *aMessageInfo)
{
    OT_UNUSED_VARIABLE(aContext);
    OT_UNUSED_VARIABLE(aMessageInfo);
    uint8_t buf[64];
    int     length;

    // Read the received message's payload
    length      = otMessageRead(aMessage, otMessageGetOffset(aMessage), buf, sizeof(buf) - 1);
    buf[length] = '\0';

    otCliOutputFormat("Message Received: %s\r\n", buf);
}

void initUdp(void)
{
    otError    error;
    otSockAddr bindAddr;

    // Initialize bindAddr
    memset(&bindAddr, 0, sizeof(bindAddr));
    bindAddr.mPort = PORT;

    // Open the socket
    error = otUdpOpen(otGetInstance(), &Socket, ReceiveCallback, NULL);
    if (error != OT_ERROR_NONE)
    {
        otCliOutputFormat("Failed to open udp socket with: %d, %s\r\n", error, otThreadErrorToString(error));
        return;
    }

    // Bind to the socket. Close the socket if bind fails.
    error = otUdpBind(otGetInstance(), &Socket, &bindAddr, OT_NETIF_THREAD);
    if (error != OT_ERROR_NONE)
    {
        otCliOutputFormat("Failed to bind udp socket with: %d, %s\r\n", error, otThreadErrorToString(error));
        IgnoreReturnValue(otUdpClose(otGetInstance(), &Socket));
        return;
    }
}

/*
 * Override default network settings, such as panid, so the devices can join a network
 */
void setNetworkConfiguration(void)
{
    static char          aNetworkName[] = "Demo";
    otError              error;
    otOperationalDataset aDataset;

    memset(&aDataset, 0, sizeof(otOperationalDataset));

    /*
     * Fields that can be configured in otOperationDataset to override defaults:
     *     Network Name, Mesh Local Prefix, Extended PAN ID, PAN ID, Delay Timer,
     *     Channel, Channel Mask Page 0, Network Key, PSKc, Security Policy
     */
    aDataset.mActiveTimestamp.mSeconds             = 1;
    aDataset.mComponents.mIsActiveTimestampPresent = true;

    /* Set Channel to 15 */
    aDataset.mChannel                      = 15;
    aDataset.mComponents.mIsChannelPresent = true;

    /* Set Pan ID to 2222 */
    aDataset.mPanId                      = (otPanId)0x1234;
    aDataset.mComponents.mIsPanIdPresent = true;

    /* Set Extended Pan ID to 1111111122222222 */
    uint8_t extPanId[OT_EXT_PAN_ID_SIZE] = {0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11};
    memcpy(aDataset.mExtendedPanId.m8, extPanId, sizeof(aDataset.mExtendedPanId));
    aDataset.mComponents.mIsExtendedPanIdPresent = true;

    /* Set network key to 00112233445566778899aabbccddeeff */
    uint8_t key[OT_NETWORK_KEY_SIZE] = {0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77,
                                        0x88, 0x99, 0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF};
    memcpy(aDataset.mNetworkKey.m8, key, sizeof(aDataset.mNetworkKey));
    aDataset.mComponents.mIsNetworkKeyPresent = true;

    /* Set Network Name to Demo */
    size_t length = strlen(aNetworkName);
    assert(length <= OT_NETWORK_NAME_MAX_SIZE);
    memcpy(aDataset.mNetworkName.m8, aNetworkName, length);
    aDataset.mComponents.mIsNetworkNamePresent = true;

    /* Set the Active Operational Dataset to this dataset */
    error = otDatasetSetActive(otGetInstance(), &aDataset);
    if (error != OT_ERROR_NONE)
    {
        otCliOutputFormat("otDatasetSetActive failed with: %d, %s\r\n", error, otThreadErrorToString(error));
        return;
    }
    rloc16 = otThreadGetRloc16(otGetInstance());

}
