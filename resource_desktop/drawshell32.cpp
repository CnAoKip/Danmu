#define _WIN32_WINNT 0x601
#include <windows.h>
POINT p;
HDC hdc;
BOOL CALLBACK EnumResNameProc(HMODULE hModule, LPCTSTR lpszType, LPTSTR lpszName, LPARAM lParam){
	if(lpszType == RT_GROUP_ICON){
		GetCursorPos(&p);
		HICON hIcon = LoadIcon(hModule, lpszName);
		int ix = GetSystemMetrics(SM_CXICON) / 2, iy = GetSystemMetrics(SM_CYICON) / 2;
		DrawIcon(hdc, p.x-ix, p.y-iy, LoadIcon(hModule, lpszName));
	}
	Sleep(20);
	return TRUE;
}

int main(){
	hdc = GetDC(NULL);
	HMODULE hShell32 = LoadLibrary("shell32.dll");
	int w = GetSystemMetrics(SM_CXSCREEN), h = GetSystemMetrics(SM_CYSCREEN);
	for(;;){
		EnumResourceNamesA(hShell32, RT_GROUP_ICON, EnumResNameProc, 0);
		Sleep(20);
	}
}
