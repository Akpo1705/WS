################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include queue/subdir.mk
-include parser_uri/subdir.mk
-include networking/subdir.mk
-include hash-dict/subdir.mk
-include datastructure/subdir.mk
-include subdir.mk
ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

OPTIONAL_TOOL_DEPS := \
$(wildcard ../makefile.defs) \
$(wildcard ../makefile.init) \
$(wildcard ../makefile.targets) \


BUILD_ARTIFACT_NAME := WS
BUILD_ARTIFACT_EXTENSION :=
BUILD_ARTIFACT_PREFIX :=
BUILD_ARTIFACT := $(BUILD_ARTIFACT_PREFIX)$(BUILD_ARTIFACT_NAME)$(if $(BUILD_ARTIFACT_EXTENSION),.$(BUILD_ARTIFACT_EXTENSION),)

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all: main-build

# Main-build Target
main-build: WS

# Tool invocations
WS: $(OBJS) $(USER_OBJS) makefile $(OPTIONAL_TOOL_DEPS)
	@echo 'Building target: $@'
	@echo 'Invoking: GCC C Linker'
	gcc -L"/home/alex/eclipse-workspace/WS/lib" -o "WS" $(OBJS) $(USER_OBJS) $(LIBS) -ltransport-64 -lm -lptoiface-64 -lcfgparser-64
	@echo 'Finished building target: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) WS
	-@echo ' '

.PHONY: all clean dependents main-build

-include ../makefile.targets
