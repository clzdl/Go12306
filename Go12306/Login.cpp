

#include "stdafx.h"
#include "Login.h"
#include "Client12306Manager.h"
#include "OthFunc.h"

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
		
		std::vector<CDuiPoint> vecPoint = m_code12306UI->GetSelectedPoint();

		for (std::vector<CDuiPoint>::iterator it = vecPoint.begin(); it != vecPoint.end(); ++it)
		{
			DUI__Trace(_T("x:%d,y:%d") , it->x,it->y);
		}
		std::string response;
		Client12306Manager::Instance()->AnsynValidPassCode(vecPoint, response);

		CDuiString userName =  m_pEdtUserName->GetText();
		CDuiString userPass = m_pEdtUserPass->GetText();

		char randCode[128] = { 0 };
		for (std::vector<CDuiPoint>::iterator it = vecPoint.begin(); it != vecPoint.end(); ++it)
		{
			if (it == vecPoint.begin())
				sprintf(randCode, "%d,%d", it->x, it->y);
			else
				sprintf(randCode, "%s,%d,%d", randCode, it->x, it->y);
		}

		Client12306Manager::Instance()->AnsysLoginSugguest(UnicodeToUtf8(userName.GetData()),
														UnicodeToUtf8(userPass.GetData()), randCode , response);

		
		Client12306Manager::Instance()->UserLogin(response);

		Client12306Manager::Instance()->InitMy12306(response);
		
		
		Close(MSGID_OK);

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

	m_code12306UI = static_cast<CCode12306CertUI*>(m_pm.FindControl(_T("code12306Code")));

	m_pEdtUserName = static_cast<CEditUI*>(m_pm.FindControl(_T("edtLoginName")));
	m_pEdtUserPass = static_cast<CEditUI*>(m_pm.FindControl(_T("edtLoginPwd")));
}
