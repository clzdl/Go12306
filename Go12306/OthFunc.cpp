#include "StdAfx.h"

#include "OthFunc.h"


std::wstring Utf8ToUnicode(const std::string &str)
{
	std::wstring wstr;

	int iLen = MultiByteToWideChar(CP_UTF8, 0, (LPCSTR)str.c_str(), -1, NULL, 0);

	wchar_t *pwText;
	pwText = new wchar_t[iLen]();
	if (!pwText)
	{
		delete []pwText;
		return _T("");
	}

	MultiByteToWideChar(CP_UTF8, 0, (LPCSTR)str.c_str(), -1, pwText, iLen);
	wstr.append(pwText);

	delete[] pwText;

	return wstr;
}



//wstring高字节不为0，返回FALSE
std::string UnicodeToUtf8(const std::wstring &wstr )
{
	std::string str;
	int nLen = (int)wstr.length();
	str.resize(nLen, ' ');

	int iLen = WideCharToMultiByte(CP_UTF8, 0, (LPCWSTR)wstr.c_str(), -1, NULL, 0, NULL, NULL);
	
	char *pmText;
	pmText = new char[iLen ]();
	if (!pmText)
	{
		delete []pmText;
		return "";
	}

	WideCharToMultiByte(CP_UTF8, 0, (LPCWSTR)wstr.c_str(), -1, pmText, iLen, NULL, NULL);
	str.append(pmText);

	delete[] pmText;

	return str;
}

