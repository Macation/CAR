################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/AppSw/Tricore/User/CircularBuffer.c \
../src/AppSw/Tricore/User/Element_handle.c \
../src/AppSw/Tricore/User/FuzzyPID.c \
../src/AppSw/Tricore/User/LQ_ImageProcess.c \
../src/AppSw/Tricore/User/LQ_Inductor.c \
../src/AppSw/Tricore/User/LQ_MotorServo.c \
../src/AppSw/Tricore/User/LQ_PID.c \
../src/AppSw/Tricore/User/My_Button.c \
../src/AppSw/Tricore/User/My_Control_Code.c \
../src/AppSw/Tricore/User/My_Key.c \
../src/AppSw/Tricore/User/My_UART.c \
../src/AppSw/Tricore/User/My_menu.c \
../src/AppSw/Tricore/User/image1.c \
../src/AppSw/Tricore/User/swj.c 

COMPILED_SRCS += \
./src/AppSw/Tricore/User/CircularBuffer.src \
./src/AppSw/Tricore/User/Element_handle.src \
./src/AppSw/Tricore/User/FuzzyPID.src \
./src/AppSw/Tricore/User/LQ_ImageProcess.src \
./src/AppSw/Tricore/User/LQ_Inductor.src \
./src/AppSw/Tricore/User/LQ_MotorServo.src \
./src/AppSw/Tricore/User/LQ_PID.src \
./src/AppSw/Tricore/User/My_Button.src \
./src/AppSw/Tricore/User/My_Control_Code.src \
./src/AppSw/Tricore/User/My_Key.src \
./src/AppSw/Tricore/User/My_UART.src \
./src/AppSw/Tricore/User/My_menu.src \
./src/AppSw/Tricore/User/image1.src \
./src/AppSw/Tricore/User/swj.src 

C_DEPS += \
./src/AppSw/Tricore/User/CircularBuffer.d \
./src/AppSw/Tricore/User/Element_handle.d \
./src/AppSw/Tricore/User/FuzzyPID.d \
./src/AppSw/Tricore/User/LQ_ImageProcess.d \
./src/AppSw/Tricore/User/LQ_Inductor.d \
./src/AppSw/Tricore/User/LQ_MotorServo.d \
./src/AppSw/Tricore/User/LQ_PID.d \
./src/AppSw/Tricore/User/My_Button.d \
./src/AppSw/Tricore/User/My_Control_Code.d \
./src/AppSw/Tricore/User/My_Key.d \
./src/AppSw/Tricore/User/My_UART.d \
./src/AppSw/Tricore/User/My_menu.d \
./src/AppSw/Tricore/User/image1.d \
./src/AppSw/Tricore/User/swj.d 

OBJS += \
./src/AppSw/Tricore/User/CircularBuffer.o \
./src/AppSw/Tricore/User/Element_handle.o \
./src/AppSw/Tricore/User/FuzzyPID.o \
./src/AppSw/Tricore/User/LQ_ImageProcess.o \
./src/AppSw/Tricore/User/LQ_Inductor.o \
./src/AppSw/Tricore/User/LQ_MotorServo.o \
./src/AppSw/Tricore/User/LQ_PID.o \
./src/AppSw/Tricore/User/My_Button.o \
./src/AppSw/Tricore/User/My_Control_Code.o \
./src/AppSw/Tricore/User/My_Key.o \
./src/AppSw/Tricore/User/My_UART.o \
./src/AppSw/Tricore/User/My_menu.o \
./src/AppSw/Tricore/User/image1.o \
./src/AppSw/Tricore/User/swj.o 


# Each subdirectory must supply rules for building sources it contributes
src/AppSw/Tricore/User/%.src: ../src/AppSw/Tricore/User/%.c src/AppSw/Tricore/User/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: TASKING C/C++ Compiler'
	cctc -cs --dep-file="$(basename $@).d" --misrac-version=2004 -D__CPU__=tc26xb "-fD:/1Project/project/BBBCAR-V3最新能用/Debug/TASKING_C_C___Compiler-Include_paths__-I_.opt" --iso=99 --c++14 --language=+volatile --exceptions --anachronisms --fp-model=3 -O0 --tradeoff=4 --compact-max-size=200 -g -Wc-w544 -Wc-w557 -Ctc26xb -Y0 -N0 -Z0 -o "$@" "$<" && \
	if [ -f "$(basename $@).d" ]; then sed.exe -r  -e 's/\b(.+\.o)\b/src\/AppSw\/Tricore\/User\/\1/g' -e 's/\\/\//g' -e 's/\/\//\//g' -e 's/"//g' -e 's/([a-zA-Z]:\/)/\L\1/g' -e 's/\d32:/@TARGET_DELIMITER@/g; s/\\\d32/@ESCAPED_SPACE@/g; s/\d32/\\\d32/g; s/@ESCAPED_SPACE@/\\\d32/g; s/@TARGET_DELIMITER@/\d32:/g' "$(basename $@).d" > "$(basename $@).d_sed" && cp "$(basename $@).d_sed" "$(basename $@).d" && rm -f "$(basename $@).d_sed" 2>/dev/null; else echo 'No dependency file to process';fi
	@echo 'Finished building: $<'
	@echo ' '

src/AppSw/Tricore/User/%.o: ./src/AppSw/Tricore/User/%.src src/AppSw/Tricore/User/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: TASKING Assembler'
	astc -Og -Os --no-warnings= --error-limit=42 -o  "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-src-2f-AppSw-2f-Tricore-2f-User

clean-src-2f-AppSw-2f-Tricore-2f-User:
	-$(RM) ./src/AppSw/Tricore/User/CircularBuffer.d ./src/AppSw/Tricore/User/CircularBuffer.o ./src/AppSw/Tricore/User/CircularBuffer.src ./src/AppSw/Tricore/User/Element_handle.d ./src/AppSw/Tricore/User/Element_handle.o ./src/AppSw/Tricore/User/Element_handle.src ./src/AppSw/Tricore/User/FuzzyPID.d ./src/AppSw/Tricore/User/FuzzyPID.o ./src/AppSw/Tricore/User/FuzzyPID.src ./src/AppSw/Tricore/User/LQ_ImageProcess.d ./src/AppSw/Tricore/User/LQ_ImageProcess.o ./src/AppSw/Tricore/User/LQ_ImageProcess.src ./src/AppSw/Tricore/User/LQ_Inductor.d ./src/AppSw/Tricore/User/LQ_Inductor.o ./src/AppSw/Tricore/User/LQ_Inductor.src ./src/AppSw/Tricore/User/LQ_MotorServo.d ./src/AppSw/Tricore/User/LQ_MotorServo.o ./src/AppSw/Tricore/User/LQ_MotorServo.src ./src/AppSw/Tricore/User/LQ_PID.d ./src/AppSw/Tricore/User/LQ_PID.o ./src/AppSw/Tricore/User/LQ_PID.src ./src/AppSw/Tricore/User/My_Button.d ./src/AppSw/Tricore/User/My_Button.o ./src/AppSw/Tricore/User/My_Button.src ./src/AppSw/Tricore/User/My_Control_Code.d ./src/AppSw/Tricore/User/My_Control_Code.o ./src/AppSw/Tricore/User/My_Control_Code.src ./src/AppSw/Tricore/User/My_Key.d ./src/AppSw/Tricore/User/My_Key.o ./src/AppSw/Tricore/User/My_Key.src ./src/AppSw/Tricore/User/My_UART.d ./src/AppSw/Tricore/User/My_UART.o ./src/AppSw/Tricore/User/My_UART.src ./src/AppSw/Tricore/User/My_menu.d ./src/AppSw/Tricore/User/My_menu.o ./src/AppSw/Tricore/User/My_menu.src ./src/AppSw/Tricore/User/image1.d ./src/AppSw/Tricore/User/image1.o ./src/AppSw/Tricore/User/image1.src ./src/AppSw/Tricore/User/swj.d ./src/AppSw/Tricore/User/swj.o ./src/AppSw/Tricore/User/swj.src

.PHONY: clean-src-2f-AppSw-2f-Tricore-2f-User

