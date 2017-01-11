#include "StdAfx.h"
#include "ShowSeatTypeWnd.h"
#include "MainFrame.h"

//////////////////////////////////////////////////////////////////////////
///

DUI_BEGIN_MESSAGE_MAP(CShowSeatTypeWnd, WindowImplBase)
DUI_ON_MSGTYPE(DUI_MSGTYPE_CLICK, OnClick)
DUI_ON_CLICK_CTRNAME(_T("btnSeatTypeYDZ"), OnBtnSeatTypeYDZ)
DUI_ON_CLICK_CTRNAME(_T("btnSeatTypeEDZ"), OnBtnSeatTypeEDZ)
DUI_ON_CLICK_CTRNAME(_T("btnSeatTypeTDZ"), OnBtnSeatTypeTDZ)
DUI_ON_CLICK_CTRNAME(_T("btnSeatTypeSWZ"), OnBtnSeatTypeSWZ)
DUI_ON_CLICK_CTRNAME(_T("btnSeatTypeYW"), OnBtnSeatTypeYW)
DUI_ON_CLICK_CTRNAME(_T("btnSeatTypeRW"), OnBtnSeatTypeRW)
DUI_ON_CLICK_CTRNAME(_T("btnSeatTypeYZ"), OnBtnSeatTypeYZ)
DUI_ON_CLICK_CTRNAME(_T("btnSeatTypeRZ"), OnBtnSeatTypeRZ)
DUI_ON_CLICK_CTRNAME(_T("btnSeatTypeGJRW"), OnBtnSeatTypeGJRW)
DUI_END_MESSAGE_MAP()

CShowSeatTypeWnd::CShowSeatTypeWnd(CMainFrame *frame)
	:m_mainFrame(frame)
{
}

CShowSeatTypeWnd::~CShowSeatTypeWnd(void)
{
}


void CShowSeatTypeWnd::OnFinalMessage(HWND hWnd)
{
	__super::OnFinalMessage(hWnd);
	delete this;
}

DuiLib::CDuiString CShowSeatTypeWnd::GetSkinFile()
{
	return _T("XML_SHOWSEATTYPE_WND");
}

LPCTSTR CShowSeatTypeWnd::GetWindowClassName(void) const
{
	return _T("ShowSeatTypeWnd");
}

void CShowSeatTypeWnd::OnClick(TNotifyUI &msg)
{
	CDuiString sName = msg.pSender->GetName();
	sName.MakeLower();

	if (msg.pSender == m_pCloseBtn) {
		Close();
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
	else if (sName.CompareNoCase(_T("confirm_btn")) == 0)
	{
		Close();
	}
	else if (sName.CompareNoCase(_T("cancel_btn")) == 0)
	{
		Close();
	}
}

LRESULT CShowSeatTypeWnd::HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
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

void CShowSeatTypeWnd::Notify(TNotifyUI &msg)
{
	return WindowImplBase::Notify(msg);
}

LRESULT CShowSeatTypeWnd::OnSysCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	bHandled = FALSE;
	return 0L;
}

void CShowSeatTypeWnd::InitWindow()
{

	m_pBtnSeatTypeYDZ = static_cast<CButtonUI*>(m_pm.FindControl(_T("btnSeatTypeYDZ")));
	m_pBtnSeatTypeEDZ = static_cast<CButtonUI*>(m_pm.FindControl(_T("btnSeatTypeEDZ")));
	m_pBtnSeatTypeTDZ = static_cast<CButtonUI*>(m_pm.FindControl(_T("btnSeatTypeTDZ")));
	m_pBtnSeatTypeSWZ = static_cast<CButtonUI*>(m_pm.FindControl(_T("btnSeatTypeSWZ")));
	m_pBtnSeatTypeYW = static_cast<CButtonUI*>(m_pm.FindControl(_T("btnSeatTypeYW")));
	m_pBtnSeatTypeRW = static_cast<CButtonUI*>(m_pm.FindControl(_T("btnSeatTypeRW")));
	m_pBtnSeatTypeYZ = static_cast<CButtonUI*>(m_pm.FindControl(_T("btnSeatTypeYZ")));
	m_pBtnSeatTypeRZ = static_cast<CButtonUI*>(m_pm.FindControl(_T("btnSeatTypeRZ")));
	m_pBtnSeatTypeGJRW = static_cast<CButtonUI*>(m_pm.FindControl(_T("btnSeatTypeGJRW")));


	CListUI *pAddSeatTypeList = static_cast<CListUI*>(m_mainFrame->GetPaintManagerUI()->FindControl(_T("autoSeatTypeList")));


	for (int i = 0; i < pAddSeatTypeList->GetCount(); ++i)
	{
		CListContainerElementUI *pEleContainer = static_cast<CListContainerElementUI*>(pAddSeatTypeList->GetItemAt(i));

		CLabelUI *pTxtUI = static_cast<CLabelUI*>(pEleContainer->GetItemAt(0));

		switch (pTxtUI->GetUserData().GetAt(0)) 
		{
		case 'M':  //Ò»µÈ×ù
			m_pBtnSeatTypeYDZ->SetEnabled(false);
			break;
		case 'O':  ///¶þµÈ×ù
			m_pBtnSeatTypeEDZ->SetEnabled(false);
			break;
		case 'P':  //ÌØµÈ×ù
			m_pBtnSeatTypeTDZ->SetEnabled(false);
			break;
		case '9':  //ÉÌÎñ×ù
			m_pBtnSeatTypeSWZ->SetEnabled(false);
			break;
		case '3':  //Ó²ÎÔ
			m_pBtnSeatTypeYW->SetEnabled(false);
			break;
		case '4':   //ÈíÎÔ
			m_pBtnSeatTypeRW->SetEnabled(false);
			break;
		case '1':   //Ó²×ù
			m_pBtnSeatTypeGJRW->SetEnabled(false);
			break;
		case '2':   ///Èí×ù
			m_pBtnSeatTypeRZ->SetEnabled(false);
			break;
		case '6':   ///¸ß¼¶ÈíÎÔ
			m_pBtnSeatTypeRZ->SetEnabled(false);
			break;
		}


	}


}

void CShowSeatTypeWnd::ShowMessageBox(CMainFrame *frame, RECT rect)
{
	CShowSeatTypeWnd* pWnd = new CShowSeatTypeWnd(frame);

	HWND hParent = frame->GetHWND();

	pWnd->Create(hParent, _T("ShowSeatTypeWnd"), UI_WNDSTYLE_FRAME, 0);

	RECT winRect;
	::GetWindowRect(pWnd->GetHWND(), &winRect);

	POINT orgPt;

	if (rect.right > winRect.right - winRect.left)
	{
		///¿¿°´Å¥×ó²àÏÔÊ¾
		orgPt.x = rect.right - (winRect.right - winRect.left);
		orgPt.y = rect.top - (winRect.bottom - winRect.top);
	}
	else
	{
		///¿¿°´Å¥ÓÒ²àÏÔÊ¾
		orgPt.x = rect.left;
		orgPt.y = rect.top - (winRect.bottom - winRect.top);

	}

	::ClientToScreen(hParent, &orgPt);


	::SetWindowPos(pWnd->GetHWND(), NULL, orgPt.x, orgPt.y, -1, -1, SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE);
	pWnd->ShowWindow(true);
}



void CShowSeatTypeWnd::OnBtnSeatTypeYDZ(TNotifyUI& msg)
{
	AddSeatType(m_pBtnSeatTypeYDZ);

	m_pBtnSeatTypeYDZ->SetEnabled(false);

}
void CShowSeatTypeWnd::OnBtnSeatTypeEDZ(TNotifyUI& msg)
{
	AddSeatType(m_pBtnSeatTypeEDZ);
	m_pBtnSeatTypeEDZ->SetEnabled(false);
}
void CShowSeatTypeWnd::OnBtnSeatTypeTDZ(TNotifyUI& msg)
{
	AddSeatType(m_pBtnSeatTypeTDZ);
	m_pBtnSeatTypeTDZ->SetEnabled(false);
}
void CShowSeatTypeWnd::OnBtnSeatTypeSWZ(TNotifyUI& msg)
{
	AddSeatType(m_pBtnSeatTypeSWZ);
	m_pBtnSeatTypeSWZ->SetEnabled(false);
}
void CShowSeatTypeWnd::OnBtnSeatTypeYW(TNotifyUI& msg)
{
	AddSeatType(m_pBtnSeatTypeYW);
	m_pBtnSeatTypeYW->SetEnabled(false);
}
void CShowSeatTypeWnd::OnBtnSeatTypeRW(TNotifyUI& msg)
{
	AddSeatType(m_pBtnSeatTypeRW);
	m_pBtnSeatTypeRW->SetEnabled(false);
}
void CShowSeatTypeWnd::OnBtnSeatTypeYZ(TNotifyUI& msg)
{
	AddSeatType(m_pBtnSeatTypeYZ);
	m_pBtnSeatTypeYZ->SetEnabled(false);
}
void CShowSeatTypeWnd::OnBtnSeatTypeRZ(TNotifyUI& msg)
{
	AddSeatType(m_pBtnSeatTypeRZ);
	m_pBtnSeatTypeRZ->SetEnabled(false);
}
void CShowSeatTypeWnd::OnBtnSeatTypeGJRW(TNotifyUI& msg)
{
	AddSeatType(m_pBtnSeatTypeGJRW);
	m_pBtnSeatTypeGJRW->SetEnabled(false);
}


void CShowSeatTypeWnd::AddSeatType(CButtonUI *pBtnUI)
{
	CListUI *pAddSeatTypeList = static_cast<CListUI*>(m_mainFrame->GetPaintManagerUI()->FindControl(_T("autoSeatTypeList")));

	CListContainerElementUI* pListItem = new CListContainerElementUI();
	pListItem->SetChildVAlign(DT_VCENTER);

	pListItem->SetFixedHeight(40);

	pListItem->SetManager(m_mainFrame->GetPaintManagerUI(), NULL, false);

	pAddSeatTypeList->Add(pListItem);


	CLabelUI *txtUI = new CLabelUI();
	txtUI->SetAttribute(_T("align"), _T("center"));
	txtUI->SetFont(2);
	txtUI->SetUserData(pBtnUI->GetUserData());
	txtUI->SetText(pBtnUI->GetText());

	pListItem->Add(txtUI);

	CButtonUI *btnDel = new CButtonUI();
	btnDel->SetManager(m_mainFrame->GetPaintManagerUI(), NULL, false);
	btnDel->SetText(_T("É¾³ý"));
	btnDel->SetName(_T("btn_del_seattype"));
	btnDel->SetAttribute(_T("style"), _T("btn_style"));
	pListItem->Add(btnDel);
}
