################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/Hardware/board_resources.c \
../Drivers/Hardware/radio_board_if.c 

OBJS += \
./Drivers/Hardware/board_resources.o \
./Drivers/Hardware/radio_board_if.o 

C_DEPS += \
./Drivers/Hardware/board_resources.d \
./Drivers/Hardware/radio_board_if.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/Hardware/%.o Drivers/Hardware/%.su Drivers/Hardware/%.cyclo: ../Drivers/Hardware/%.c Drivers/Hardware/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DCORE_CM4 -DUSE_HAL_DRIVER -DSTM32WL55xx -c -I../Core -I../Core/Peripherals -I../Core/Configuration -I../Drivers/STM32WLxx_HAL_Driver/Inc -I../Drivers/STM32WLxx_HAL_Driver/Inc/Legacy -I../Utilities/lpm/tiny_lpm -I../Utilities/misc -I../Utilities/sequencer -I../Utilities/timer -I../Utilities/trace/adv_trace -I../Middlewares/Third_Party/SubGHz_Phy -I../Middlewares/Third_Party/SubGHz_Phy/stm32_radio_driver -I../Drivers/CMSIS/Include -I../Drivers/CMSIS/Device/ST/STM32WLxx/Include -I../Drivers/Hardware -I../App -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Drivers-2f-Hardware

clean-Drivers-2f-Hardware:
	-$(RM) ./Drivers/Hardware/board_resources.cyclo ./Drivers/Hardware/board_resources.d ./Drivers/Hardware/board_resources.o ./Drivers/Hardware/board_resources.su ./Drivers/Hardware/radio_board_if.cyclo ./Drivers/Hardware/radio_board_if.d ./Drivers/Hardware/radio_board_if.o ./Drivers/Hardware/radio_board_if.su

.PHONY: clean-Drivers-2f-Hardware

