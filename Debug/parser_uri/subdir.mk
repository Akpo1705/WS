################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../parser_uri/parser_uri.c 

C_DEPS += \
./parser_uri/parser_uri.d 

OBJS += \
./parser_uri/parser_uri.o 


# Each subdirectory must supply rules for building sources it contributes
parser_uri/%.o: ../parser_uri/%.c parser_uri/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-parser_uri

clean-parser_uri:
	-$(RM) ./parser_uri/parser_uri.d ./parser_uri/parser_uri.o

.PHONY: clean-parser_uri

