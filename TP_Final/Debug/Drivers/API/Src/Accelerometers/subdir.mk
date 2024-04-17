################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/API/Src/Accelerometers/API_adxl345.c \
../Drivers/API/Src/Accelerometers/API_adxl345_sensitivity.c 

OBJS += \
./Drivers/API/Src/Accelerometers/API_adxl345.o \
./Drivers/API/Src/Accelerometers/API_adxl345_sensitivity.o 

C_DEPS += \
./Drivers/API/Src/Accelerometers/API_adxl345.d \
./Drivers/API/Src/Accelerometers/API_adxl345_sensitivity.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/API/Src/Accelerometers/%.o Drivers/API/Src/Accelerometers/%.su Drivers/API/Src/Accelerometers/%.cyclo: ../Drivers/API/Src/Accelerometers/%.c Drivers/API/Src/Accelerometers/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DSTM32 -DSTM32F429ZITx -DSTM32F4 -DNUCLEO_F429ZI -DUSE_HAL_DRIVER -DSTM32F429xx -c -I../Inc -I"/home/lpmeoli/Especializacion/PdM_workspace/TP_Final/Drivers/STM32F4xx_HAL_Driver/Inc" -I"/home/lpmeoli/Especializacion/PdM_workspace/TP_Final/Drivers/CMSIS/Include" -I"/home/lpmeoli/Especializacion/PdM_workspace/TP_Final/Drivers/CMSIS/Device/ST/STM32F4xx/Include" -I"/home/lpmeoli/Especializacion/PdM_workspace/TP_Final/Drivers/BSP/STM32F4xx_Nucleo_144" -I"/home/lpmeoli/Especializacion/PdM_workspace/TP_Final/Drivers/Core/Inc" -I"/home/lpmeoli/Especializacion/PdM_workspace/TP_Final/Drivers/API" -I"/home/lpmeoli/Especializacion/PdM_workspace/TP_Final/Drivers/API/Inc" -I"/home/lpmeoli/Especializacion/PdM_workspace/TP_Final/Drivers/API/Src" -I"/home/lpmeoli/Especializacion/PdM_workspace/TP_Final/Drivers/API/Inc/Utils" -I"/home/lpmeoli/Especializacion/PdM_workspace/TP_Final/Drivers/API/Src/Utils" -I"/home/lpmeoli/Especializacion/PdM_workspace/TP_Final/Drivers/API/Inc/Displays" -I"/home/lpmeoli/Especializacion/PdM_workspace/TP_Final/Drivers/API/Src/Displays" -I"/home/lpmeoli/Especializacion/PdM_workspace/TP_Final/Drivers/API/Inc/Accelerometers" -I"/home/lpmeoli/Especializacion/PdM_workspace/TP_Final/Drivers/API/Src/Accelerometers" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Drivers-2f-API-2f-Src-2f-Accelerometers

clean-Drivers-2f-API-2f-Src-2f-Accelerometers:
	-$(RM) ./Drivers/API/Src/Accelerometers/API_adxl345.cyclo ./Drivers/API/Src/Accelerometers/API_adxl345.d ./Drivers/API/Src/Accelerometers/API_adxl345.o ./Drivers/API/Src/Accelerometers/API_adxl345.su ./Drivers/API/Src/Accelerometers/API_adxl345_sensitivity.cyclo ./Drivers/API/Src/Accelerometers/API_adxl345_sensitivity.d ./Drivers/API/Src/Accelerometers/API_adxl345_sensitivity.o ./Drivers/API/Src/Accelerometers/API_adxl345_sensitivity.su

.PHONY: clean-Drivers-2f-API-2f-Src-2f-Accelerometers

