#include "StdAfx.h"

#include "OthFunc.h"

std::wstring StringToWString(const std::string &str)
{
	std::wstring wstr;
	int nLen = (int)str.length();
	wstr.resize(nLen, L' ');

	int nResult = MultiByteToWideChar(CP_ACP, 0, (LPCSTR)str.c_str(), nLen, (LPWSTR)wstr.c_str(), nLen);

	if (nResult == 0)
	{
		return _T("");
	}

	return wstr;
}
//wstring高字节不为0，返回FALSE
std::string WStringToString(const std::wstring &wstr )
{
	std::string str;
	int nLen = (int)wstr.length();
	str.resize(nLen, ' ');

	int nResult = WideCharToMultiByte(CP_ACP, 0, (LPCWSTR)wstr.c_str(), nLen, (LPSTR)str.c_str(), nLen, NULL, NULL);

	if (nResult == 0)
	{
		return "";
	}

	return str;
}