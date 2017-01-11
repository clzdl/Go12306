#include "StdAfx.h"
#include "ShowPassengerWnd.h"
#include "MainFrame.h"

//////////////////////////////////////////////////////////////////////////
///

DUI_BEGIN_MESSAGE_MAP(CShowPassengerWnd, WindowImplBase)
DUI_ON_MSGTYPE(DUI_MSGTYPE_CLICK, OnClick)
DUI_ON_CLICK_CTRNAME(_T("btnAddPassenger"), OnAddPassengerCodeCb)
DUI_END_MESSAGE_MAP()

CShowPassengerWnd::CShowPassengerWnd(CMainFrame *frame)
	:m_mainFrame(frame)
{
}

CShowPassengerWnd::~CShowPassengerWnd(void)
{
}


void CShowPassengerWnd::OnFinalMessage(HWND hWnd)
{
	__super::OnFinalMessage(hWnd);
	delete this;
}

DuiLib::CDuiString CShowPassengerWnd::GetSkinFile()
{
	return _T("XML_SHOWPASSENGER_WND");
}

LPCTSTR CShowPassengerWnd::GetWindowClassName(void) const
{
	return _T("ShowPassengerWnd");
}

void CShowPassengerWnd::OnClick(TNotifyUI &msg)
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

LRESULT CShowPassengerWnd::HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
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

void CShowPassengerWnd::Notify(TNotifyUI &msg)
{
	return WindowImplBase::Notify(msg);
}

LRESULT CShowPassengerWnd::OnSysCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	bHandled = FALSE;
	return 0L;
}

void CShowPassengerWnd::InitWindow()
{
	m_listPassenger = static_cast<CListUI*>(m_pm.FindControl(_T("passengerList")));

	std::map<std::string, CPassenger>& objMap = Client12306Manager::Instance()->GetPassenger();

	if (!objMap.empty())
	{
		for (std::map<std::string, CPassenger>::iterator it = objMap.begin(); it != objMap.end(); ++it)
		{
			CPassenger &p = it->second;

			CListContainerElementUI* pListItem = new CListContainerElementUI();
			pListItem->SetChildVAlign(DT_VCENTER);

			pListItem->SetFixedHeight(40);

			pListItem->SetManager(&m_pm, NULL, false);

			m_listPassenger->Add(pListItem);

			///
			CCheckBoxUI *pCheck = new CCheckBoxUI();
			pCheck->SetNormalImage(_T("file='common/unchecked.png' source='0,0,20,20' dest='0,10,20,30'"));
			pCheck->SetSelectedImage(_T("file='common/checked.png' source='00,0,20,20' dest='0,10,20,30'"));
			pListItem->Add(pCheck);

			///姓名
			CLabelUI *pTxtUI = new CLabelUI();
			pTxtUI->SetText(p.GetName());
			pListItem->Add(pTxtUI);

			///证件类型
			pTxtUI = new CLabelUI();
			pTxtUI->SetText(p.GetCardTypeName());
			pListItem->Add(pTxtUI);


			///证件号码
			pTxtUI = new CLabelUI();
			pTxtUI->SetText(p.GetCardNo());
			pListItem->Add(pTxtUI);
		}
	
	}
	else
	{
		CListLabelElementUI *eleLabel = new CListLabelElementUI();
		eleLabel->SetText(_T("请添加联系人"));
		m_listPassenger->Add(eleLabel);

	}


}

void CShowPassengerWnd::ShowMessageBox(CMainFrame *frame, RECT rect)
{
	CShowPassengerWnd* pWnd = new CShowPassengerWnd(frame);

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

void CShowPassengerWnd::OnAddPassengerCodeCb(TNotifyUI& msg)
{

	CListUI *pAddPassengerList = static_cast<CListUI*>(m_mainFrame->GetPaintManagerUI()->FindControl(_T("autoPassengerList")));

	pAddPassengerList->RemoveAll();

	for (int i = 0; i < m_listPassenger->GetCount(); ++i)
	{
		CListContainerElementUI *eleContainer = static_cast<CListContainerElementUI*>(m_listPassenger->GetItemAt(i));


		CCheckBoxUI *pCheck = static_cast<CCheckBoxUI*>(eleContainer->GetItemAt(0));

		if (!pCheck->GetCheck())
			continue;


		CListContainerElementUI* pListItem = new CListContainerElementUI();
		pListItem->SetChildVAlign(DT_VCENTER);

		pListItem->SetFixedHeight(40);

		pListItem->SetManager(m_mainFrame->GetPaintManagerUI(), NULL, false);

		pAddPassengerList->Add(pListItem);

		CLabelUI *orgUI = static_cast<CLabelUI*>(eleContainer->GetItemAt(1));
		CLabelUI *orgNoUI = static_cast<CLabelUI*>(eleContainer->GetItemAt(3));

		CLabelUI *txtUI = new CLabelUI();
		txtUI->SetAttribute(_T("align"), _T("center"));
		txtUI->SetFont(2);
		txtUI->SetUserData(orgNoUI->GetText());   ///证件号码
		txtUI->SetText(orgUI->GetText());

		pListItem->Add(txtUI);

		CButtonUI *btnDel = new CButtonUI();
		btnDel->SetManager(m_mainFrame->GetPaintManagerUI(), NULL, false);
		btnDel->SetText(_T("删除"));
		btnDel->SetName(_T("btn_del_passenger"));
		btnDel->SetAttribute(_T("style"), _T("btn_style"));
		pListItem->Add(btnDel);
	}


	Close();
}
