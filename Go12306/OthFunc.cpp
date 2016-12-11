#include "StdAfx.h"

#include "OthFunc.h"

#include "Code12306Cert.h"


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



//wstring���ֽڲ�Ϊ0������FALSE
std::string UnicodeToUtf8(const std::wstring &wstr )
{
	std::string str;

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



//wstring���ֽڲ�Ϊ0������FALSE
std::wstring GbkToUnicode(const std::string &str)
{
	std::wstring wstr;

	int iLen = MultiByteToWideChar(CP_ACP, 0, (LPCSTR)str.c_str(), -1, NULL, 0);

	wchar_t *pwText;
	pwText = new wchar_t[iLen]();
	if (!pwText)
	{
		delete[]pwText;
		return _T("");
	}

	MultiByteToWideChar(CP_ACP, 0, (LPCSTR)str.c_str(), -1, pwText, iLen);
	wstr.append(pwText);

	delete[] pwText;

	return wstr;
}


std::string Gbk2Utf8(const std::string str)
{
	std::wstring strUnicode = GbkToUnicode(str);

	return UnicodeToUtf8(strUnicode);
}



void InitResource()
{
	HINSTANCE hDll = ::LoadLibrary(_T("DllRes_d.dll"));
	if (hDll)
	{
		CPaintManagerUI::SetResourceDll(hDll);
	}

	// ��Դ����
#ifdef _DEBUG
	CPaintManagerUI::SetResourceType(UILIB_FILE);
#else
	CPaintManagerUI::SetResourceType(UILIB_ZIPRESOURCE);
#endif
	// ��Դ·��
	CDuiString strResourcePath = CPaintManagerUI::GetInstancePath();
	// ������Դ
	switch (CPaintManagerUI::GetResourceType())
	{
	case UILIB_FILE:
	{
		strResourcePath += _T("skin\\");
		CPaintManagerUI::SetResourcePath(strResourcePath.GetData());
		// ������Դ������
		CResourceManager::GetInstance()->LoadResource(_T("res.xml"), NULL);
		break;
	}
	case UILIB_RESOURCE:
	{
		strResourcePath += _T("skin\\");
		CPaintManagerUI::SetResourcePath(strResourcePath.GetData());
		// ������Դ������
		CResourceManager::GetInstance()->LoadResource(_T("IDR_RES"), _T("xml"));
		break;
	}
	case UILIB_ZIP:
	{
		strResourcePath += _T("skin\\");
		CPaintManagerUI::SetResourcePath(strResourcePath.GetData());
		CPaintManagerUI::SetResourceZip(_T("skin.zip"), true);
		// ������Դ������
		CResourceManager::GetInstance()->LoadResource(_T("res.xml"), NULL);
		break;
	}
	case UILIB_ZIPRESOURCE:
	{
		strResourcePath += _T("skin\\");
		CPaintManagerUI::SetResourcePath(strResourcePath.GetData());

		HRSRC hResource = ::FindResource(CPaintManagerUI::GetResourceDll(), _T("IDR_ZIPRES"), _T("ZIPRES"));
		if (hResource != NULL) {
			DWORD dwSize = 0;
			HGLOBAL hGlobal = ::LoadResource(CPaintManagerUI::GetResourceDll(), hResource);
			if (hGlobal != NULL) {
				dwSize = ::SizeofResource(CPaintManagerUI::GetResourceDll(), hResource);
				if (dwSize > 0) {
					CPaintManagerUI::SetResourceZip((LPBYTE)::LockResource(hGlobal), dwSize);
					// ������Դ������
					CResourceManager::GetInstance()->LoadResource(_T("res.xml"), NULL);
				}
			}
			::FreeResource(hResource);
		}
	}
	break;
	}

	// ע��ؼ�
	REGIST_DUICONTROL(CCode12306CertUI);
}