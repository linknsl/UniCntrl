################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/m_usb_charging.c 

OBJS += \
./src/m_usb_charging.o 

C_DEPS += \
./src/m_usb_charging.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	arm-linux-gnueabihf-gcc -I"/media/link/workspace/new_workspace/UniCntrl/nativeutils/h" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


