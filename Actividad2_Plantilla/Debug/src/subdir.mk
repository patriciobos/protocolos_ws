################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/cr_startup_lpc13.c \
../src/main.c 

OBJS += \
./src/cr_startup_lpc13.o \
./src/main.o 

C_DEPS += \
./src/cr_startup_lpc13.d \
./src/main.d 


# Each subdirectory must supply rules for building sources it contributes
src/cr_startup_lpc13.o: ../src/cr_startup_lpc13.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -D__REDLIB__ -DDEBUG -D__CODE_RED -D__USE_CMSIS=CMSISv2p00_LPC13xx -D__USE_CMSIS_DSPLIB=CMSISv2p10_DSPLIB_CM3 -I"/home/patricio/Embebidos/Protocolos/Practica/ws-1343 hola mundo/Actividad2_Plantilla/src" -I"/home/patricio/Embebidos/Protocolos/Practica/ws-1343 hola mundo/Actividad2_Plantilla/Modules/Board/inc" -I"/home/patricio/Embebidos/Protocolos/Practica/ws-1343 hola mundo/Actividad2_Plantilla/Modules/cc2520/inc" -I"/home/patricio/Embebidos/Protocolos/Practica/ws-1343 hola mundo/CMSISv2p00_LPC13xx/inc" -I"/home/patricio/Embebidos/Protocolos/Practica/ws-1343 hola mundo/CMSISv2p10_DSPLIB_CM3/inc" -Os -g3 -Wall -c -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections -mcpu=cortex-m3 -mthumb -D__REDLIB__ -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"src/cr_startup_lpc13.d" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -D__REDLIB__ -DDEBUG -D__CODE_RED -D__USE_CMSIS=CMSISv2p00_LPC13xx -D__USE_CMSIS_DSPLIB=CMSISv2p10_DSPLIB_CM3 -I"/home/patricio/Embebidos/Protocolos/Practica/ws-1343 hola mundo/Actividad2_Plantilla/src" -I"/home/patricio/Embebidos/Protocolos/Practica/ws-1343 hola mundo/Actividad2_Plantilla/Modules/Board/inc" -I"/home/patricio/Embebidos/Protocolos/Practica/ws-1343 hola mundo/Actividad2_Plantilla/Modules/cc2520/inc" -I"/home/patricio/Embebidos/Protocolos/Practica/ws-1343 hola mundo/CMSISv2p00_LPC13xx/inc" -I"/home/patricio/Embebidos/Protocolos/Practica/ws-1343 hola mundo/CMSISv2p10_DSPLIB_CM3/inc" -O0 -g3 -Wall -c -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections -mcpu=cortex-m3 -mthumb -D__REDLIB__ -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


