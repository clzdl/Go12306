

#include "stdafx.h"
#include "Code12306Cert.h"

#include "Client12306Manager.h"


#define STB_IMAGE_IMPLEMENTATION
#include "Utils/stb_image.h"

IMPLEMENT_DUICONTROL(CCode12306CertUI)



static TImageInfo* LoadImage(std::string orgImgBytes)
{
	LPBYTE pImage = NULL;
	int x, y, n;
	pImage = stbi_load_from_memory((const unsigned char*)orgImgBytes.c_str(), orgImgBytes.length(), &x, &y, &n, 4);

	if (!pImage) {
		//::MessageBox(0, _T("½âÎöÍ¼Æ¬Ê§°Ü"), _T("×¥BUG"), MB_OK);
		return NULL;
	}


	BITMAPINFO bmi;
	::ZeroMemory(&bmi, sizeof(BITMAPINFO));
	bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bmi.bmiHeader.biWidth = x;
	bmi.bmiHeader.biHeight = -y;
	bmi.bmiHeader.biPlanes = 1;
	bmi.bmiHeader.biBitCount = 32;
	bmi.bmiHeader.biCompression = BI_RGB;
	bmi.bmiHeader.biSizeImage = x * y * 4;

	bool bAlphaChannel = false;
	LPBYTE pDest = NULL;
	HBITMAP hBitmap = ::CreateDIBSection(NULL, &bmi, DIB_RGB_COLORS, (void**)&pDest, NULL, 0);
	if (!hBitmap) {
		//::MessageBox(0, _T("CreateDIBSectionÊ§°Ü"), _T("×¥BUG"), MB_OK);
		return NULL;
	}

	for (int i = 0; i < x * y; i++)
	{
		pDest[i * 4 + 3] = pImage[i * 4 + 3];
		if (pDest[i * 4 + 3] < 255)
		{
			pDest[i * 4] = (BYTE)(DWORD(pImage[i * 4 + 2])*pImage[i * 4 + 3] / 255);
			pDest[i * 4 + 1] = (BYTE)(DWORD(pImage[i * 4 + 1])*pImage[i * 4 + 3] / 255);
			pDest[i * 4 + 2] = (BYTE)(DWORD(pImage[i * 4])*pImage[i * 4 + 3] / 255);
			bAlphaChannel = true;
		}
		else
		{
			pDest[i * 4] = pImage[i * 4 + 2];
			pDest[i * 4 + 1] = pImage[i * 4 + 1];
			pDest[i * 4 + 2] = pImage[i * 4];
		}

		/*if (*(DWORD*)(&pDest[i * 4]) == mask) {
			pDest[i * 4] = (BYTE)0;
			pDest[i * 4 + 1] = (BYTE)0;
			pDest[i * 4 + 2] = (BYTE)0;
			pDest[i * 4 + 3] = (BYTE)0;
			bAlphaChannel = true;
		}*/
	}

	stbi_image_free(pImage);

	TImageInfo* data = new TImageInfo;
	data->pBits = NULL;
	data->pSrcBits = NULL;
	data->hBitmap = hBitmap;
	data->nX = x;
	data->nY = y;
	data->bAlpha = bAlphaChannel;

	return data;
}

CCode12306CertUI::CCode12306CertUI()
{

}
CCode12306CertUI::~CCode12306CertUI()
{

}

LPCTSTR CCode12306CertUI::GetClass() const
{
	return _T("Code12306CertUI");
}
LPVOID CCode12306CertUI::GetInterface(LPCTSTR pstrName)
{
	if (_tcsicmp(pstrName, _T("Code12306Cert")) == 0) return static_cast<CCode12306CertUI*>(this);
	return CControlUI::GetInterface(pstrName);
}


void CCode12306CertUI::DoEvent(TEventUI& event)
{
	if (event.Type == UIEVENT_RBUTTONUP)
	{

		Client12306Manager::Instance()->QueryPassCode("login", m_ortImageBytes);
		Invalidate();
	}
	else if (event.Type == UIEVENT_BUTTONUP)
	{
		std::vector<RECT>::iterator it = m_vecClickPoints.begin();
		for (; it != m_vecClickPoints.end(); ++it)
		{
			if (PtInRect(&(*it), event.ptMouse))
			{
				break;
			}

			
		}
		if (it != m_vecClickPoints.end())
			m_vecClickPoints.erase(it);
		else
			m_vecClickPoints.push_back(CDuiRect(event.ptMouse.x-10 , event.ptMouse.y - 10, event.ptMouse.x + 10, event.ptMouse.y + 10));

		
		Invalidate();
	}

}



void CCode12306CertUI::DoPaint(HDC hDC, const RECT& rcPaint)
{
	
	if(!m_ortImageBytes.empty())
	{
		RECT rcClient;
		rcClient = GetClientPos();


	

		TImageInfo *pImage = LoadImage(m_ortImageBytes);

		CDuiRect rcBmpPart(0, 0, pImage->nX, pImage->nY);

		CDuiRect rcCorner(0, 0, 0, 0);

		CRenderEngine::DrawImage(hDC, pImage->hBitmap, rcClient , rcClient , rcBmpPart, rcCorner, pImage->bAlpha , 255);
	

		for (std::vector<RECT>::iterator it = m_vecClickPoints.begin(); it != m_vecClickPoints.end(); ++it)
		{

			CRenderEngine::DrawImageString(hDC, m_pManager, *it, *it, m_sNormalImage, NULL, m_instance);
		}

	}
}

void CCode12306CertUI::SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue)
{
	if (_tcsicmp(pstrName, _T("normalimage")) == 0) 
		SetNormalImage(pstrValue);
	
}


void CCode12306CertUI::DoInit()
{
	Client12306Manager::Instance()->QueryPassCode("login", m_ortImageBytes);
}