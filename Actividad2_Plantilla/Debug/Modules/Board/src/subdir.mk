################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Modules/Board/src/ledpul.c \
../Modules/Board/src/ssp.c 

OBJS += \
./Modules/Board/src/ledpul.o \
./Modules/Board/src/ssp.o 

C_DEPS += \
./Modules/Board/src/ledpul.d \
./Modules/Board/src/ssp.d 


# Each subdirectory must supply rules for building sources it contributes
Modules/Board/src/%.o: ../Modules/Board/src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -D__REDLIB__ -DDEBUG -D__CODE_RED -D__USE_CMSIS=CMSISv2p00_LPC13xx -D__USE_CMSIS_DSPLIB=CMSISv2p10_DSPLIB_CM3 -I"/home/pato/Embebidos/Protocolos/protocolos_ws/Actividad2_Plantilla/src" -I"/home/pato/Embebidos/Protocolos/protocolos_ws/Actividad2_Plantilla/Modules/Board/inc" -I"/home/pato/Embebidos/Protocolos/protocolos_ws/Actividad2_Plantilla/Modules/cc2520/inc" -I"/home/pato/Embebidos/Protocolos/protocolos_ws/CMSISv2p00_LPC13xx/inc" -O0 -g3 -Wall -c -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections -mcpu=cortex-m3 -mthumb -D__REDLIB__ -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


