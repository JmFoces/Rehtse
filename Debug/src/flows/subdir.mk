################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/flows/Flow.cpp \
../src/flows/FlowTracker.cpp \
../src/flows/GenericFlow.cpp \
../src/flows/TCPFlow.cpp 

OBJS += \
./src/flows/Flow.o \
./src/flows/FlowTracker.o \
./src/flows/GenericFlow.o \
./src/flows/TCPFlow.o 

CPP_DEPS += \
./src/flows/Flow.d \
./src/flows/FlowTracker.d \
./src/flows/GenericFlow.d \
./src/flows/TCPFlow.d 


# Each subdirectory must supply rules for building sources it contributes
src/flows/%.o: ../src/flows/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -std=c++0x -DBOOST_ALL_DYN_LINK -I../include -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


