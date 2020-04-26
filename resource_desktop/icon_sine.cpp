#define _WIN32_WINNT 0x601
#include <Windows.h>
#include <tchar.h>
#include <stdio.h>
#include <math.h>
#include <libloaderapi.h>

int w, h, w2, h2;
int ix, times;
int dir = 1;

BOOL CALLBACK EnumResNameProc(HMODULE hModule, LPCTSTR lpszType, LPTSTR lpszName, LPARAM lParam){
	if(lpszType == RT_GROUP_ICON){
		HICON hIcon = LoadIcon(hModule, lpszName);
		DrawIcon(GetDC(NULL), ix, dir?((float*)lParam)[ix]/2:(h/2 - ((float*)lParam)[ix]), hIcon);
		DrawIcon(GetDC(NULL), ix, dir?h-(((float*)lParam)[ix]):((float*)lParam)[ix], hIcon);
	}
	ix += 16;
	if(ix > w) {
		ix = 0;
		dir = !dir;
		times++;
		if(times==2)return FALSE;
	}
	Sleep(20);
	return TRUE;
}

int main(){
	
	w = GetSystemMetrics(SM_CXSCREEN), h = GetSystemMetrics(SM_CYSCREEN);
	w2 = w / 2, h2 = h / 2;
	float *sines = (float *)LocalAlloc(LMEM_ZEROINIT, w * sizeof(float));
	HMODULE hShell32 = LoadLibrary("shell32.dll");
	for (int i = 0; i < w; i++) {
		sines[i] = sin(i / ((float)w)*3.14159) * 800.0;
	}
	
	for(int k = 1; k < 4; k++){
		dir = 1; ix = 0; times = 0;
		for (int i = 0; i < w; i++) {
			sines[i] = sines[i] / (float)k;
		}
		
		EnumResourceNamesA(hShell32, RT_GROUP_ICON, EnumResNameProc, (LPARAM)sines);
	}
	
	FreeLibrary(hShell32);
	
}
