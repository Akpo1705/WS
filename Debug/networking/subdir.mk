################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../networking/HTTPRequest.c \
../networking/server.c 

C_DEPS += \
./networking/HTTPRequest.d \
./networking/server.d 

OBJS += \
./networking/HTTPRequest.o \
./networking/server.o 


# Each subdirectory must supply rules for building sources it contributes
networking/%.o: ../networking/%.c networking/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-networking

clean-networking:
	-$(RM) ./networking/HTTPRequest.d ./networking/HTTPRequest.o ./networking/server.d ./networking/server.o

.PHONY: clean-networking

