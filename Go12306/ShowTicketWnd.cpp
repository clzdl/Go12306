#include "StdAfx.h"
#include "ShowTicketWnd.h"

//////////////////////////////////////////////////////////////////////////
///

DUI_BEGIN_MESSAGE_MAP(CShowTicketWnd, WindowImplBase)
DUI_ON_MSGTYPE(DUI_MSGTYPE_CLICK, OnClick)
DUI_END_MESSAGE_MAP()

CShowTicketWnd::CShowTicketWnd(void)
{
}

CShowTicketWnd::~CShowTicketWnd(void)
{
}


void CShowTicketWnd::OnFinalMessage(HWND hWnd)
{
	__super::OnFinalMessage(hWnd);
	delete this;
}

DuiLib::CDuiString CShowTicketWnd::GetSkinFile()
{
	return _T("XML_SHOWTICKET_WND");
}

LPCTSTR CShowTicketWnd::GetWindowClassName(void) const
{
	return _T("ShowTicketWnd");
}

void CShowTicketWnd::OnClick(TNotifyUI &msg)
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
	else if (sName.CompareNoCase(_T("confirm_btn")) == 0)
	{
		Close(MSGID_OK);
	}
	else if (sName.CompareNoCase(_T("cancel_btn")) == 0)
	{
		Close(MSGID_CANCEL);
	}
}

LRESULT CShowTicketWnd::HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	
	switch (uMsg)
	{
	case WM_KILLFOCUS:
		Close();
		bHandled = true;
		break;
	default:
		bHandled = FALSE;
		break;
	}
	return 0;
}

void CShowTicketWnd::Notify(TNotifyUI &msg)
{
	return WindowImplBase::Notify(msg);
}

LRESULT CShowTicketWnd::OnSysCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	bHandled = FALSE;
	return 0L;
}

void CShowTicketWnd::InitWindow()
{
	m_pCloseBtn = static_cast<CButtonUI*>(m_pm.FindControl(_T("closebtn")));
}
