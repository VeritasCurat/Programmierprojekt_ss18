################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../nichtbenutzt/adjazensliste.c 

OBJS += \
./nichtbenutzt/adjazensliste.o 

C_DEPS += \
./nichtbenutzt/adjazensliste.d 


# Each subdirectory must supply rules for building sources it contributes
nichtbenutzt/%.o: ../nichtbenutzt/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


