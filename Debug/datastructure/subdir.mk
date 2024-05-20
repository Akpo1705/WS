################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../datastructure/BinarySearchTree.c \
../datastructure/Dictionary.c \
../datastructure/Entery.c \
../datastructure/LinkedList.c \
../datastructure/node.c \
../datastructure/queue.c 

C_DEPS += \
./datastructure/BinarySearchTree.d \
./datastructure/Dictionary.d \
./datastructure/Entery.d \
./datastructure/LinkedList.d \
./datastructure/node.d \
./datastructure/queue.d 

OBJS += \
./datastructure/BinarySearchTree.o \
./datastructure/Dictionary.o \
./datastructure/Entery.o \
./datastructure/LinkedList.o \
./datastructure/node.o \
./datastructure/queue.o 


# Each subdirectory must supply rules for building sources it contributes
datastructure/%.o: ../datastructure/%.c datastructure/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-datastructure

clean-datastructure:
	-$(RM) ./datastructure/BinarySearchTree.d ./datastructure/BinarySearchTree.o ./datastructure/Dictionary.d ./datastructure/Dictionary.o ./datastructure/Entery.d ./datastructure/Entery.o ./datastructure/LinkedList.d ./datastructure/LinkedList.o ./datastructure/node.d ./datastructure/node.o ./datastructure/queue.d ./datastructure/queue.o

.PHONY: clean-datastructure

