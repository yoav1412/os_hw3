################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../kermod/message_slot.c \
../kermod/message_slot.mod.c 

O_SRCS += \
../kermod/message_slot.mod.o 

OBJS += \
./kermod/message_slot.o \
./kermod/message_slot.mod.o 

C_DEPS += \
./kermod/message_slot.d \
./kermod/message_slot.mod.d 


# Each subdirectory must supply rules for building sources it contributes
kermod/%.o: ../kermod/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


