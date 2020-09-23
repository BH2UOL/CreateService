#include <Windows.h>
#include <Shlwapi.h>
#include <stdio.h>
#include <tchar.h>
#pragma comment(lib, "Shlwapi.lib")
static  char* szName;

// 0 ���ط���    1 ��������    2 ֹͣ����    3 ɾ������
BOOL SystemServiceOperate(char* lpszDriverPath, int iOperateType)
{
	BOOL bRet = TRUE;
	//char szName[MAX_PATH] = { 0 };

	//lstrcpy(szName, lpszDriverPath);
	// ���˵��ļ�Ŀ¼����ȡ�ļ���
	//PathStripPath(szName);

	SC_HANDLE shSCManager = NULL, shService = NULL;
	SERVICE_STATUS sStatus;
	DWORD dwErrorCode = 0;

	// �򿪷�����ƹ��������ݿ�
	shSCManager = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);

	if (0 != iOperateType)
	{
		// ��һ���Ѿ����ڵķ���
		shService = OpenService(shSCManager, szName, SERVICE_ALL_ACCESS);
		if (!shService)
		{
			CloseServiceHandle(shSCManager);
			shSCManager = NULL;
			return FALSE;
		}
	}

	switch (iOperateType)
	{
	case 0:
	{
		// ��������
		// SERVICE_AUTO_START   ��ϵͳ�Զ�����
		// SERVICE_DEMAND_START �ֶ�����
		shService = CreateService(shSCManager, szName, szName,
			SERVICE_ALL_ACCESS,
			SERVICE_WIN32_OWN_PROCESS | SERVICE_INTERACTIVE_PROCESS,
			SERVICE_AUTO_START,
			SERVICE_ERROR_NORMAL,
			lpszDriverPath, NULL, NULL, NULL, NULL, NULL);
		break;
	}
	case 1:
	{
		// ��������
		StartService(shService, 0, NULL);
		break;
	}
	case 2:
	{
		// ֹͣ����
		ControlService(shService, SERVICE_CONTROL_STOP, &sStatus);
		break;
	}
	case 3:
	{
		// ɾ������
		DeleteService(shService);
		break;
	}
	default:
		break;
	}
	// �رվ��

	CloseServiceHandle(shService);
	CloseServiceHandle(shSCManager);

	return TRUE;
}

int main(int argc, TCHAR* argv[])
{
	BOOL bRet = FALSE;
	char* szFileName = argv[1];
	szName = argv[2];
	printf("[*] CreateService by Uknow\n");
	if (argc != 4)
	{
		printf("   [+] usage: %s BinPath ServiceName start/stop\n", argv[0]);
		printf("   [+] eg: %s \"c:\\evil.exe\" EvilService start/stop\n", argv[0]);
		return -1;
	}
	else if (strcmp(argv[3], "start") == 0) {
		bRet = SystemServiceOperate(szFileName, 0);
		if (FALSE == bRet)
		{
			printf("   [-] Create Error!\n");
			return -1;
		}
		bRet = SystemServiceOperate(szFileName, 1);
		if (FALSE == bRet)
		{
			printf("   [-] Start Error!\n");
			return -1;
		}
		printf("    [+] ServiceName: %s\n", szName);
		printf("    [+] BinaryPathName: %s\n", szFileName);
		printf("    [+] Success! Service successfully Create and Start.\n");
		return 0;
	}
	else if (strcmp(argv[3], "stop") == 0)
	{
		bRet = SystemServiceOperate(szFileName, 2);
		if (FALSE == bRet)
		{
			printf("   [-] Stop Error!\n");
			return -1;
		}
		bRet = SystemServiceOperate(szFileName, 3);
		if (FALSE == bRet)
		{
			printf("   [-] Delete Error!\n");
			return -1;
		}
		printf("    [+] ServiceName: %s\n", szName);
		printf("    [+] BinaryPathName: %s\n", szFileName);
		printf("    [+] Success! Service successfully Stop and Delete.\n");
		return 0;
	}
}