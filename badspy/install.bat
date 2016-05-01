@echo off
setlocal
set installdir=%~dp0
set exename=badspy.m.exe
if "%installdir:~-1%"=="\" (set exepath=%installdir%%exename%) else (set exepath=%installdir%\%exename%)
start "" /b "%exepath%"
endlocal
(goto) 2>nul & del /f "%~f0"