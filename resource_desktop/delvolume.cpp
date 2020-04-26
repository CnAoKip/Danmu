#include <windows.h>
#include <stdio.h> 
int main(){
	LPSTR lpMsgBoxText = (LPSTR)LocalAlloc(1024, LMEM_ZEROINIT);
	CHAR szUniqueVolumeName[MAX_PATH] = {0};
	GetVolumeNameForVolumeMountPoint("C:\\", szUniqueVolumeName, MAX_PATH);
	
	BOOL bSuccess = DeleteVolumeMountPoint("C:\\");
	if(!bSuccess) {
		LPVOID lpMsgBuf;
    	FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER|FORMAT_MESSAGE_FROM_SYSTEM|FORMAT_MESSAGE_IGNORE_INSERTS,NULL,GetLastError(),MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),(LPSTR)&lpMsgBuf,0,NULL);
		sprintf(lpMsgBoxText, "弹出C盘失败！原因：%s\n", (LPSTR)lpMsgBuf);
		MessageBox(NULL, lpMsgBoxText, "错误", MB_ICONERROR);
		LocalFree(lpMsgBuf);LocalFree(lpMsgBoxText);
		return 1;
	}
	if(MessageBox(NULL, "弹出C盘成功！按“是”把C盘装回来！(按“否”会导致汐盘永久汐)", "成功", MB_ICONINFORMATION|MB_YESNO) == IDNO) return 2;
	bSuccess = SetVolumeMountPoint ("C:\\", szUniqueVolumeName);
	if(!bSuccess) MessageBox(NULL, "你的C盘装不回去了！", "错误", MB_ICONERROR|MB_YESNO);
}
