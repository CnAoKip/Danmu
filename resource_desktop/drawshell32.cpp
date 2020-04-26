#define _WIN32_WINNT 0x601
#include <windows.h>
//#include <iostream>
//using namespace std;
POINT p;
HDC hdc;
int x0, y0, x1, y1, x2, y2;

BOOL CALLBACK EnumResNameProc(HMODULE hModule, LPCTSTR lpszType, LPTSTR lpszName, LPARAM lParam) {
	GetCursorPos(&p);
	HICON hIcon = LoadIcon(hModule, lpszName);
	x2 = int(double(p.x) / x1 * x0), y2 = int(double(p.y) / y1 * y0);
	int ix = GetSystemMetrics(SM_CXICON) / 2, iy = GetSystemMetrics(SM_CYICON) / 2;

	DrawIcon(hdc, x2 - ix, y2 - iy, LoadIcon(hModule, lpszName));
//	cout << x0 << "\t" << y0 << "\t" << x1 << "\t" << y1 << "\t" << x2 << "\t" << y2 << "\t" << p.x << "\t" << p.y << "\n";
	Sleep(10);
	return TRUE;
}

int main() {
	hdc = GetDC(NULL);
	HMODULE hShell32 = LoadLibrary("shell32.dll");
	x1 = GetSystemMetrics(SM_CXSCREEN), y1 = GetSystemMetrics(SM_CYSCREEN);
	HDC hdc1 = GetDC(NULL);
	x0 = GetDeviceCaps(hdc1, DESKTOPHORZRES);
	y0 = GetDeviceCaps(hdc1, DESKTOPVERTRES);
	ReleaseDC(NULL, hdc1);
	system("pause");
	for (;;) {
		EnumResourceNamesA(hShell32, RT_GROUP_ICON, EnumResNameProc, 0);
		Sleep(1);
	}
}
