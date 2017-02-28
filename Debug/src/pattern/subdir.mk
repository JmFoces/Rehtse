################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/pattern/Pattern.cpp \
../src/pattern/Scanner.cpp 

OBJS += \
./src/pattern/Pattern.o \
./src/pattern/Scanner.o 

CPP_DEPS += \
./src/pattern/Pattern.d \
./src/pattern/Scanner.d 


# Each subdirectory must supply rules for building sources it contributes
src/pattern/%.o: ../src/pattern/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -std=c++0x -DBOOST_ALL_DYN_LINK -I../include -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


