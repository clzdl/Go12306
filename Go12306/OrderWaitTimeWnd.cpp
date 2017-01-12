#include "stdafx.h"
#include "OrderWaitTimeWnd.h"

#include "OthFunc.h"


#define		WAIT_TIMER_ID		4444
///

DUI_BEGIN_MESSAGE_MAP(COrderWaitTimeWnd, WindowImplBase)
	DUI_ON_MSGTYPE(DUI_MSGTYPE_CLICK, OnClick)
DUI_END_MESSAGE_MAP()




COrderWaitTimeWnd::COrderWaitTimeWnd(std::string token)
	:m_tokenString(token)
{
}

COrderWaitTimeWnd::~COrderWaitTimeWnd(void)
{
}

void COrderWaitTimeWnd::SetTitle(LPCTSTR lpstrTitle)
{
	if (lstrlen(lpstrTitle) <= 0) return;

	CControlUI* pControl = static_cast<CControlUI*>(m_pm.FindControl(_T("MessageTitle")));
	if (pControl) pControl->SetText(lpstrTitle);
}

void COrderWaitTimeWnd::SetMsg(LPCTSTR lpstrMsg)
{
	if (lstrlen(lpstrMsg) <= 0) return;

	CControlUI* pControl = static_cast<CControlUI*>(m_pm.FindControl(_T("MessageText")));
	if (pControl) pControl->SetText(lpstrMsg);
}

void COrderWaitTimeWnd::OnFinalMessage(HWND hWnd)
{
	__super::OnFinalMessage(hWnd);
}

DuiLib::CDuiString COrderWaitTimeWnd::GetSkinFile()
{
	return _T("XML_ORDER_WAIT_TIME_WND");
}

LPCTSTR COrderWaitTimeWnd::GetWindowClassName(void) const
{
	return _T("OrderWaitTimeWnd");
}

void COrderWaitTimeWnd::OnClick(TNotifyUI &msg)
{
	CDuiString sName = msg.pSender->GetName();
	sName.MakeLower();

	if (msg.pSender == m_pCloseBtn) {
		m_sErrInfo = _T("您关闭了窗口，订票结果未知，请通过查询订单确认订票结果，谢谢");
		Close(E_FAILURE);
		return;
	}


}

LRESULT COrderWaitTimeWnd::HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	switch (uMsg)
	{
	case WM_TIMER:
		if (wParam == WAIT_TIMER_ID) {

			--m_iDispatchTime;
			RefreshShowText();

			if (m_iDispatchTime > m_iNextRequestTime)
				return 0;
		
			::KillTimer(GetHWND(), WAIT_TIMER_ID);

			m_orderWaitTimeResult.Init();

			QueryOrderWaitTime();
			
			return 0;
		}
		break;
	}

	bHandled = FALSE;
	return 0;
}

void COrderWaitTimeWnd::Notify(TNotifyUI &msg)
{
	return WindowImplBase::Notify(msg);
}

LRESULT COrderWaitTimeWnd::OnSysCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	bHandled = FALSE;
	return 0L;
}

void COrderWaitTimeWnd::InitWindow()
{
	m_pCloseBtn = static_cast<CButtonUI*>(m_pm.FindControl(_T("closebtn")));

	QueryOrderWaitTime();

}

void COrderWaitTimeWnd::InitilizeTimer()
{
	::SetTimer(m_hWnd, WAIT_TIMER_ID, 1000, NULL);

	m_iDispatchTime = m_orderWaitTimeResult.GetWaitTime();

	int iTmp = m_iDispatchTime / 1.5;

	iTmp = iTmp > 60 ? 60 : iTmp;

	iTmp = m_iDispatchTime - iTmp;

	m_iNextRequestTime = iTmp <= 0 ? 1 : iTmp;



}


void COrderWaitTimeWnd::QueryOrderWaitTime()
{
	_ERRNO err = E_OK;
	if (E_OK != (err =  Client12306Manager::Instance()->QueryOrderWaitTime(m_tokenString, m_orderWaitTimeResult)))
	{
		m_sErrInfo = Utf8ToUnicode(Client12306Manager::Instance()->GetLastErrInfo()).c_str();
		Close(err);
	}

	
	if (m_orderWaitTimeResult.GetWaitTime() > 0)
	{
		InitilizeTimer();

		RefreshShowText();
		return;
	}
	else if (m_orderWaitTimeResult.GetOrderId().empty())
	{
		m_orderWaitTimeResult.SetWaitTime(1);
		InitilizeTimer();

		RefreshShowText();
		return;
	}


	///等待时间结束，
	if (E_OK != (err = Client12306Manager::Instance()->ResultOrderForDcQueue(m_orderWaitTimeResult.GetOrderId(), m_tokenString)))
	{
		m_sErrInfo = Utf8ToUnicode(Client12306Manager::Instance()->GetLastErrInfo()).c_str();

		Close(err);
	}

	Close(E_OK);


}

void COrderWaitTimeWnd::RefreshShowText()
{
	CLabelUI* pMsg = static_cast<CLabelUI*>(m_pm.FindControl(_T("MessageText")));

	CDuiString sMsg;
	sMsg.Format(_T("剩余等待时间 %d 秒......") , m_iDispatchTime);

	pMsg->SetText(sMsg);

}