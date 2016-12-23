#include "StdAfx.h"

#include "OthFunc.h"



#include "Code12306Cert.h"
#include "EditCombo.h"


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

std::string UnicodeToGbk(const std::wstring &wstr)
{
	std::string str;

	int iLen = WideCharToMultiByte(CP_ACP, 0, (LPCWSTR)wstr.c_str(), -1, NULL, 0, NULL, NULL);

	char *pmText;
	pmText = new char[iLen]();
	if (!pmText)
	{
		delete[]pmText;
		return "";
	}

	WideCharToMultiByte(CP_ACP, 0, (LPCWSTR)wstr.c_str(), -1, pmText, iLen, NULL, NULL);
	str.append(pmText);

	delete[] pmText;

	return str;
}



//wstring高字节不为0，返回FALSE
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


std::string Utf8ToGbk(const std::string str)
{
	std::wstring strUnicode = Utf8ToUnicode(str);

	return UnicodeToGbk(strUnicode);
}


void InitResource()
{
	HINSTANCE hDll = ::LoadLibrary(_T("DllRes_d.dll"));
	if (hDll)
	{
		CPaintManagerUI::SetResourceDll(hDll);
	}

	// 资源类型
#ifdef _DEBUG
	CPaintManagerUI::SetResourceType(UILIB_FILE);
#else
	CPaintManagerUI::SetResourceType(UILIB_ZIPRESOURCE);
#endif
	// 资源路径
	CDuiString strResourcePath = CPaintManagerUI::GetInstancePath();
	// 加载资源
	switch (CPaintManagerUI::GetResourceType())
	{
	case UILIB_FILE:
	{
		strResourcePath += _T("skin\\");
		CPaintManagerUI::SetResourcePath(strResourcePath.GetData());
		// 加载资源管理器
		CResourceManager::GetInstance()->LoadResource(_T("res.xml"), NULL);
		break;
	}
	case UILIB_RESOURCE:
	{
		strResourcePath += _T("skin\\");
		CPaintManagerUI::SetResourcePath(strResourcePath.GetData());
		// 加载资源管理器
		CResourceManager::GetInstance()->LoadResource(_T("IDR_RES"), _T("xml"));
		break;
	}
	case UILIB_ZIP:
	{
		strResourcePath += _T("skin\\");
		CPaintManagerUI::SetResourcePath(strResourcePath.GetData());
		CPaintManagerUI::SetResourceZip(_T("skin.zip"), true);
		// 加载资源管理器
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
					// 加载资源管理器
					CResourceManager::GetInstance()->LoadResource(_T("res.xml"), NULL);
				}
			}
			::FreeResource(hResource);
		}
	}
	break;
	}

	// 注册控件
	REGIST_DUICONTROL(CCode12306CertUI);
	REGIST_DUICONTROL(CEditComboUI);
}




/* Compress gzip data */
/* data 原数据 ndata 原数据长度 zdata 压缩后数据 nzdata 压缩后长度 */
int gzcompress(Bytef *data, uLong ndata,	Bytef *zdata, uLong *nzdata)
{
	z_stream c_stream;
	int err = 0;

	if (data && ndata > 0) {
		c_stream.zalloc = NULL;
		c_stream.zfree = NULL;
		c_stream.opaque = NULL;
		//只有设置为MAX_WBITS + 16才能在在压缩文本中带header和trailer
		if (deflateInit2(&c_stream, Z_DEFAULT_COMPRESSION, Z_DEFLATED,
			MAX_WBITS + 16, 8, Z_DEFAULT_STRATEGY) != Z_OK) return -1;
		c_stream.next_in = data;
		c_stream.avail_in = ndata;
		c_stream.next_out = zdata;
		c_stream.avail_out = *nzdata;
		while (c_stream.avail_in != 0 && c_stream.total_out < *nzdata) {
			if (deflate(&c_stream, Z_NO_FLUSH) != Z_OK) return -1;
		}
		if (c_stream.avail_in != 0) return c_stream.avail_in;
		for (;;) {
			if ((err = deflate(&c_stream, Z_FINISH)) == Z_STREAM_END) break;
			if (err != Z_OK) return -1;
		}
		if (deflateEnd(&c_stream) != Z_OK) return -1;
		*nzdata = c_stream.total_out;
		return 0;
	}
	return -1;
}


int Gunzip(Byte *orgBytes, uLong orgSize, std::string &gunZipString)
{
	int err = 0;
	z_stream zStm = { 0 }; /* decompression stream */

	struct {
		Byte *pData;
		uLong size;
		Byte *pCur;
	} outData = {0,0};

	static char dummy_head[2] = {
		0x8 + 0x7 * 0x10,
		(((0x8 + 0x7 * 0x10) * 0x100 + 30) / 31 * 31) & 0xFF,
	};
	zStm.zalloc = NULL;
	zStm.zfree = NULL;
	zStm.opaque = NULL;
	zStm.next_in = orgBytes;
	zStm.avail_in = orgSize;

	//只有设置为MAX_WBITS + 16才能在解压带header和trailer的文本
	if (inflateInit2(&zStm, MAX_WBITS + 16) != Z_OK) 
		return FAIL;
	
	gz_header gzHeader;
	memset(&gzHeader, 0, sizeof(gz_header));
	int ret = inflateGetHeader(&zStm , &gzHeader);

	do
	{
		if (outData.size == 0)
		{

			outData.pData = new Byte[orgSize]();
			outData.size = orgSize;
			outData.pCur = outData.pData;
		}
		else
		{
			Byte *tmpData = new Byte[outData.size + orgSize]();
			memcpy(tmpData, outData.pData, outData.size * sizeof(Byte));

			delete[] outData.pData;

			outData.pData = tmpData;

			///移动到当前可输出位置
			outData.pCur = outData.pData + outData.size;

			outData.size += orgSize;

		}

		zStm.next_out = outData.pCur;

		zStm.avail_out = orgSize;

		if ((err = inflate(&zStm, Z_NO_FLUSH)) == Z_STREAM_END)
			break;
		else if (err == Z_DATA_ERROR && -1 == gzHeader.done)
		{
			///非gzip格式
			gunZipString.append((char*)orgBytes);
			goto END;
		}
		else if (err == Z_DATA_ERROR)
			goto END;
	} while (zStm.avail_out == 0);


	if (inflateEnd(&zStm) != Z_OK)
		goto END;

	gunZipString.append((char*)outData.pData);

END:
	if (outData.pData)
		delete [] outData.pData;

	return 0;

}


long String2Number(std::string str)
{
	long res = 0;
	for (std::string::iterator it = str.begin(); it != str.end(); ++it)
	{
		unsigned char p = *it;
		if (!isdigit(p))
			return 0;

		unsigned int i = p - 0x30;

		res *= 10;
		res += i;
		
	}

	return res;
}


