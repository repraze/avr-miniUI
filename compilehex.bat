@echo off

SET BOARD=atmega644p
SET CLOCK=12000000

set fName=%1
if '%1' equ '' set /p fName=Input file name:

for /f "tokens=* delims= " %%F in ('echo %fName%') do (	
	@avr-gcc -mmcu=%BOARD% -DF_CPU=%CLOCK% -Os %%~pdnxF -o %%~pdnF.elf
	@avr-objcopy -O ihex %%~pdnF.elf %%~pdnF.hex
)
