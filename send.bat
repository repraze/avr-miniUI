@echo off

SET TYPE=m644p

set fName=%1
if '%1' equ '' set /p fName=Input file name:

@avrdude -c usbasp -p %TYPE% -U flash:w:%fName%