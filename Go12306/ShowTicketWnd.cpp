#include "StdAfx.h"
#include "ShowTicketWnd.h"
#include "MainFrame.h"

//////////////////////////////////////////////////////////////////////////
///

DUI_BEGIN_MESSAGE_MAP(CShowTicketWnd, WindowImplBase)
DUI_ON_MSGTYPE(DUI_MSGTYPE_CLICK, OnClick)
DUI_ON_CLICK_CTRNAME(_T("btnAddTrain"), OnAddTrainCodeCb)
DUI_END_MESSAGE_MAP()

CShowTicketWnd::CShowTicketWnd(CMainFrame *frame)
	:m_mainFrame(frame)
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
	else if (msg.pSender == m_pMenuBtn) {
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
	m_listTicket = static_cast<CListUI*>(m_pm.FindControl(_T("ticketList")));

	CListUI *pTicketList = static_cast<CListUI*>(m_mainFrame->GetPaintManagerUI()->FindControl(_T("ticketListView")));

	if (pTicketList->GetCount() > 0)
	{
		for (int i = 0; i < pTicketList->GetCount(); ++i)
		{
			CListContainerElementUI* pListItem = new CListContainerElementUI();
			pListItem->SetChildVAlign(DT_VCENTER);

			pListItem->SetFixedHeight(40);

			pListItem->SetManager(&m_pm, NULL, false);

			m_listTicket->Add(pListItem);

			///
			CCheckBoxUI *pCheck = new CCheckBoxUI();
			pCheck->SetNormalImage(_T("file='common/unchecked.png' source='0,0,20,20' dest='0,10,20,30'"));
			pCheck->SetSelectedImage(_T("file='common/checked.png' source='00,0,20,20' dest='0,10,20,30'"));
			pListItem->Add(pCheck);


			///车次
			CListContainerElementUI *listContainer = static_cast<CListContainerElementUI*>(pTicketList->GetItemAt(i));
			CLabelUI *pTxtUI = static_cast<CLabelUI*>(listContainer->GetItemAt(0));

			CLabelUI *pNewTxtUI = new CLabelUI();
			pNewTxtUI->SetText(pTxtUI->GetText());
			pListItem->Add(pNewTxtUI);


			CVerticalLayoutUI *pVerLayout = static_cast<CVerticalLayoutUI*>(listContainer->GetItemAt(1));
			///发站
			pTxtUI = static_cast<CLabelUI*>(pVerLayout->GetItemAt(0));

			pNewTxtUI = new CLabelUI();
			pNewTxtUI->SetText(pTxtUI->GetText());
			pListItem->Add(pNewTxtUI);

			///到站
			pTxtUI = static_cast<CLabelUI*>(pVerLayout->GetItemAt(1));
			pNewTxtUI = new CLabelUI();
			pNewTxtUI->SetText(pTxtUI->GetText());
			pListItem->Add(pNewTxtUI);

			pVerLayout = static_cast<CVerticalLayoutUI*>(listContainer->GetItemAt(2));
			///发站时间
			pTxtUI = static_cast<CLabelUI*>(pVerLayout->GetItemAt(0));
			pNewTxtUI = new CLabelUI();
			pNewTxtUI->SetText(pTxtUI->GetText());
			pListItem->Add(pNewTxtUI);

			///到站时间
			pTxtUI = static_cast<CLabelUI*>(pVerLayout->GetItemAt(1));
			pNewTxtUI = new CLabelUI();
			pNewTxtUI->SetText(pTxtUI->GetText());
			pListItem->Add(pNewTxtUI);


			pVerLayout = static_cast<CVerticalLayoutUI*>(listContainer->GetItemAt(3));
			//////历时
			pTxtUI = static_cast<CLabelUI*>(pVerLayout->GetItemAt(0));
			pNewTxtUI = new CLabelUI();
			pNewTxtUI->SetText(pTxtUI->GetText());
			pListItem->Add(pNewTxtUI);


		}
	}
	else
	{
		CListLabelElementUI *emptyUI = new CListLabelElementUI();
		emptyUI->SetText(_T("请先查询车次"));

		m_listTicket->Add(emptyUI);
	}


}

void CShowTicketWnd::ShowMessageBox(CMainFrame *frame, RECT rect)
{
	CShowTicketWnd* pWnd = new CShowTicketWnd(frame);

	HWND hParent = frame->GetHWND();

	pWnd->Create(hParent, _T("ShowTicketWnd"), UI_WNDSTYLE_FRAME, 0);

	RECT winRect;
	::GetWindowRect(pWnd->GetHWND(), &winRect);

	POINT orgPt;

	if (rect.right > winRect.right - winRect.left)
	{
		///靠按钮左侧显示
		orgPt.x = rect.right - (winRect.right - winRect.left);
		orgPt.y = rect.top - (winRect.bottom - winRect.top);
	}
	else
	{
		///靠按钮右侧显示
		orgPt.x = rect.left;
		orgPt.y = rect.top - (winRect.bottom - winRect.top);

	}

	::ClientToScreen(hParent, &orgPt);


	::SetWindowPos(pWnd->GetHWND(), NULL, orgPt.x, orgPt.y, -1, -1, SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE);
	pWnd->ShowWindow(true);
}

void CShowTicketWnd::OnAddTrainCodeCb(TNotifyUI& msg)
{
	
	CListUI *pAddTicketList = static_cast<CListUI*>(m_mainFrame->GetPaintManagerUI()->FindControl(_T("autoTicketList")));

	pAddTicketList->RemoveAll();

	for (int i = 0; i < m_listTicket->GetCount(); ++i)
	{
		CListContainerElementUI *eleContainer  = static_cast<CListContainerElementUI*>(m_listTicket->GetItemAt(i));


		CCheckBoxUI *pCheck = static_cast<CCheckBoxUI*>(eleContainer->GetItemAt(0));

		if (!pCheck->GetCheck())
			continue;

		CListContainerElementUI* pListItem = new CListContainerElementUI();
		pListItem->SetChildVAlign(DT_VCENTER);

		pListItem->SetFixedHeight(40);

		pListItem->SetManager(m_mainFrame->GetPaintManagerUI(), NULL, false);

		pAddTicketList->Add(pListItem);

		CLabelUI *orgUI = static_cast<CLabelUI*>(eleContainer->GetItemAt(1));


		CLabelUI *txtUI = new CLabelUI();
		txtUI->SetAttribute(_T("align") , _T("center"));
		txtUI->SetFont(2);
		txtUI->SetText(orgUI->GetText());

		pListItem->Add(txtUI);

		CButtonUI *btnDel = new CButtonUI();
		btnDel->SetManager(m_mainFrame->GetPaintManagerUI(), NULL , false);
		btnDel->SetText(_T("删除"));
		btnDel->SetName(_T("btn_del_ticket"));
		btnDel->SetAttribute(_T("style"), _T("btn_style"));
		pListItem->Add(btnDel);

	}

	Close();
}
