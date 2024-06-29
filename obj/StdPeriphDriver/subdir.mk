################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../StdPeriphDriver/CH58x_adc.c \
../StdPeriphDriver/CH58x_clk.c \
../StdPeriphDriver/CH58x_flash.c \
../StdPeriphDriver/CH58x_gpio.c \
../StdPeriphDriver/CH58x_i2c.c \
../StdPeriphDriver/CH58x_pwr.c \
../StdPeriphDriver/CH58x_spi1.c \
../StdPeriphDriver/CH58x_sys.c \
../StdPeriphDriver/CH58x_uart0.c \
../StdPeriphDriver/CH58x_uart1.c \
../StdPeriphDriver/CH58x_usb2dev.c \
../StdPeriphDriver/CH58x_usb2hostBase.c 

OBJS += \
./StdPeriphDriver/CH58x_adc.o \
./StdPeriphDriver/CH58x_clk.o \
./StdPeriphDriver/CH58x_flash.o \
./StdPeriphDriver/CH58x_gpio.o \
./StdPeriphDriver/CH58x_i2c.o \
./StdPeriphDriver/CH58x_pwr.o \
./StdPeriphDriver/CH58x_spi1.o \
./StdPeriphDriver/CH58x_sys.o \
./StdPeriphDriver/CH58x_uart0.o \
./StdPeriphDriver/CH58x_uart1.o \
./StdPeriphDriver/CH58x_usb2dev.o \
./StdPeriphDriver/CH58x_usb2hostBase.o 

C_DEPS += \
./StdPeriphDriver/CH58x_adc2.d \
./StdPeriphDriver/CH58x_clk.d \
./StdPeriphDriver/CH58x_flash.d \
./StdPeriphDriver/CH58x_gpio.d \
./StdPeriphDriver/CH58x_i2c.d \
./StdPeriphDriver/CH58x_pwr.d \
./StdPeriphDriver/CH58x_spi1.d \
./StdPeriphDriver/CH58x_sys.d \
./StdPeriphDriver/CH58x_uart0.d \
./StdPeriphDriver/CH58x_uart1.d \
./StdPeriphDriver/CH58x_usb2dev.d \
./StdPeriphDriver/CH58x_usb2hostBase.d 


# Each subdirectory must supply rules for building sources it contributes
StdPeriphDriver/CH58x_adc.o: /home/flinux/Downloads/ch583-main/EVT/EXAM/my_code_new/StdPeriphDriver/CH58x_adc.c
	@echo 'Building file: $<'
	@echo 'Invoking: GNU RISC-V Cross C Compiler'
	riscv-none-embed-gcc -march=rv32imac -mabi=ilp32 -mcmodel=medany -msmall-data-limit=8 -mno-save-restore -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common  -g -DDEBUG=1 -I"../StdPeriphDriver/inc" -I/home/flinux/Downloads/ch583-main/EVT/EXAM/my_code_new/inc -I"../RVMSIS" -std=gnu99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

StdPeriphDriver/%.o: ../StdPeriphDriver/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GNU RISC-V Cross C Compiler'
	riscv-none-embed-gcc -march=rv32imac -mabi=ilp32 -mcmodel=medany -msmall-data-limit=8 -mno-save-restore -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common  -g -DDEBUG=1 -I"../StdPeriphDriver/inc" -I/home/flinux/Downloads/ch583-main/EVT/EXAM/my_code_new/inc -I"../RVMSIS" -std=gnu99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


