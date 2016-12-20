#include "stdafx.h"
#include "OrderTicketWnd.h"


DUI_BEGIN_MESSAGE_MAP(COrderTicketWnd, WindowImplBase)
	DUI_ON_MSGTYPE(DUI_MSGTYPE_CLICK, OnClick)
DUI_END_MESSAGE_MAP()


COrderTicketWnd::COrderTicketWnd()
{

}
COrderTicketWnd::~COrderTicketWnd()
{

}


void COrderTicketWnd::OnFinalMessage(HWND hWnd)
{
	__super::OnFinalMessage(hWnd);
	delete this;
}
CDuiString COrderTicketWnd::GetSkinFile()
{
	return _T("XML_ORDER_TICKET_WND");
}
LPCTSTR COrderTicketWnd::GetWindowClassName(void) const
{
	return _T("OrderTicketWnd");
}

void COrderTicketWnd::Notify(TNotifyUI &msg)
{
	return WindowImplBase::Notify(msg);
}
void COrderTicketWnd::InitWindow()
{
	m_pCloseBtn = static_cast<CButtonUI*>(m_pm.FindControl(_T("closebtn")));
}

void COrderTicketWnd::OnClick(TNotifyUI &msg)
{
	CDuiString sName = msg.pSender->GetName();
	sName.MakeLower();

	if (msg.pSender == m_pCloseBtn) {
		Close();
		return;
	}
	
	else if (sName.CompareNoCase(_T("confirm_btn")) == 0)
	{
		Close();
	}
	else if (sName.CompareNoCase(_T("cancel_btn")) == 0)
	{
		Close();
	}
}



LRESULT COrderTicketWnd::OnSysCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	bHandled = FALSE;
	return 0L;
}
LRESULT COrderTicketWnd::HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	bHandled = FALSE;
	return 0;
}