#include <stdlib.h>
#include <locale.h>  
#include <stdio.h>
#include <windows.h>
#include <tchar.h>

BOOL searchFile(LONGLONG cluster) {
    HANDLE hDevice = CreateFile(_T("\\\\.\\D:"),
        GENERIC_READ,
        FILE_SHARE_READ | FILE_SHARE_WRITE,
        NULL,
        OPEN_EXISTING,
        FILE_FLAG_BACKUP_SEMANTICS,
        NULL);

    if (hDevice == INVALID_HANDLE_VALUE){
        wprintf(L"Invalid handle value!\n");
        return FALSE;
    }
    LOOKUP_STREAM_FROM_CLUSTER_INPUT input = { 0 };
    input.NumberOfClusters = 1;
    input.Cluster[0].QuadPart = cluster;
    BYTE output[5000];
    DWORD dwRes = 0;
    LOOKUP_STREAM_FROM_CLUSTER_OUTPUT result = { 0 };

    BOOL res = DeviceIoControl((HANDLE)hDevice,
        FSCTL_LOOKUP_STREAM_FROM_CLUSTER,
        (LPVOID)&input,
        (DWORD)sizeof(input),
        (LPVOID)output,
        (DWORD)5000,
        (LPDWORD)&dwRes,
        NULL);

    if (res == FALSE) {
        wprintf(L"No device to control!\n");
        return FALSE;
    }
    memcpy(&result, output, sizeof(LOOKUP_STREAM_FROM_CLUSTER_OUTPUT));
    if (result.NumberOfMatches == 0) {
        wprintf(L"File not found!\n");
        return FALSE;
    }
    wprintf(L"File info:\n");
    BYTE* p = (BYTE*)output + result.Offset;
    LOOKUP_STREAM_FROM_CLUSTER_ENTRY* pentry = (LOOKUP_STREAM_FROM_CLUSTER_ENTRY*)p;
    wprintf(L"File: %s\n", pentry->FileName);
    return TRUE;
}

int _tmain(int argc, _TCHAR* argv[])
{
    LONGLONG input = 0;
    wprintf(L"Enter the number of the cluster: \n");
    scanf_s("%llu", &input);
    searchFile(input);
    system("PAUSE");
    return 0;
}