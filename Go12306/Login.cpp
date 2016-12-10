

#include "stdafx.h"
#include "Login.h"
#include "Client12306Manager.h"

#include <fstream>

DUI_BEGIN_MESSAGE_MAP(CLoginWnd, WindowImplBase)
DUI_ON_MSGTYPE(DUI_MSGTYPE_CLICK, OnClick)
DUI_END_MESSAGE_MAP()

CLoginWnd::CLoginWnd(void)
{
}

CLoginWnd::~CLoginWnd(void)
{
}


void CLoginWnd::OnFinalMessage(HWND hWnd)
{
	__super::OnFinalMessage(hWnd);
	//delete this;
}

DuiLib::CDuiString CLoginWnd::GetSkinFile()
{
	return _T("XML_LOGIN");
}

LPCTSTR CLoginWnd::GetWindowClassName(void) const
{
	return _T("LoginWnd");
}

void CLoginWnd::OnClick(TNotifyUI &msg)
{
	CDuiString sName = msg.pSender->GetName();
	sName.MakeLower();

	if (msg.pSender == m_pCloseBtn) {
		Close(MSGID_CANCEL);
		return;
	}
	else if (msg.pSender == m_pMinBtn) {
		SendMessage(WM_SYSCOMMAND, SC_MINIMIZE, 0); return;
	}
	else if (msg.pSender == m_pMaxBtn) {
		SendMessage(WM_SYSCOMMAND, SC_MAXIMIZE, 0); return;
	}
	else if (msg.pSender == m_pRestoreBtn) {
		SendMessage(WM_SYSCOMMAND, SC_RESTORE, 0); return;
	}
	else if (msg.pSender == m_pMenuBtn) {
	}
	else if (sName.CompareNoCase(_T("btnLogin")) == 0)
	{
		Close(MSGID_OK);

		std::string bytes;
		Client12306Manager::Instance()->QueryPassCode("login", bytes );
		std::ofstream file("aaaa.jpg" , ios::binary);

		file.write(bytes.c_str() , bytes.length());
		

	}
	else if (sName.CompareNoCase(_T("btnCancel")) == 0)
	{
		Close(MSGID_CANCEL);

	}
}

LRESULT CLoginWnd::HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	bHandled = FALSE;
	return 0;
}

void CLoginWnd::Notify(TNotifyUI &msg)
{
	return WindowImplBase::Notify(msg);
}

LRESULT CLoginWnd::OnSysCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	bHandled = FALSE;
	return 0L;
}

void CLoginWnd::InitWindow()
{
	m_pCloseBtn = static_cast<CButtonUI*>(m_pm.FindControl(_T("closebtn")));
}
