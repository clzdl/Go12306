

#include "stdafx.h"
#include "Code12306Cert.h"
#include "Client12306Manager.h"
#include "OthFunc.h"
#include "ximage.h"


#define STB_IMAGE_IMPLEMENTATION
#include "utils/stb_image.h"

IMPLEMENT_DUICONTROL(CCode12306CertUI)



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
		RECT  clientRect = GetPos();
		if (event.ptMouse.y - clientRect.top <= 30)
			return;



		std::vector<CDuiRect>::iterator it = m_vecClickPoints.begin();
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
	if (m_ortImageBytes.empty())
		return;
	

	RECT rcClient;
	rcClient = GetClientPos();

	CxImage jpg((unsigned char*)m_ortImageBytes.c_str(), m_ortImageBytes.length(), CXIMAGE_FORMAT_JPG);

	jpg.Draw(hDC, rcClient);

	for (std::vector<CDuiRect>::iterator it = m_vecClickPoints.begin(); it != m_vecClickPoints.end(); ++it)
	{
		CRenderEngine::DrawImageString(hDC, m_pManager, *it, *it, m_sNormalImage, NULL, m_instance);
	}

	
}

void CCode12306CertUI::SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue)
{
	CControlUI::SetAttribute( pstrName,  pstrValue);

	if (_tcsicmp(pstrName, _T("normalimage")) == 0) 
		SetNormalImage(pstrValue);
	
}


void CCode12306CertUI::DoInit()
{
	Client12306Manager::Instance()->QueryPassCode("login", m_ortImageBytes);

}

std::vector<CDuiPoint> CCode12306CertUI::GetSelectedPoint()
{
	std::vector<CDuiPoint> vecPoint;
	RECT rect = GetPos();
	for (std::vector<CDuiRect>::iterator it = m_vecClickPoints.begin(); it != m_vecClickPoints.end(); ++it)
	{
		vecPoint.push_back(CDuiPoint(it->left + 10 - rect.left, it->top + 10 - rect.top - 30 ));
	}

	return vecPoint;
}

void CCode12306CertUI::Refresh()
{
	Client12306Manager::Instance()->QueryPassCode("login", m_ortImageBytes);
	m_vecClickPoints.clear();
	Invalidate();
}

