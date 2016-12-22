#include "stdafx.h"

#include "CertCodeWnd.h"
#include "Code12306Cert.h"

DUI_BEGIN_MESSAGE_MAP(CCertCodeWnd, WindowImplBase)
	DUI_ON_MSGTYPE(DUI_MSGTYPE_CLICK, OnClick)
DUI_END_MESSAGE_MAP()

CCertCodeWnd::CCertCodeWnd(void)
{
}

CCertCodeWnd::~CCertCodeWnd(void)
{
}

void CCertCodeWnd::SetTitle(LPCTSTR lpstrTitle)
{
	if (lstrlen(lpstrTitle) <= 0) return;

	CControlUI* pControl = static_cast<CControlUI*>(m_pm.FindControl(_T("MessageTitle")));
	if (pControl) pControl->SetText(lpstrTitle);
}


void CCertCodeWnd::OnFinalMessage(HWND hWnd)
{
	__super::OnFinalMessage(hWnd);
///	delete this;
}

DuiLib::CDuiString CCertCodeWnd::GetSkinFile()
{
	return _T("XML_CERT_CODE");
}

LPCTSTR CCertCodeWnd::GetWindowClassName(void) const
{
	return _T("CertCodeWnd");
}

void CCertCodeWnd::OnClick(TNotifyUI &msg)
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

		m_vecSelPoint = m_pCert12306UI->GetSelectedPoint();
		Close(MSGID_OK);
	}
	else if (sName.CompareNoCase(_T("cancel_btn")) == 0)
	{
		Close(MSGID_CANCEL);
	}
}

LRESULT CCertCodeWnd::HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	bHandled = FALSE;
	return 0;
}

void CCertCodeWnd::Notify(TNotifyUI &msg)
{
	return WindowImplBase::Notify(msg);
}

LRESULT CCertCodeWnd::OnSysCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	bHandled = FALSE;
	return 0L;
}

void CCertCodeWnd::InitWindow()
{
	m_pCloseBtn = static_cast<CButtonUI*>(m_pm.FindControl(_T("closebtn")));

	m_pCert12306UI = static_cast<CCode12306CertUI*>(m_pm.FindControl(_T("code12306Code")));
}
