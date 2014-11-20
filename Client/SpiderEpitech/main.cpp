//
//  main.cpp
//  AsyncClientBoost
//
//  Created by Charles Fournier on 21/10/2014.
//  Copyright (c) 2014 Charles Fournier. All rights reserved.
//

#include "Spider.h"
#include "Config.h"
#include <string.h>
#include <windows.h>

void startup()
{
	TCHAR szPath[MAX_PATH];
	DWORD pathLen = 0;

	pathLen = GetModuleFileName(NULL, szPath, MAX_PATH);
	if (pathLen == 0)
	{
		std::cout << "Failed to get path" << std::endl;
		return ;
	}
	HKEY newValue;
	if (RegOpenKey(HKEY_CURRENT_USER,
		TEXT("Software\\Microsoft\\Windows\\CurrentVersion\\Run"),
		&newValue) != ERROR_SUCCESS)
	{
		std::cout << "Failed to open registry" << std::endl;
		return ;
	}
	DWORD pathLenInBytes = pathLen * sizeof(*szPath);
	if (RegSetValueEx(newValue,
		TEXT("Spider"),
		0,
		REG_SZ,
		(LPBYTE)szPath,
		pathLenInBytes) != ERROR_SUCCESS)
	{
		std::cout << "Failed to add key" << std::endl;
		RegCloseKey(newValue);
		return ;
	}
	RegCloseKey(newValue);
	return ;
}

int main(int argc, const char * argv[])
{
	startup();
	try
	{
		Spider spider;

		spider.run();
	}
	catch (std::exception const &e)
	{
		std::cerr << e.what() << std::endl;
	}
}
