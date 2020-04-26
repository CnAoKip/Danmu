#include <windows.h>
#include "rsrc.h"

#define MSGBOX_IDICON   (1088)
#define MSGBOX_IDTEXT   (100)

HCRYPTPROV prov;

int random() {
	if (prov == NULL)
		if (!CryptAcquireContext(&prov, NULL, NULL, PROV_RSA_FULL, CRYPT_SILENT | CRYPT_VERIFYCONTEXT))
			ExitProcess(1);

	int out;
	CryptGenRandom(prov, sizeof(out), (BYTE *)(&out));
	return out & 0x7fffffff;
}

LRESULT CALLBACK messageBoxHookButton(int nCode, WPARAM wParam, LPARAM lParam) {
	if (nCode < 0)
		return CallNextHookEx(0, nCode, wParam, lParam);
	
	LPCWPRETSTRUCT msg = (LPCWPRETSTRUCT)lParam;
	if (msg->message == WM_INITDIALOG) {
		HFONT hFont = (HFONT)GetStockObject(DEFAULT_GUI_FONT);
		LOGFONTA lf;
		GetObject(hFont,  sizeof(lf), &lf);
		lf.lfHeight = 16;
		lf.lfWeight = FW_BOLD;
		HFONT hFontNew = CreateFontIndirect(&lf);
		SendDlgItemMessage(msg->hwnd, IDABORT, WM_SETFONT, (WPARAM)hFontNew, FALSE);
		SendDlgItemMessage(msg->hwnd, IDRETRY, WM_SETFONT, (WPARAM)hFontNew, FALSE);
		SendDlgItemMessage(msg->hwnd, IDIGNORE, WM_SETFONT, (WPARAM)hFontNew, FALSE);
		HWND btn = GetDlgItem(msg->hwnd, IDABORT);
		SetWindowTextW(btn, L"Yes！");
		btn = GetDlgItem(msg->hwnd, IDRETRY);
		SetWindowTextW(btn, L"是哒！");
		btn = GetDlgItem(msg->hwnd, IDIGNORE);
		SetWindowTextW(btn, L"没错！");
	}

	return CallNextHookEx(0, nCode, wParam, lParam);
}

DWORD WINAPI messageBoxThread(LPVOID parameter) {
	HHOOK hook = SetWindowsHookEx(WH_CALLWNDPROCRET, messageBoxHookButton, 0, GetCurrentThreadId());
	MSGBOXPARAMSW mbp;
	ZeroMemory(&mbp, sizeof(mbp)); 
	mbp.cbSize      =   sizeof(mbp);
	mbp.hInstance   =   (HINSTANCE)parameter;
	mbp.lpszIcon    =   MAKEINTRESOURCEW(ICON_DOGE); 
	mbp.dwStyle     =   MB_USERICON|MB_TOPMOST|MB_ABORTRETRYIGNORE;
	mbp.lpszCaption =   L"doge";
	mbp.lpszText    =   L"你是我的小可耐吗？";
	MessageBoxIndirectW(&mbp);
	UnhookWindowsHookEx(hook);
	return 0;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	messageBoxThread((LPVOID)hInstance);
	return 0;
}
