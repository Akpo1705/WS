################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../avltree.c \
../main.c \
../module.c \
../moduleconf.c \
../moduleconfparser.c \
../ws_file.c \
../ws_handshake.c 

C_DEPS += \
./avltree.d \
./main.d \
./module.d \
./moduleconf.d \
./moduleconfparser.d \
./ws_file.d \
./ws_handshake.d 

OBJS += \
./avltree.o \
./main.o \
./module.o \
./moduleconf.o \
./moduleconfparser.o \
./ws_file.o \
./ws_handshake.o 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.c subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean--2e-

clean--2e-:
	-$(RM) ./avltree.d ./avltree.o ./main.d ./main.o ./module.d ./module.o ./moduleconf.d ./moduleconf.o ./moduleconfparser.d ./moduleconfparser.o ./ws_file.d ./ws_file.o ./ws_handshake.d ./ws_handshake.o

.PHONY: clean--2e-

