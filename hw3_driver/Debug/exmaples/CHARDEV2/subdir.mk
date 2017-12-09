################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../exmaples/CHARDEV2/chardev.c \
../exmaples/CHARDEV2/user_chardev.c 

OBJS += \
./exmaples/CHARDEV2/chardev.o \
./exmaples/CHARDEV2/user_chardev.o 

C_DEPS += \
./exmaples/CHARDEV2/chardev.d \
./exmaples/CHARDEV2/user_chardev.d 


# Each subdirectory must supply rules for building sources it contributes
exmaples/CHARDEV2/%.o: ../exmaples/CHARDEV2/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


