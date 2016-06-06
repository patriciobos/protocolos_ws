################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include src/subdir.mk
-include Modules/cc2520/src/subdir.mk
-include Modules/Board/src/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all: Actividad1-DEVICE.axf

# Tool invocations
Actividad1-DEVICE.axf: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: MCU Linker'
	arm-none-eabi-gcc -nostdlib -L"/home/patricio/Embebidos/Protocolos/protocolos_ws/CMSISv2p00_LPC13xx/Debug" -Xlinker -Map="Actividad1-DEVICE.map" -Xlinker --gc-sections -mcpu=cortex-m3 -mthumb -T "Actividad1_Debug.ld" -o "Actividad1-DEVICE.axf" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '
	$(MAKE) --no-print-directory post-build

# Other Targets
clean:
	-$(RM) $(EXECUTABLES)$(OBJS)$(C_DEPS) Actividad1-DEVICE.axf
	-@echo ' '

post-build:
	-@echo 'Performing post-build steps'
	-arm-none-eabi-size "Actividad1-DEVICE.axf"; # arm-none-eabi-objcopy -O binary "Actividad1-DEVICE.axf" "Actividad1-DEVICE.bin" ; checksum -p LPC1343 -d "Actividad1-DEVICE.bin";
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY: post-build

-include ../makefile.targets
