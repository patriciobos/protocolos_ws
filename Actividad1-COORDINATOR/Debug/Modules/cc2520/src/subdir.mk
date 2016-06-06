################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Modules/cc2520/src/cc2520-mac.c \
../Modules/cc2520/src/cc2520-task.c \
../Modules/cc2520/src/cc2520.c 

OBJS += \
./Modules/cc2520/src/cc2520-mac.o \
./Modules/cc2520/src/cc2520-task.o \
./Modules/cc2520/src/cc2520.o 

C_DEPS += \
./Modules/cc2520/src/cc2520-mac.d \
./Modules/cc2520/src/cc2520-task.d \
./Modules/cc2520/src/cc2520.d 


# Each subdirectory must supply rules for building sources it contributes
Modules/cc2520/src/%.o: ../Modules/cc2520/src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -D__REDLIB__ -DDEBUG -D__CODE_RED -D__USE_CMSIS=CMSISv2p00_LPC13xx -D__USE_CMSIS_DSPLIB=CMSISv2p10_DSPLIB_CM3 -I"/home/patricio/Embebidos/Protocolos/Practica/ws-1343 hola mundo/Actividad1-COORDINATOR/src" -I"/home/patricio/Embebidos/Protocolos/Practica/ws-1343 hola mundo/Actividad1-COORDINATOR/Modules/Board/inc" -I"/home/patricio/Embebidos/Protocolos/Practica/ws-1343 hola mundo/Actividad1-COORDINATOR/Modules/cc2520/inc" -I"/home/patricio/Embebidos/Protocolos/Practica/ws-1343 hola mundo/CMSISv2p00_LPC13xx/inc" -I"/home/patricio/Embebidos/Protocolos/Practica/ws-1343 hola mundo/CMSISv2p10_DSPLIB_CM3/inc" -O0 -g3 -Wall -c -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections -mcpu=cortex-m3 -mthumb -D__REDLIB__ -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


