#include <Windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
using std::cout;
using std::endl;
#pragma warning(disable:6387)

int main(int argc, char * argv[])
{
	int pid;
	if (argc == 1)
	{
		printf("Usage:tasksuspend.exe <ProcessId>");
		system("pause>nul");
		ExitProcess(0);
	}
	pid = atoi(argv[1]);
	typedef NTSTATUS(*_NtSuspendProcess)(HANDLE hProcess);
	typedef NTSTATUS(*_NtResumeProcess)(HANDLE hProcess);
	_NtSuspendProcess NtSuspendProcess = (_NtSuspendProcess)GetProcAddress(GetModuleHandle(L"ntdll.dll"), "NtSuspendProcess");
	_NtResumeProcess NtResumeProcess = (_NtResumeProcess)GetProcAddress(GetModuleHandle(L"ntdll.dll"), "NtResumeProcess");
	HANDLE hProcess = OpenProcess(PROCESS_SUSPEND_RESUME, FALSE, pid);
	if (NULL == hProcess)
	{
		cout << "Error:Cannot open process, GetLastError return:" << GetLastError() << endl;
		system("pause>nul");
		ExitProcess(1);
	}
	NTSTATUS nSuspendCode = NtSuspendProcess(hProcess);
	if (NO_ERROR == nSuspendCode)
	{
		cout << "Successed to suspend process, Please press anykey to resume it.";
		system("pause>nul");
		cout << endl;
		NTSTATUS nResumeCode = NtResumeProcess(hProcess);
		if (NO_ERROR == nResumeCode)
		{
			cout << "Successed to resume process, Please press anykey to exit task.";
			system("pause>nul");
			ExitProcess(0);
		}
		else
		{
			cout << "Error:Resume NtStatus Code is:" << nResumeCode;
			system("pause>nul");
			ExitProcess(1);
		}
	}
	else
	{
		cout << "Error:Suspend NtStatus Code is:" << nSuspendCode;
		system("pause>nul");
		ExitProcess(1);
	}
	system("pause>nul");
	return 0;
}
