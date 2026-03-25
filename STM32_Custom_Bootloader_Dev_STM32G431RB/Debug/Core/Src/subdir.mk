################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (14.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/boot_button.c \
../Core/Src/boot_config.c \
../Core/Src/boot_crc.c \
../Core/Src/boot_image.c \
../Core/Src/boot_jump.c \
../Core/Src/boot_led.c \
../Core/Src/boot_state.c \
../Core/Src/gpio.c \
../Core/Src/main.c \
../Core/Src/retarget.c \
../Core/Src/std_def.c \
../Core/Src/stm32g4xx_hal_msp.c \
../Core/Src/stm32g4xx_it.c \
../Core/Src/syscalls.c \
../Core/Src/sysmem.c \
../Core/Src/system_stm32g4xx.c \
../Core/Src/usart.c 

OBJS += \
./Core/Src/boot_button.o \
./Core/Src/boot_config.o \
./Core/Src/boot_crc.o \
./Core/Src/boot_image.o \
./Core/Src/boot_jump.o \
./Core/Src/boot_led.o \
./Core/Src/boot_state.o \
./Core/Src/gpio.o \
./Core/Src/main.o \
./Core/Src/retarget.o \
./Core/Src/std_def.o \
./Core/Src/stm32g4xx_hal_msp.o \
./Core/Src/stm32g4xx_it.o \
./Core/Src/syscalls.o \
./Core/Src/sysmem.o \
./Core/Src/system_stm32g4xx.o \
./Core/Src/usart.o 

C_DEPS += \
./Core/Src/boot_button.d \
./Core/Src/boot_config.d \
./Core/Src/boot_crc.d \
./Core/Src/boot_image.d \
./Core/Src/boot_jump.d \
./Core/Src/boot_led.d \
./Core/Src/boot_state.d \
./Core/Src/gpio.d \
./Core/Src/main.d \
./Core/Src/retarget.d \
./Core/Src/std_def.d \
./Core/Src/stm32g4xx_hal_msp.d \
./Core/Src/stm32g4xx_it.d \
./Core/Src/syscalls.d \
./Core/Src/sysmem.d \
./Core/Src/system_stm32g4xx.d \
./Core/Src/usart.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/%.o Core/Src/%.su Core/Src/%.cyclo: ../Core/Src/%.c Core/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32G431xx -c -I../Core/Inc -I../Drivers/STM32G4xx_HAL_Driver/Inc -I../Drivers/STM32G4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32G4xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Src

clean-Core-2f-Src:
	-$(RM) ./Core/Src/boot_button.cyclo ./Core/Src/boot_button.d ./Core/Src/boot_button.o ./Core/Src/boot_button.su ./Core/Src/boot_config.cyclo ./Core/Src/boot_config.d ./Core/Src/boot_config.o ./Core/Src/boot_config.su ./Core/Src/boot_crc.cyclo ./Core/Src/boot_crc.d ./Core/Src/boot_crc.o ./Core/Src/boot_crc.su ./Core/Src/boot_image.cyclo ./Core/Src/boot_image.d ./Core/Src/boot_image.o ./Core/Src/boot_image.su ./Core/Src/boot_jump.cyclo ./Core/Src/boot_jump.d ./Core/Src/boot_jump.o ./Core/Src/boot_jump.su ./Core/Src/boot_led.cyclo ./Core/Src/boot_led.d ./Core/Src/boot_led.o ./Core/Src/boot_led.su ./Core/Src/boot_state.cyclo ./Core/Src/boot_state.d ./Core/Src/boot_state.o ./Core/Src/boot_state.su ./Core/Src/gpio.cyclo ./Core/Src/gpio.d ./Core/Src/gpio.o ./Core/Src/gpio.su ./Core/Src/main.cyclo ./Core/Src/main.d ./Core/Src/main.o ./Core/Src/main.su ./Core/Src/retarget.cyclo ./Core/Src/retarget.d ./Core/Src/retarget.o ./Core/Src/retarget.su ./Core/Src/std_def.cyclo ./Core/Src/std_def.d ./Core/Src/std_def.o ./Core/Src/std_def.su ./Core/Src/stm32g4xx_hal_msp.cyclo ./Core/Src/stm32g4xx_hal_msp.d ./Core/Src/stm32g4xx_hal_msp.o ./Core/Src/stm32g4xx_hal_msp.su ./Core/Src/stm32g4xx_it.cyclo ./Core/Src/stm32g4xx_it.d ./Core/Src/stm32g4xx_it.o ./Core/Src/stm32g4xx_it.su ./Core/Src/syscalls.cyclo ./Core/Src/syscalls.d ./Core/Src/syscalls.o ./Core/Src/syscalls.su ./Core/Src/sysmem.cyclo ./Core/Src/sysmem.d ./Core/Src/sysmem.o ./Core/Src/sysmem.su ./Core/Src/system_stm32g4xx.cyclo ./Core/Src/system_stm32g4xx.d ./Core/Src/system_stm32g4xx.o ./Core/Src/system_stm32g4xx.su ./Core/Src/usart.cyclo ./Core/Src/usart.d ./Core/Src/usart.o ./Core/Src/usart.su

.PHONY: clean-Core-2f-Src

