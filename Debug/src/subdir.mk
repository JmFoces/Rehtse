################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/NFQueue.cpp \
../src/UserInterface.cpp \
../src/main.cpp \
../src/misc.cpp 

OBJS += \
./src/NFQueue.o \
./src/UserInterface.o \
./src/main.o \
./src/misc.o 

CPP_DEPS += \
./src/NFQueue.d \
./src/UserInterface.d \
./src/main.d \
./src/misc.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -std=c++0x -DBOOST_ALL_DYN_LINK -I"/home/xshell/workspace-neon/RehtSe/include" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


