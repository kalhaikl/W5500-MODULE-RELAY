################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/Ethernet_W550/MQTT/MQTTPacket/src/MQTTConnectClient.c \
../Drivers/Ethernet_W550/MQTT/MQTTPacket/src/MQTTConnectServer.c \
../Drivers/Ethernet_W550/MQTT/MQTTPacket/src/MQTTDeserializePublish.c \
../Drivers/Ethernet_W550/MQTT/MQTTPacket/src/MQTTFormat.c \
../Drivers/Ethernet_W550/MQTT/MQTTPacket/src/MQTTPacket.c \
../Drivers/Ethernet_W550/MQTT/MQTTPacket/src/MQTTSerializePublish.c \
../Drivers/Ethernet_W550/MQTT/MQTTPacket/src/MQTTSubscribeClient.c \
../Drivers/Ethernet_W550/MQTT/MQTTPacket/src/MQTTSubscribeServer.c \
../Drivers/Ethernet_W550/MQTT/MQTTPacket/src/MQTTUnsubscribeClient.c \
../Drivers/Ethernet_W550/MQTT/MQTTPacket/src/MQTTUnsubscribeServer.c 

OBJS += \
./Drivers/Ethernet_W550/MQTT/MQTTPacket/src/MQTTConnectClient.o \
./Drivers/Ethernet_W550/MQTT/MQTTPacket/src/MQTTConnectServer.o \
./Drivers/Ethernet_W550/MQTT/MQTTPacket/src/MQTTDeserializePublish.o \
./Drivers/Ethernet_W550/MQTT/MQTTPacket/src/MQTTFormat.o \
./Drivers/Ethernet_W550/MQTT/MQTTPacket/src/MQTTPacket.o \
./Drivers/Ethernet_W550/MQTT/MQTTPacket/src/MQTTSerializePublish.o \
./Drivers/Ethernet_W550/MQTT/MQTTPacket/src/MQTTSubscribeClient.o \
./Drivers/Ethernet_W550/MQTT/MQTTPacket/src/MQTTSubscribeServer.o \
./Drivers/Ethernet_W550/MQTT/MQTTPacket/src/MQTTUnsubscribeClient.o \
./Drivers/Ethernet_W550/MQTT/MQTTPacket/src/MQTTUnsubscribeServer.o 

C_DEPS += \
./Drivers/Ethernet_W550/MQTT/MQTTPacket/src/MQTTConnectClient.d \
./Drivers/Ethernet_W550/MQTT/MQTTPacket/src/MQTTConnectServer.d \
./Drivers/Ethernet_W550/MQTT/MQTTPacket/src/MQTTDeserializePublish.d \
./Drivers/Ethernet_W550/MQTT/MQTTPacket/src/MQTTFormat.d \
./Drivers/Ethernet_W550/MQTT/MQTTPacket/src/MQTTPacket.d \
./Drivers/Ethernet_W550/MQTT/MQTTPacket/src/MQTTSerializePublish.d \
./Drivers/Ethernet_W550/MQTT/MQTTPacket/src/MQTTSubscribeClient.d \
./Drivers/Ethernet_W550/MQTT/MQTTPacket/src/MQTTSubscribeServer.d \
./Drivers/Ethernet_W550/MQTT/MQTTPacket/src/MQTTUnsubscribeClient.d \
./Drivers/Ethernet_W550/MQTT/MQTTPacket/src/MQTTUnsubscribeServer.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/Ethernet_W550/MQTT/MQTTPacket/src/%.o Drivers/Ethernet_W550/MQTT/MQTTPacket/src/%.su Drivers/Ethernet_W550/MQTT/MQTTPacket/src/%.cyclo: ../Drivers/Ethernet_W550/MQTT/MQTTPacket/src/%.c Drivers/Ethernet_W550/MQTT/MQTTPacket/src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F103x6 -c -I../Core/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -I"C:/Users/HAIKAL/STM32CubeIDE/workspace_1.19.0/w5500-sht-read-tcp/Drivers/Ethernet_W550" -Os -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Drivers-2f-Ethernet_W550-2f-MQTT-2f-MQTTPacket-2f-src

clean-Drivers-2f-Ethernet_W550-2f-MQTT-2f-MQTTPacket-2f-src:
	-$(RM) ./Drivers/Ethernet_W550/MQTT/MQTTPacket/src/MQTTConnectClient.cyclo ./Drivers/Ethernet_W550/MQTT/MQTTPacket/src/MQTTConnectClient.d ./Drivers/Ethernet_W550/MQTT/MQTTPacket/src/MQTTConnectClient.o ./Drivers/Ethernet_W550/MQTT/MQTTPacket/src/MQTTConnectClient.su ./Drivers/Ethernet_W550/MQTT/MQTTPacket/src/MQTTConnectServer.cyclo ./Drivers/Ethernet_W550/MQTT/MQTTPacket/src/MQTTConnectServer.d ./Drivers/Ethernet_W550/MQTT/MQTTPacket/src/MQTTConnectServer.o ./Drivers/Ethernet_W550/MQTT/MQTTPacket/src/MQTTConnectServer.su ./Drivers/Ethernet_W550/MQTT/MQTTPacket/src/MQTTDeserializePublish.cyclo ./Drivers/Ethernet_W550/MQTT/MQTTPacket/src/MQTTDeserializePublish.d ./Drivers/Ethernet_W550/MQTT/MQTTPacket/src/MQTTDeserializePublish.o ./Drivers/Ethernet_W550/MQTT/MQTTPacket/src/MQTTDeserializePublish.su ./Drivers/Ethernet_W550/MQTT/MQTTPacket/src/MQTTFormat.cyclo ./Drivers/Ethernet_W550/MQTT/MQTTPacket/src/MQTTFormat.d ./Drivers/Ethernet_W550/MQTT/MQTTPacket/src/MQTTFormat.o ./Drivers/Ethernet_W550/MQTT/MQTTPacket/src/MQTTFormat.su ./Drivers/Ethernet_W550/MQTT/MQTTPacket/src/MQTTPacket.cyclo ./Drivers/Ethernet_W550/MQTT/MQTTPacket/src/MQTTPacket.d ./Drivers/Ethernet_W550/MQTT/MQTTPacket/src/MQTTPacket.o ./Drivers/Ethernet_W550/MQTT/MQTTPacket/src/MQTTPacket.su ./Drivers/Ethernet_W550/MQTT/MQTTPacket/src/MQTTSerializePublish.cyclo ./Drivers/Ethernet_W550/MQTT/MQTTPacket/src/MQTTSerializePublish.d ./Drivers/Ethernet_W550/MQTT/MQTTPacket/src/MQTTSerializePublish.o ./Drivers/Ethernet_W550/MQTT/MQTTPacket/src/MQTTSerializePublish.su ./Drivers/Ethernet_W550/MQTT/MQTTPacket/src/MQTTSubscribeClient.cyclo ./Drivers/Ethernet_W550/MQTT/MQTTPacket/src/MQTTSubscribeClient.d ./Drivers/Ethernet_W550/MQTT/MQTTPacket/src/MQTTSubscribeClient.o ./Drivers/Ethernet_W550/MQTT/MQTTPacket/src/MQTTSubscribeClient.su ./Drivers/Ethernet_W550/MQTT/MQTTPacket/src/MQTTSubscribeServer.cyclo ./Drivers/Ethernet_W550/MQTT/MQTTPacket/src/MQTTSubscribeServer.d ./Drivers/Ethernet_W550/MQTT/MQTTPacket/src/MQTTSubscribeServer.o ./Drivers/Ethernet_W550/MQTT/MQTTPacket/src/MQTTSubscribeServer.su ./Drivers/Ethernet_W550/MQTT/MQTTPacket/src/MQTTUnsubscribeClient.cyclo ./Drivers/Ethernet_W550/MQTT/MQTTPacket/src/MQTTUnsubscribeClient.d ./Drivers/Ethernet_W550/MQTT/MQTTPacket/src/MQTTUnsubscribeClient.o ./Drivers/Ethernet_W550/MQTT/MQTTPacket/src/MQTTUnsubscribeClient.su ./Drivers/Ethernet_W550/MQTT/MQTTPacket/src/MQTTUnsubscribeServer.cyclo ./Drivers/Ethernet_W550/MQTT/MQTTPacket/src/MQTTUnsubscribeServer.d ./Drivers/Ethernet_W550/MQTT/MQTTPacket/src/MQTTUnsubscribeServer.o ./Drivers/Ethernet_W550/MQTT/MQTTPacket/src/MQTTUnsubscribeServer.su

.PHONY: clean-Drivers-2f-Ethernet_W550-2f-MQTT-2f-MQTTPacket-2f-src

