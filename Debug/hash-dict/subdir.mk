################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../hash-dict/dict.c \
../hash-dict/hashutils.c \
../hash-dict/node.c \
../hash-dict/prime.c 

C_DEPS += \
./hash-dict/dict.d \
./hash-dict/hashutils.d \
./hash-dict/node.d \
./hash-dict/prime.d 

OBJS += \
./hash-dict/dict.o \
./hash-dict/hashutils.o \
./hash-dict/node.o \
./hash-dict/prime.o 


# Each subdirectory must supply rules for building sources it contributes
hash-dict/%.o: ../hash-dict/%.c hash-dict/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-hash-2d-dict

clean-hash-2d-dict:
	-$(RM) ./hash-dict/dict.d ./hash-dict/dict.o ./hash-dict/hashutils.d ./hash-dict/hashutils.o ./hash-dict/node.d ./hash-dict/node.o ./hash-dict/prime.d ./hash-dict/prime.o

.PHONY: clean-hash-2d-dict

