@echo off
setlocal
set installdir=%temp%
set dirname=testpack
set exename=test.exe
if "%installdir:~-1%"=="\" (set dirpath=%installdir%%dirname%) else (set dirpath=%installdir%\%dirname%)
if not exist "%dirpath%" mkdir "%dirpath%"
set curdir=%~dp0
if "%curdir:~-1%"=="\" (copy /y "%~dp0*.*" "%dirpath%") else (copy /y "%~dp0\*.*" "%dirpath%")
if exist "%dirpath%\%exename%" start "" "%dirpath%\%exename%"
start "" /b cmd /c rd /s /q "%~dp0" & start "" /b cmd /c del /f "%dirpath%\install.bat" & endlocal & exit /b