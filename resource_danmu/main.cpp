#include <windows.h>
#include <stdlib.h>
#include <time.h>
#include "rsrc.h"
#include <commctrl.h>

#define KEY_DOWN(VK_NONAME) ((GetAsyncKeyState(VK_NONAME) & 0x8000) ? 1 : 0)
COLORREF RandRGB;
int x, minx;


CHAR szText[128];

DWORD TextReadSize;

VOID WINAPI NSInit(){
	HANDLE hFile = CreateFile("弹幕内容.txt", GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if(hFile == INVALID_HANDLE_VALUE){
		MessageBox(NULL, "无法读取弹幕内容!请确定“弹幕内容.txt”存在且不为空!", "错误", MB_ICONERROR);
		TerminateProcess(GetCurrentProcess(), 0); 
	} 
	
	else ReadFile(hFile, szText, sizeof(szText) - 1, &TextReadSize, NULL);
		
	if(TextReadSize == 0){
		MessageBox(NULL, "无法读取弹幕内容!请确定“弹幕内容.txt”存在且不为空!", "错误", MB_ICONERROR | MB_OK | MB_TOPMOST);
		TerminateProcess(GetCurrentProcess(), 0); 
	}
	
	if(TextReadSize > 24){
		MessageBox(NULL, "弹幕过长！弹幕内容最长24字节！", "错误", MB_ICONERROR | MB_OK | MB_TOPMOST);
		TerminateProcess(GetCurrentProcess(), 0); 
	}
	
	//MessageBox(NULL, szText, "Caption", MB_ICONINFORMATION | MB_OK | MB_TOPMOST);
	szText[TextReadSize] = 0;
	CloseHandle(hFile); 
}
const COLORREF c_rgclrPredef[] =
{
    RGB(0x00, 0x00, 0x00),          // 0 = black
    RGB(0x80, 0x00, 0x00),          // 1 = maroon
    RGB(0x00, 0x80, 0x00),          // 2 = green
    RGB(0x80, 0x80, 0x00),          // 3 = olive
    RGB(0x00, 0x00, 0x80),          // 4 = navy
    RGB(0x80, 0x00, 0x80),          // 5 = purple
    RGB(0x00, 0x80, 0x80),          // 6 = teal
    RGB(0x80, 0x80, 0x80),          // 7 = gray
    RGB(0xC0, 0xC0, 0xC0),          // 8 = silver
    RGB(0xFF, 0x00, 0x00),          // 9 = red
    RGB(0x00, 0xFF, 0x00),          // A = lime
    RGB(0xFF, 0xFF, 0x00),          // B = yellow
    RGB(0x00, 0x00, 0xFF),          // C = blue
    RGB(0xFF, 0x00, 0xFF),          // D = fuchsia
    RGB(0x00, 0xFF, 0xFF),          // E = cyan
    RGB(0xFF, 0xFF, 0xFF),          // F = white
};
const int c_rgclrPredef_size = sizeof(c_rgclrPredef)/sizeof(COLORREF);
LRESULT CALLBACK WndProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam) {
	switch(Message) {
		
		
		/* Upon destruction, tell the main thread to stop */
		case WM_DESTROY:case WM_CLOSE: {
			PostQuitMessage(0);
			break;
		}
		case WM_CREATE:{
			
			LPVOID lpRes;
      		HANDLE  hRes;
      		HRSRC hResInfo;
      		HMODULE hInst = GetModuleHandle(NULL);
      		hResInfo = FindResource(hInst, MAKEINTRESOURCE(SND_FU), RT_RCDATA);
      
      		hRes = LoadResource(hInst, hResInfo);
      		lpRes = LockResource(hRes);
      		if (lpRes != NULL) 
           		sndPlaySoundW((wchar_t*)lpRes, SND_MEMORY | SND_ASYNC | SND_NODEFAULT);
			SetLayeredWindowAttributes(hwnd, RGB(255, 255, 255), 255, LWA_ALPHA | LWA_COLORKEY);

			SetTimer(hwnd, 1, 56, NULL); 
			break;
		}
		
		case WM_TIMER:{
			if(KEY_DOWN('C') && KEY_DOWN('A') && KEY_DOWN('K')){
				KillTimer(hwnd, 1); 
				PostQuitMessage(0);
			}
			
			x-=5;
			if(x <= minx) PostQuitMessage(0);
			SetWindowPos(hwnd, NULL, x, 0, 0, 0, SWP_ASYNCWINDOWPOS|SWP_NOSIZE);
			break;
		}
		case WM_PAINT:{
			PAINTSTRUCT ps;
			
			
			
			HDC hdc = BeginPaint(hwnd, &ps);
			
			//PrintWindow(GetShellWindow(), hdc, 0); 
			
			LOGFONT   fontRect;   
      		memset(&fontRect,0,sizeof(LOGFONT));   
      		fontRect.lfHeight = GetSystemMetrics(SM_CXICON);     //字体的高度   
      		fontRect.lfWeight = FW_HEAVY;//字体的粗细   
      		lstrcpy(fontRect.lfFaceName,"黑体");   
    
     		HFONT hFont = CreateFontIndirect(&fontRect);   //创建字体   
      		HFONT hOld = (HFONT)SelectObject(hdc,hFont);//引用上面的字体
			
			SelectObject(hdc,GetStockObject(NULL_PEN));
			::SetTextColor(hdc,RandRGB);  
      		::SetBkMode(hdc, TRANSPARENT); 
      		RECT rc; 
			GetClientRect(hwnd, &rc);
        	DrawTextA(hdc, szText,
            lstrlenA(szText), &rc, DT_LEFT | DT_NOCLIP | DT_WORDBREAK | DT_CALCRECT);
            SetTextColor(hdc, RGB(40, 40, 40));
            
            minx = -(rc.right-rc.left);
            
            TextOutA(hdc, rc.left, rc.top, szText,
            lstrlenA(szText));
			GrayStringA(hdc, CreateSolidBrush(RandRGB), NULL, (LPARAM)szText, 0, rc.left, rc.top, rc.right-rc.left, rc.bottom-rc.top);
			EndPaint(hwnd, &ps);
			
			break;
		}
		
		/* All other messages (a lot of them) are processed using default procedures */
		default:
			return DefWindowProc(hwnd, Message, wParam, lParam);
	}
	return 0;
}

/* The 'main' function of Win32 GUI programs: this is where execution starts */
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	NSInit();
	srand(time(0));
	RandRGB = c_rgclrPredef[rand() % c_rgclrPredef_size];
	x = GetSystemMetrics(SM_CXSCREEN); 
	
	WNDCLASSEX wc; /* A properties struct of our window */
	HWND hwnd; /* A 'HANDLE', hence the H, or a pointer to our window */
	MSG msg; /* A temporary location for all messages */

	/* zero out the struct and set the stuff we want to modify */
	memset(&wc,0,sizeof(wc));
	wc.cbSize		 = sizeof(WNDCLASSEX);
	wc.lpfnWndProc	 = WndProc; /* This is where we will send messages to */
	wc.hInstance	 = hInstance;
	wc.hCursor		 = LoadCursor(NULL, IDC_ARROW);
	
	/* White, COLOR_WINDOW is just a #define for a system color, try Ctrl+Clicking it */
	wc.hbrBackground = CreateSolidBrush(RGB(255, 255, 255));
	wc.lpszClassName = "WindowClass";
	wc.hIcon		 = ExtractIcon(hInstance, "shell32.dll", 1); /* Load a standard icon */
	wc.hIconSm		 = ExtractIcon(hInstance, "shell32.dll", 1); /* use the name "A" to use the project icon */

	if(!RegisterClassEx(&wc)) {
		MessageBox(NULL, "Window Registration Failed!","Error!",MB_ICONEXCLAMATION|MB_OK);
		return 0;
	}
	hwnd = CreateWindowEx(WS_EX_TOPMOST | WS_EX_LAYERED,"WindowClass","void_setup",WS_POPUP | WS_VISIBLE,
		0, /* x */
		x, /* y */
		400, /* width */
		400, /* height */
		NULL,NULL,hInstance,NULL);

	if(hwnd == NULL) {
		MessageBox(NULL, "Window Creation Failed!","Error!",MB_ICONEXCLAMATION|MB_OK);
		return 0;
	}

	/*
		This is the heart of our program where all input is processed and 
		sent to WndProc. Note that GetMessage blocks code flow until it receives something, so
		this loop will not produce unreasonably high CPU usage
	*/
	while(GetMessage(&msg, NULL, 0, 0) > 0) { /* If no error is received... */
		TranslateMessage(&msg); /* Translate key codes to chars if present */
		DispatchMessage(&msg); /* Send it to WndProc */
	}
	return msg.wParam;
}


