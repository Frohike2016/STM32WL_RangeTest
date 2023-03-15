################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/main.c \
../Core/stm32_lpm_if.c \
../Core/stm32wlxx_hal_msp.c \
../Core/stm32wlxx_it.c \
../Core/sys_app.c \
../Core/sys_debug.c \
../Core/syscalls.c \
../Core/sysmem.c \
../Core/system_stm32wlxx.c \
../Core/timer_if.c 

OBJS += \
./Core/main.o \
./Core/stm32_lpm_if.o \
./Core/stm32wlxx_hal_msp.o \
./Core/stm32wlxx_it.o \
./Core/sys_app.o \
./Core/sys_debug.o \
./Core/syscalls.o \
./Core/sysmem.o \
./Core/system_stm32wlxx.o \
./Core/timer_if.o 

C_DEPS += \
./Core/main.d \
./Core/stm32_lpm_if.d \
./Core/stm32wlxx_hal_msp.d \
./Core/stm32wlxx_it.d \
./Core/sys_app.d \
./Core/sys_debug.d \
./Core/syscalls.d \
./Core/sysmem.d \
./Core/system_stm32wlxx.d \
./Core/timer_if.d 


# Each subdirectory must supply rules for building sources it contributes
Core/%.o Core/%.su Core/%.cyclo: ../Core/%.c Core/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DCORE_CM4 -DUSE_HAL_DRIVER -DSTM32WL55xx -c -I../Core -I../Core/Peripherals -I../Core/Configuration -I../Drivers/STM32WLxx_HAL_Driver/Inc -I../Drivers/STM32WLxx_HAL_Driver/Inc/Legacy -I../Utilities/lpm/tiny_lpm -I../Utilities/misc -I../Utilities/sequencer -I../Utilities/timer -I../Utilities/trace/adv_trace -I../Middlewares/Third_Party/SubGHz_Phy -I../Middlewares/Third_Party/SubGHz_Phy/stm32_radio_driver -I../Drivers/CMSIS/Include -I../Drivers/CMSIS/Device/ST/STM32WLxx/Include -I../Drivers/Hardware -I../App -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Core

clean-Core:
	-$(RM) ./Core/main.cyclo ./Core/main.d ./Core/main.o ./Core/main.su ./Core/stm32_lpm_if.cyclo ./Core/stm32_lpm_if.d ./Core/stm32_lpm_if.o ./Core/stm32_lpm_if.su ./Core/stm32wlxx_hal_msp.cyclo ./Core/stm32wlxx_hal_msp.d ./Core/stm32wlxx_hal_msp.o ./Core/stm32wlxx_hal_msp.su ./Core/stm32wlxx_it.cyclo ./Core/stm32wlxx_it.d ./Core/stm32wlxx_it.o ./Core/stm32wlxx_it.su ./Core/sys_app.cyclo ./Core/sys_app.d ./Core/sys_app.o ./Core/sys_app.su ./Core/sys_debug.cyclo ./Core/sys_debug.d ./Core/sys_debug.o ./Core/sys_debug.su ./Core/syscalls.cyclo ./Core/syscalls.d ./Core/syscalls.o ./Core/syscalls.su ./Core/sysmem.cyclo ./Core/sysmem.d ./Core/sysmem.o ./Core/sysmem.su ./Core/system_stm32wlxx.cyclo ./Core/system_stm32wlxx.d ./Core/system_stm32wlxx.o ./Core/system_stm32wlxx.su ./Core/timer_if.cyclo ./Core/timer_if.d ./Core/timer_if.o ./Core/timer_if.su

.PHONY: clean-Core

