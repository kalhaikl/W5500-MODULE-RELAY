#include "main.h"
#include "wizchip_conf.h"
#include "socket.h"
#include "MQTT/MQTTClient.h"
#include "MQTT/mqtt_interface.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MQTT_SOCKET             0
#define MQTT_BUF_SIZE           1024
#define MQTT_KEEPALIVE          60

#define MQTT_BROKER_IP          {172,155,0,202}
#define MQTT_BROKER_PORT        1883

#define MQTT_CLIENT_ID          "STM32F103"
#define MQTT_USERNAME           "micro"
#define MQTT_PASSWORD           "Admin*46835micro"

#define TOPIC_IP                "WO/08/mezanin/micro/pintu_depan/IP"
#define TOPIC_LWT               "WO/08/mezanin/micro/pintu_depan/availability"
#define TOPIC_CMD               "WO/08/mezanin/micro/pintu_depan/cmnd/power"

static uint8_t mqtt_sendbuf[MQTT_BUF_SIZE];
static uint8_t mqtt_readbuf[MQTT_BUF_SIZE];

Network net;
MQTTClient client;

extern void RF_ProcessCommand(
    const char *topic,
    const char *payload
);

static uint8_t mqtt_connected = 0;
static uint32_t mqtt_last_check = 0;


/* =========================================================
 * MQTT MESSAGE CALLBACK
 * ========================================================= */

void messageArrived(MessageData *md)
{
    char topic[64];
    char payload[64];

    int topicLen =
        md->topicName->lenstring.len;

    int payloadLen =
        md->message->payloadlen;

    if (topicLen >= sizeof(topic))
        topicLen = sizeof(topic) - 1;

    if (payloadLen >= sizeof(payload))
        payloadLen = sizeof(payload) - 1;

    memcpy(
        topic,
        md->topicName->lenstring.data,
        topicLen
    );

    topic[topicLen] = '\0';

    memcpy(
        payload,
        md->message->payload,
        payloadLen
    );

    payload[payloadLen] = '\0';

    printf("\r\nMQTT MESSAGE\r\n");
    printf("Topic   : %s\r\n", topic);
    printf("Payload : %s\r\n", payload);

    if (strcmp(topic, TOPIC_CMD) == 0)
    {
        RF_ProcessCommand(
            topic,
            payload
        );

        return;
    }

    printf("Unknown MQTT topic\r\n");
}


/* =========================================================
 * TCP NETWORK INIT
 * ========================================================= */

int mqtt_network_init(void)
{
    uint8_t broker_ip[4] =
        MQTT_BROKER_IP;

    printf("=== MQTT START ===\r\n");

    close(MQTT_SOCKET);

    HAL_Delay(50);

    socket(
        MQTT_SOCKET,
        Sn_MR_TCP,
        0,
        0
    );

    int ret =
        connect(
            MQTT_SOCKET,
            broker_ip,
            MQTT_BROKER_PORT
        );

    printf(
        "TCP connect ret=%d\r\n",
        ret
    );

    if (ret != SOCK_OK)
    {
        printf(
            "TCP connect failed\r\n"
        );

        mqtt_connected = 0;

        return -1;
    }

    printf(
        "TCP Connected\r\n"
    );

    return 0;
}


/* =========================================================
 * MQTT CONNECT
 * ========================================================= */

int mqtt_connect_broker(void)
{
    MQTTPacket_connectData data =
        MQTTPacket_connectData_initializer;

    MQTTPacket_willOptions will =
        MQTTPacket_willOptions_initializer;

    NewNetwork(
        &net,
        MQTT_SOCKET
    );

    MQTTClientInit(
        &client,
        &net,
        30000,
        mqtt_sendbuf,
        MQTT_BUF_SIZE,
        mqtt_readbuf,
        MQTT_BUF_SIZE
    );

    data.MQTTVersion = 3;

    data.keepAliveInterval =
        MQTT_KEEPALIVE;

    data.cleansession = 1;

    data.clientID.cstring =
        MQTT_CLIENT_ID;

    data.username.cstring =
        MQTT_USERNAME;

    data.password.cstring =
        MQTT_PASSWORD;


    /*
     * MQTT Last Will
     */

    will.topicName.cstring =
        TOPIC_LWT;

    will.message.cstring =
        "offline";

    will.qos =
        QOS0;

    will.retained =
        1;

    data.willFlag = 1;

    data.will = will;


    printf(
        "MQTT: Connecting to broker...\r\n"
    );

    if (
        MQTTConnect(
            &client,
            &data
        ) != SUCCESS
    )
    {
        printf(
            "MQTT: Connection FAILED!\r\n"
        );

        mqtt_connected = 0;

        return -1;
    }

    printf(
        "MQTT: Connected successfully!\r\n"
    );


    /*
     * Publish ONLINE
     */

    MQTTMessage msg;

    msg.qos =
        QOS0;

    msg.retained =
        1;

    msg.dup =
        0;

    msg.payload =
        (void *)"online";

    msg.payloadlen =
        strlen("online");


    if (
        MQTTPublish(
            &client,
            TOPIC_LWT,
            &msg
        ) == SUCCESS
    )
    {
        printf(
            "MQTT: LWT online published\r\n"
        );
    }
    else
    {
        printf(
            "MQTT: LWT online publish FAILED\r\n"
        );

        mqtt_connected = 0;

        return -1;
    }


    mqtt_connected = 1;

    return 0;
}


/* =========================================================
 * MQTT SUBSCRIBE
 * ========================================================= */

int mqtt_subscribe(char *topic)
{
    if (
        MQTTSubscribe(
            &client,
            topic,
            QOS0,
            messageArrived
        ) != SUCCESS
    )
    {
        printf(
            "MQTT: Subscription failed!\r\n"
        );

        return -1;
    }

    printf(
        "MQTT: Subscribed to [%s]\r\n",
        topic
    );

    return 0;
}


/* =========================================================
 * MQTT PUBLISH
 * ========================================================= */

void mqtt_publish(
    char *topic,
    const char *payload
)
{
    MQTTMessage msg;

    msg.qos =
        QOS0;

    msg.retained =
        0;

    msg.dup =
        0;

    msg.payload =
        (void *)payload;

    msg.payloadlen =
        strlen(payload);


    printf(
        "Publish Topic : %s\r\n",
        topic
    );

    printf(
        "Publish Data  : %s\r\n",
        payload
    );


    if (
        MQTTPublish(
            &client,
            topic,
            &msg
        ) == SUCCESS
    )
    {
        printf(
            "MQTT: Published successfully\r\n"
        );
    }
    else
    {
        printf(
            "MQTT: Publish FAILED\r\n"
        );

        mqtt_connected = 0;
    }
}


/* =========================================================
 * MQTT PUBLISH INTEGER
 * ========================================================= */

void mqtt_publish_int(
    char *topic,
    int value
)
{
    char payload[16];

    snprintf(
        payload,
        sizeof(payload),
        "%d",
        value
    );

    mqtt_publish(
        topic,
        payload
    );
}


/* =========================================================
 * MQTT PUBLISH IP
 * ========================================================= */

void mqtt_publish_ip(
    char *topic
)
{
    wiz_NetInfo netInfo;

    char ipString[20];

    wizchip_getnetinfo(
        &netInfo
    );

    snprintf(
        ipString,
        sizeof(ipString),
        "%d.%d.%d.%d",
        netInfo.ip[0],
        netInfo.ip[1],
        netInfo.ip[2],
        netInfo.ip[3]
    );

    mqtt_publish(
        topic,
        ipString
    );
}


/* =========================================================
 * MQTT RECONNECT
 * ========================================================= */

static int mqtt_reconnect(void)
{
    uint8_t status;

    printf(
        "\r\n================================\r\n"
    );

    printf(
        "MQTT: CONNECTION LOST\r\n"
    );

    status =
        getSn_SR(MQTT_SOCKET);

    printf(
        "MQTT: Socket status = 0x%02X\r\n",
        status
    );


    mqtt_connected = 0;


    /*
     * Close old socket
     */

    close(MQTT_SOCKET);

    HAL_Delay(100);


    /*
     * Recreate TCP connection
     */

    if (
        mqtt_network_init() != 0
    )
    {
        printf(
            "MQTT: TCP reconnect FAILED\r\n"
        );

        return -1;
    }


    /*
     * Reconnect MQTT
     */

    if (
        mqtt_connect_broker() != 0
    )
    {
        printf(
            "MQTT: MQTT reconnect FAILED\r\n"
        );

        close(MQTT_SOCKET);

        return -1;
    }


    /*
     * Subscribe again
     */

    if (
        mqtt_subscribe(TOPIC_CMD) != 0
    )
    {
        printf(
            "MQTT: Resubscribe FAILED\r\n"
        );

        mqtt_connected = 0;

        close(MQTT_SOCKET);

        return -1;
    }


    /*
     * Publish IP again
     */

    mqtt_publish_ip(
        TOPIC_IP
    );


    printf(
        "MQTT: RECONNECTED SUCCESSFULLY\r\n"
    );

    printf(
        "================================\r\n"
    );


    return 0;
}


/* =========================================================
 * MQTT YIELD
 * ========================================================= */

void mqtt_yield(void)
{
    int rc;

    /*
     * Check MQTT every call
     */

    if (!mqtt_connected)
    {
        if (
            mqtt_reconnect() == 0
        )
        {
            printf(
                "MQTT: Connection restored\r\n"
            );
        }

        HAL_Delay(1000);

        return;
    }


    /*
     * Process MQTT packets
     */

    rc =
        MQTTYield(
            &client,
            100
        );


    if (rc != SUCCESS)
    {
        printf(
            "MQTTYield Error = %d\r\n",
            rc
        );

        mqtt_connected = 0;

        return;
    }


    /*
     * Additional socket check
     */

    if (
        getSn_SR(MQTT_SOCKET)
        != SOCK_ESTABLISHED
    )
    {
        printf(
            "MQTT: TCP socket disconnected\r\n"
        );

        mqtt_connected = 0;
    }
}
