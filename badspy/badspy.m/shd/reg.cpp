#include "reg.h"

bool Reg::write_value_sz(const char * key, const char * value)
{
	return RegSetValueExA(hkey, key, 0, REG_SZ, (LPBYTE)value, strlen(value) * sizeof(char)) == ERROR_SUCCESS;
}

bool Reg::read_value_sz(const char * key, char * buffer)
{
	DWORD length = MAX_PATH;
	DWORD dwtype = REG_SZ;
	return RegQueryValueExA(hkey, key, NULL, &dwtype, (LPBYTE)&buffer, &length) != ERROR_SUCCESS;
}

Reg::Reg(HKEY root, const char * subkey, bool writable)
{
	if (RegOpenKeyExA(root, subkey, 0, KEY_READ | (writable ? KEY_WRITE : 0), &hkey) != ERROR_SUCCESS)
	{
		LOG_E("Unable to open registry key");
		throw GetLastError();
	}
}

Reg::~Reg()
{
	RegCloseKey(hkey);
}
