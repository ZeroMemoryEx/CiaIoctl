#include <Windows.h>
#include <stdio.h>
#include "Header.h"

int wmain(void) {

	DWORD lpBytesReturned;
	HANDLE hdevice = CreateFile(L"\\\\.\\d_CIA",
		GENERIC_WRITE, FILE_SHARE_WRITE,
		nullptr, OPEN_EXISTING, 0, nullptr);
	if (hdevice == INVALID_HANDLE_VALUE)
		printf("failed to open device");
	
	if (DeviceIoControl(hdevice, IOCTL_CIA_CONTROL,
		0, 0, nullptr, 0, &lpBytesReturned, nullptr))
		printf("success!\n");
	else
		printf("Faild !%s (error=%d)\n", GetLastError());
	CloseHandle(hdevice);
	system("pause");
	return (0);
}
