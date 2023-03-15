################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Peripherals/adc.c \
../Core/Peripherals/adc_if.c \
../Core/Peripherals/dma.c \
../Core/Peripherals/rtc.c \
../Core/Peripherals/subghz.c \
../Core/Peripherals/usart.c \
../Core/Peripherals/usart_if.c 

OBJS += \
./Core/Peripherals/adc.o \
./Core/Peripherals/adc_if.o \
./Core/Peripherals/dma.o \
./Core/Peripherals/rtc.o \
./Core/Peripherals/subghz.o \
./Core/Peripherals/usart.o \
./Core/Peripherals/usart_if.o 

C_DEPS += \
./Core/Peripherals/adc.d \
./Core/Peripherals/adc_if.d \
./Core/Peripherals/dma.d \
./Core/Peripherals/rtc.d \
./Core/Peripherals/subghz.d \
./Core/Peripherals/usart.d \
./Core/Peripherals/usart_if.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Peripherals/%.o Core/Peripherals/%.su Core/Peripherals/%.cyclo: ../Core/Peripherals/%.c Core/Peripherals/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DCORE_CM4 -DUSE_HAL_DRIVER -DSTM32WL55xx -c -I../Core -I../Core/Peripherals -I../Core/Configuration -I../Drivers/STM32WLxx_HAL_Driver/Inc -I../Drivers/STM32WLxx_HAL_Driver/Inc/Legacy -I../Utilities/lpm/tiny_lpm -I../Utilities/misc -I../Utilities/sequencer -I../Utilities/timer -I../Utilities/trace/adv_trace -I../Middlewares/Third_Party/SubGHz_Phy -I../Middlewares/Third_Party/SubGHz_Phy/stm32_radio_driver -I../Drivers/CMSIS/Include -I../Drivers/CMSIS/Device/ST/STM32WLxx/Include -I../Drivers/Hardware -I../App -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Core-2f-Peripherals

clean-Core-2f-Peripherals:
	-$(RM) ./Core/Peripherals/adc.cyclo ./Core/Peripherals/adc.d ./Core/Peripherals/adc.o ./Core/Peripherals/adc.su ./Core/Peripherals/adc_if.cyclo ./Core/Peripherals/adc_if.d ./Core/Peripherals/adc_if.o ./Core/Peripherals/adc_if.su ./Core/Peripherals/dma.cyclo ./Core/Peripherals/dma.d ./Core/Peripherals/dma.o ./Core/Peripherals/dma.su ./Core/Peripherals/rtc.cyclo ./Core/Peripherals/rtc.d ./Core/Peripherals/rtc.o ./Core/Peripherals/rtc.su ./Core/Peripherals/subghz.cyclo ./Core/Peripherals/subghz.d ./Core/Peripherals/subghz.o ./Core/Peripherals/subghz.su ./Core/Peripherals/usart.cyclo ./Core/Peripherals/usart.d ./Core/Peripherals/usart.o ./Core/Peripherals/usart.su ./Core/Peripherals/usart_if.cyclo ./Core/Peripherals/usart_if.d ./Core/Peripherals/usart_if.o ./Core/Peripherals/usart_if.su

.PHONY: clean-Core-2f-Peripherals

