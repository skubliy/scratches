################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
/home/sergueik/w/ecl/libhrt/timer.c 

OBJS += \
./src/libhrt/timer.o 

C_DEPS += \
./src/libhrt/timer.d 


# Each subdirectory must supply rules for building sources it contributes
src/libhrt/timer.o: /home/sergueik/w/ecl/libhrt/timer.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -I/home/sergueik/w/ecl -I/home/sergueik/w/ecl/libhrt -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


