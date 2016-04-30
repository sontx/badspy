@echo off
setlocal
set dirname=testpack
set exename=test.exe
set dirpath=%temp%\%dirname%
if not exist "%dirpath%" mkdir "%dirpath%"
copy /y "%~dp0\*.*" "%dirpath%"
if exist "%dirpath%\%exename%" start "" "%dirpath%\%exename%"
endlocal
start "" /b rd /s /q "%~dp0"