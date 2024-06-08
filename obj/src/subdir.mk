################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/AHT.c \
../src/Main.c \
../src/app_i2c.c 

OBJS += \
./src/AHT.o \
./src/Main.o \
./src/app_i2c.o 

C_DEPS += \
./src/AHT.d \
./src/Main.d \
./src/app_i2c.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GNU RISC-V Cross C Compiler'
	riscv-none-embed-gcc -march=rv32imac -mabi=ilp32 -mcmodel=medany -msmall-data-limit=8 -mno-save-restore -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common  -g -DDEBUG=1 -I"../StdPeriphDriver/inc" -I/home/flinux/Downloads/ch583-main/EVT/EXAM/my_code_new/inc -I"../RVMSIS" -std=gnu99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


