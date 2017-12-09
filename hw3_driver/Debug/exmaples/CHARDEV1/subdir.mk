################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../exmaples/CHARDEV1/chardev.c \
../exmaples/CHARDEV1/chardev.mod.c 

O_SRCS += \
../exmaples/CHARDEV1/chardev.mod.o \
../exmaples/CHARDEV1/chardev.o 

OBJS += \
./exmaples/CHARDEV1/chardev.o \
./exmaples/CHARDEV1/chardev.mod.o 

C_DEPS += \
./exmaples/CHARDEV1/chardev.d \
./exmaples/CHARDEV1/chardev.mod.d 


# Each subdirectory must supply rules for building sources it contributes
exmaples/CHARDEV1/%.o: ../exmaples/CHARDEV1/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


