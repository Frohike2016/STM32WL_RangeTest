################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../App/app_subghz_phy.c \
../App/subghz_phy_app.c 

OBJS += \
./App/app_subghz_phy.o \
./App/subghz_phy_app.o 

C_DEPS += \
./App/app_subghz_phy.d \
./App/subghz_phy_app.d 


# Each subdirectory must supply rules for building sources it contributes
App/%.o App/%.su App/%.cyclo: ../App/%.c App/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DCORE_CM4 -DUSE_HAL_DRIVER -DSTM32WL55xx -c -I../Core -I../Core/Peripherals -I../Core/Configuration -I../Drivers/STM32WLxx_HAL_Driver/Inc -I../Drivers/STM32WLxx_HAL_Driver/Inc/Legacy -I../Utilities/lpm/tiny_lpm -I../Utilities/misc -I../Utilities/sequencer -I../Utilities/timer -I../Utilities/trace/adv_trace -I../Middlewares/Third_Party/SubGHz_Phy -I../Middlewares/Third_Party/SubGHz_Phy/stm32_radio_driver -I../Drivers/CMSIS/Include -I../Drivers/CMSIS/Device/ST/STM32WLxx/Include -I../Drivers/Hardware -I../App -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-App

clean-App:
	-$(RM) ./App/app_subghz_phy.cyclo ./App/app_subghz_phy.d ./App/app_subghz_phy.o ./App/app_subghz_phy.su ./App/subghz_phy_app.cyclo ./App/subghz_phy_app.d ./App/subghz_phy_app.o ./App/subghz_phy_app.su

.PHONY: clean-App

