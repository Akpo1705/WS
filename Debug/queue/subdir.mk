################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../queue/queue.c \
../queue/queue_internal.c 

C_DEPS += \
./queue/queue.d \
./queue/queue_internal.d 

OBJS += \
./queue/queue.o \
./queue/queue_internal.o 


# Each subdirectory must supply rules for building sources it contributes
queue/%.o: ../queue/%.c queue/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-queue

clean-queue:
	-$(RM) ./queue/queue.d ./queue/queue.o ./queue/queue_internal.d ./queue/queue_internal.o

.PHONY: clean-queue

