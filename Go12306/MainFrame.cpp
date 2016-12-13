#include "stdafx.h"
#include "MainFrame.h"
#include "Resource.h"
#include "MsgWnd.h"
#include "SkinFrame.h"
#include <sstream>
#include "OthFunc.h"


#include "Client12306Manager.h"
#include "ProgressDlg.h"
#include "TicketModel.h"
#include "Login.h"




CDuiString CMainFrame::m_transLiShi[] = { _T("当日到达"),_T("次日到达"),_T("两日到达"),_T("三日到达") };

CMainFrame::CMainFrame()
:m_bAllTrainType(true),
 m_tWorker(new CWorker())
{}

CControlUI* CMainFrame::CreateControl(LPCTSTR pstrClass)
{
	
	return NULL;
}

void CMainFrame::InitWindow()
{
	// 多语言接口
	CResourceManager::GetInstance()->SetTextQueryInterface(this);
	CResourceManager::GetInstance()->LoadLanguage(_T("lan_cn.xml"));
	// 皮肤接口
	CSkinManager::GetSkinManager()->AddReceiver(this);

	m_pCloseBtn = static_cast<CButtonUI*>(m_pm.FindControl(_T("closebtn")));
	m_pMaxBtn = static_cast<CButtonUI*>(m_pm.FindControl(_T("maxbtn")));
	m_pRestoreBtn = static_cast<CButtonUI*>(m_pm.FindControl(_T("restorebtn")));
	m_pMinBtn = static_cast<CButtonUI*>(m_pm.FindControl(_T("minbtn")));
	m_pSkinBtn = static_cast<CButtonUI*>(m_pm.FindControl(_T("skinbtn")));

	///导航条
	m_pOptTicketQuery = static_cast<COptionUI*>(m_pm.FindControl(_T("ticket_query")));
	m_pOptOrderManage = static_cast<COptionUI*>(m_pm.FindControl(_T("order_manager")));
	m_pOptUserManage = static_cast<COptionUI*>(m_pm.FindControl(_T("user_manager")));

	RefreshAllTrainCHeckBox(m_bAllTrainType);

	RefreshOrderListView();

	//CLoginWnd* pLogin = new CLoginWnd();
	//pLogin->Create(NULL, _T("LoginWnd"), WS_POPUP | WS_CLIPCHILDREN, WS_EX_TOOLWINDOW);
	//pLogin->CenterWindow();


	//switch (pLogin->ShowModal())
	//{
	//case MSGID_OK:
	//	break;
	//case MSGID_CANCEL:
	//	PostQuitMessage(0L);
	//	break;

	//}
	

	// 注册托盘图标
	m_trayIcon.CreateTrayIcon(m_hWnd, IDI_GO12306, _T("gogo 12306"));
}

BOOL CMainFrame::Receive(SkinChangedParam param)
{
	CControlUI* pRoot = m_pm.FindControl(_T("root"));
	if (pRoot != NULL) {
		if (param.bColor) {
			pRoot->SetBkColor(param.bkcolor);
			pRoot->SetBkImage(_T(""));
		}
		else {
			pRoot->SetBkColor(0);
			pRoot->SetBkImage(param.bgimage);
			//m_pm.SetLayeredImage(param.bgimage);
		}
	}
	return TRUE;
}



DuiLib::CDuiString CMainFrame::GetSkinFile()
{
	return _T("XML_MAIN");
}

LPCTSTR CMainFrame::GetWindowClassName() const
{
	return _T("MainFrame");
}

UINT CMainFrame::GetClassStyle() const
{
	return CS_DBLCLKS;
}

void CMainFrame::OnFinalMessage(HWND hWnd)
{
	__super::OnFinalMessage(hWnd);
}


LPCTSTR CMainFrame::QueryControlText(LPCTSTR lpstrId, LPCTSTR lpstrType)
{
	CDuiString sLanguage = CResourceManager::GetInstance()->GetLanguage();
	if (sLanguage == _T("en")) {
		if (lstrcmpi(lpstrId, _T("titletext")) == 0) {
			return _T("Duilib Demo v1.1");
		}
		else if (lstrcmpi(lpstrId, _T("hometext")) == 0) {
			return _T("{a}Home Page{/a}");
		}
	}
	else {
		if (lstrcmpi(lpstrId, _T("titletext")) == 0) {
			return _T("Duilib 使用演示 v1.1");
		}
		else if (lstrcmpi(lpstrId, _T("hometext")) == 0) {
			return _T("{a}演示官网{/a}");
		}
	}

	return NULL;
}

LRESULT CMainFrame::OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
{
	m_trayIcon.DeleteTrayIcon();
	bHandled = FALSE;
	// 退出程序
	PostQuitMessage(0);
	return 0;
}
LRESULT CMainFrame::HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	// 关闭窗口，退出程序
	if (uMsg == WM_DESTROY)
	{
		::PostQuitMessage(0L);
		bHandled = TRUE;
		return 0;
	}
	else if (uMsg == WM_TIMER)
	{
		bHandled = FALSE;
	}
	else if (uMsg == WM_MENUCLICK)
	{
		MenuCmd* pMenuCmd = (MenuCmd*)wParam;
		if (pMenuCmd != NULL)
		{
			BOOL bChecked = pMenuCmd->bChecked;
			CDuiString sMenuName = pMenuCmd->szName;
			CDuiString sUserData = pMenuCmd->szUserData;
			CDuiString sText = pMenuCmd->szText;
			m_pm.DeletePtr(pMenuCmd);

			if (sMenuName == _T("exit")) {
				Close(0);
			}
			else
			{
				CMsgWnd::MessageBox(m_hWnd, NULL, sText);
			}
		}
		if (m_pMenu != NULL) {
			delete m_pMenu;
			m_pMenu = NULL;
		}
		bHandled = TRUE;
		return 0;
	}
	else if (uMsg == UIMSG_TRAYICON)
	{
		UINT uIconMsg = (UINT)lParam;
		if (uIconMsg == WM_LBUTTONUP) {
			BOOL bVisible = IsWindowVisible(m_hWnd);
			::ShowWindow(m_hWnd, !bVisible ? SW_SHOW : SW_HIDE);
		}
		else if (uIconMsg == WM_RBUTTONUP) {
			if (m_pMenu != NULL) {
				delete m_pMenu;
				m_pMenu = NULL;
			}
			m_pMenu = new CMenuWnd();
			CDuiPoint point;
			::GetCursorPos(&point);
			point.y -= 100;
			m_pMenu->Init(NULL, _T("menu.xml"), point, &m_pm);
			// 动态添加后重新设置菜单的大小
			m_pMenu->ResizeMenu();
		}
	}
	
	bHandled = FALSE;
	return 0;
}



void CMainFrame::Notify(TNotifyUI& msg)
{
	CDuiString name = msg.pSender->GetName();
	
	if (msg.sType == _T("click"))
	{
		if (name.CompareNoCase(_T("closebtn")) == 0)
		{
			if (IDYES == MessageBox(m_hWnd, _T("确定退出？"), _T("Go12306"), MB_YESNO))
			{
				::DestroyWindow(m_hWnd);
			}
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
		else if (msg.pSender == m_pSkinBtn) {
			new CSkinFrame(m_hWnd, m_pSkinBtn);
		}
		// 按钮消息
		OnLClick(msg.pSender);
	}
	else if (msg.sType == _T("selectchanged"))
	{

		if (m_pOptTicketQuery == msg.pSender || m_pOptOrderManage == msg.pSender || m_pOptUserManage == msg.pSender)
		{
			CTabLayoutUI* pTabSwitch = static_cast<CTabLayoutUI*>(m_pm.FindControl(_T("tab_switch")));
			if (name.CompareNoCase(_T("ticket_query")) == 0)
			{
				pTabSwitch->SelectItem(0);
			}
			else if (name.CompareNoCase(_T("order_manager")) == 0)
			{
				pTabSwitch->SelectItem(1);
			}
			else if (name.CompareNoCase(_T("user_manager")) == 0)
			{
				pTabSwitch->SelectItem(2);
			}
		}
		else if (name.CompareNoCase(_T("train_all")) == 0)
		{
			m_bAllTrainType = !m_bAllTrainType;

			RefreshAllTrainCHeckBox(m_bAllTrainType);

			RefreshTicketListView();

		}
		else if (name.CompareNoCase(_T("train_gc")) == 0)
		{

			CCheckBoxUI *pCheckBoxUI = dynamic_cast<CCheckBoxUI*>(msg.pSender);

			if (pCheckBoxUI->GetCheck())
				m_setShowTrainType.insert(_GC);
			else
				m_setShowTrainType.erase(_GC);

			RefreshTicketListView();
		}
		else if (name.CompareNoCase(_T("train_d")) == 0)
		{
			CCheckBoxUI *pCheckBoxUI = dynamic_cast<CCheckBoxUI*>(msg.pSender);

			if (pCheckBoxUI->GetCheck())
				m_setShowTrainType.insert(_D);
			else
				m_setShowTrainType.erase(_D);

			RefreshTicketListView();
		}
		else if (name.CompareNoCase(_T("train_z")) == 0)
		{
			CCheckBoxUI *pCheckBoxUI = dynamic_cast<CCheckBoxUI*>(msg.pSender);

			if (pCheckBoxUI->GetCheck())
				m_setShowTrainType.insert(_Z);
			else
				m_setShowTrainType.erase(_Z);

			RefreshTicketListView();
		}
		else if (name.CompareNoCase(_T("train_t")) == 0)
		{
			CCheckBoxUI *pCheckBoxUI = dynamic_cast<CCheckBoxUI*>(msg.pSender);

			if (pCheckBoxUI->GetCheck())
				m_setShowTrainType.insert(_T);
			else
				m_setShowTrainType.erase(_T);

			RefreshTicketListView();
		}
		else if (name.CompareNoCase(_T("train_k")) == 0)
		{
			CCheckBoxUI *pCheckBoxUI = dynamic_cast<CCheckBoxUI*>(msg.pSender);

			if (pCheckBoxUI->GetCheck())
				m_setShowTrainType.insert(_K);
			else
				m_setShowTrainType.erase(_K);

			RefreshTicketListView();
		}
		else if (name.CompareNoCase(_T("train_o")) == 0)
		{
			CCheckBoxUI *pCheckBoxUI = dynamic_cast<CCheckBoxUI*>(msg.pSender);

			if (pCheckBoxUI->GetCheck())
				m_setShowTrainType.insert(_O);
			else
				m_setShowTrainType.erase(_O);

			RefreshTicketListView();
		}
		else if (name.CompareNoCase(_T("ORDER_EXPAND_BTN")) == 0)
		{
			CCheckBoxUI *pOrderExpandBtn = dynamic_cast<CCheckBoxUI*>(msg.pSender);

			CListUI* pLstTicket = (CListUI*)m_pm.FindControl(pOrderExpandBtn->GetUserData());

			if (pOrderExpandBtn->GetCheck())
			{
				pLstTicket->SetVisible(true);
				pLstTicket->GetParent()->GetParent()->SetFixedHeight(100);
			}
			else
			{
				pLstTicket->SetVisible(false);
				pLstTicket->GetParent()->GetParent()->SetFixedHeight(20);

			}


		}
		
		
	}

}


void CMainFrame::OnLClick(CControlUI *pControl)
{
	CDuiString sName = pControl->GetName();
	if (sName.CompareNoCase(_T("btnTicketQuery")) == 0)
	{

		//CMsgWnd::MessageBox(m_hWnd,  _T(""), _T("aaaaa"));
		QueryTicket(_T("") , _T("") ,  _T(""));

	}
	else if (sName.CompareNoCase(_T("menubtn")) == 0)
	{
		if (m_pMenu != NULL) {
			delete m_pMenu;
			m_pMenu = NULL;
		}
		m_pMenu = new CMenuWnd();
		CMenuWnd::GetGlobalContextMenuObserver().SetMenuCheckInfo(&m_MenuInfos);
		CDuiPoint point;
		::GetCursorPos(&point);

		m_pMenu->Init(NULL, _T("menu.xml"), point, &m_pm);


		// 动态添加后重新设置菜单的大小
		m_pMenu->ResizeMenu();
	}
	else if (sName.CompareNoCase(_T("OrderTicketBtn")) == 0)
	{
		CMsgWnd::MessageBox(m_hWnd, _T(""), _T("aaaaa"));

		CMsgWnd::MessageBox(m_hWnd, _T(""), pControl->GetUserData());
	
	}
	
	
}


int CMainFrame::QueryTicket(CDuiString begPlace, CDuiString endPlace, CDuiString travelTime )
{


	begPlace = _T("SJP");
	endPlace = _T("BJP");
	travelTime = _T("2016-12-12");

	CProgressDlg* pProgressDlg = CProgressDlg::CreateDlg(this->GetHWND());
	
	

	m_tWorker->SetVecTicket(&m_vecTicket);
	m_tWorker->SetProgressDlg(pProgressDlg);
	m_tWorker->SetQueryParam(begPlace, endPlace, travelTime,_ADULT);
	m_tpWorker.start(*m_tWorker);
	pProgressDlg->ShowModal();

	RefreshTicketListView();
		
	return 0;
}


int CMainFrame::RefreshTicketListView()
{
	CListUI* pTicketListView = static_cast<CListUI*>(m_pm.FindControl(_T("ticketListView")));

	pTicketListView->RemoveAll();

	CDuiString tmpString;
	int iIndex = 0;
	for (std::vector<CTicketModel>::iterator it = m_vecTicket.begin(); it != m_vecTicket.end(); ++it, ++iIndex)
	{
		DUI__Trace(it->GetTrainNo());

		if (!IsShowTrain(it->GetStationTrainCode()))
			continue;

		///添加行
		CListContainerElementUI* pListItem = new CListContainerElementUI();
		pListItem->SetChildVAlign(DT_VCENTER);
		pListItem->SetFixedHeight(40);
		pListItem->SetManager(&m_pm, NULL, false);

		pTicketListView->Add(pListItem);

		{
			///车次
			CLabelUI *pTxtUI = new CLabelUI();
			pTxtUI->SetManager(&m_pm, NULL, false);
			pTxtUI->SetAttribute(_T("style"), _T("listitem_style"));
			pTxtUI->SetFont(2);
			pTxtUI->SetText(it->GetStationTrainCode());
			pListItem->Add(pTxtUI);
		}


		{///出发站/到达站
			CVerticalLayoutUI *pStationVLayoutUI = new CVerticalLayoutUI();

			CLabelUI *pTxtUI = new CLabelUI();
			pTxtUI->SetManager(&m_pm, NULL, false);
			pTxtUI->SetAttribute(_T("style"), _T("listitem_style"));
			if (it->GetFromStationTelecode() == it->GetStartStationTelecode())
				tmpString = _T("(始)");
			else
				tmpString = _T("(过)");

			pTxtUI->SetText(tmpString + it->GetFromStationName());
			pStationVLayoutUI->Add(pTxtUI);

			///////

			pTxtUI = new CLabelUI();
			pTxtUI->SetManager(&m_pm, NULL, false);
			pTxtUI->SetAttribute(_T("style"), _T("listitem_style"));

			if (it->GetToStationTelecode() == it->GetEndStationTelecode())
				tmpString = _T("(终)");
			else
				tmpString = _T("(过)");

			pTxtUI->SetText(tmpString + it->GetToStationName());
			pStationVLayoutUI->Add(pTxtUI);

			pListItem->Add(pStationVLayoutUI);
		}

		{///出发时间  到达时间
			CVerticalLayoutUI *pTimeVLayoutUI = new CVerticalLayoutUI();
			CLabelUI *pTxtUI = new CLabelUI();
			pTxtUI->SetManager(&m_pm, NULL, false);
			pTxtUI->SetAttribute(_T("style"), _T("listitem_style"));

			pTxtUI->SetText(it->GetStartTime());
			pTimeVLayoutUI->Add(pTxtUI);

			pTxtUI = new CLabelUI();
			pTxtUI->SetManager(&m_pm, NULL, false);
			pTxtUI->SetAttribute(_T("style"), _T("listitem_style"));

			pTxtUI->SetText(it->GetArriveTime());
			pTimeVLayoutUI->Add(pTxtUI);

			pListItem->Add(pTimeVLayoutUI);


		}
		{///历时

			CVerticalLayoutUI *pLiShiVLayoutUI = new CVerticalLayoutUI();
			CLabelUI *pTxtUI = new CLabelUI();
			pTxtUI->SetManager(&m_pm, NULL, false);
			pTxtUI->SetAttribute(_T("style"), _T("listitem_style"));

			pTxtUI->SetText(it->GetLiShi());
			pLiShiVLayoutUI->Add(pTxtUI);

			pTxtUI = new CLabelUI();
			pTxtUI->SetManager(&m_pm, NULL, false);
			pTxtUI->SetAttribute(_T("style"), _T("listitem_style"));
			pTxtUI->SetText(m_transLiShi[_wtoi(it->GetDayDifferent().GetData())]);
			pLiShiVLayoutUI->Add(pTxtUI);

			pListItem->Add(pLiShiVLayoutUI);
		}

		{///商务座
			CLabelUI *pTxtUI = new CLabelUI();
			pTxtUI->SetManager(&m_pm, NULL, false);
			pTxtUI->SetAttribute(_T("style"), _T("listitem_style"));
			if (it->GetSwzNum().Compare(_T("--")) && it->GetSwzNum().Compare(_T("无")))
			{
				pTxtUI->SetFont(2);
				pTxtUI->SetTextColor(0xFF00AA00);
			}
			pTxtUI->SetText(it->GetSwzNum());
			pListItem->Add(pTxtUI);
		}

		{///特等座
			CLabelUI *pTxtUI = new CLabelUI();
			pTxtUI->SetManager(&m_pm, NULL, false);
			pTxtUI->SetAttribute(_T("style"), _T("listitem_style"));
			if (it->GetTzNum().Compare(_T("--")) && it->GetTzNum().Compare(_T("无")))
			{
				pTxtUI->SetFont(2);

				pTxtUI->SetTextColor(0xFF00AA00);
			}
			pTxtUI->SetText(it->GetTzNum());
			pListItem->Add(pTxtUI);
		}

		{///一等座
			CLabelUI *pTxtUI = new CLabelUI();
			pTxtUI->SetManager(&m_pm, NULL, false);
			pTxtUI->SetAttribute(_T("style"), _T("listitem_style"));
			if (it->GetZyNum().Compare(_T("--")) && it->GetZyNum().Compare(_T("无")))
			{
				pTxtUI->SetFont(2);
				pTxtUI->SetTextColor(0xFF00AA00);
			}
			pTxtUI->SetText(it->GetZyNum());
			pListItem->Add(pTxtUI);
		}

		{///二等座
			CLabelUI *pTxtUI = new CLabelUI();
			pTxtUI->SetManager(&m_pm, NULL, false);
			pTxtUI->SetAttribute(_T("style"), _T("listitem_style"));
			if (it->GetZeNum().Compare(_T("--")) && it->GetZeNum().Compare(_T("无")))
			{
				pTxtUI->SetFont(2);
				pTxtUI->SetTextColor(0xFF00AA00);
			}
			pTxtUI->SetText(it->GetZeNum());
			pListItem->Add(pTxtUI);
		}

		{///高级软卧
			CLabelUI *pTxtUI = new CLabelUI();
			pTxtUI->SetManager(&m_pm, NULL, false);
			pTxtUI->SetAttribute(_T("style"), _T("listitem_style"));
			if (it->GetGrNum().Compare(_T("--")) && it->GetGrNum().Compare(_T("无")))
			{
				pTxtUI->SetFont(2);
				pTxtUI->SetTextColor(0xFF00AA00);
			}
			pTxtUI->SetText(it->GetGrNum());
			pListItem->Add(pTxtUI);
		}

		{///软卧
			CLabelUI *pTxtUI = new CLabelUI();
			pTxtUI->SetManager(&m_pm, NULL, false);
			pTxtUI->SetAttribute(_T("style"), _T("listitem_style"));
			if (it->GetRwNum().Compare(_T("--")) && it->GetRwNum().Compare(_T("无")))
			{
				pTxtUI->SetFont(2);
				pTxtUI->SetTextColor(0xFF00AA00);
			}
			pTxtUI->SetText(it->GetRwNum());
			pListItem->Add(pTxtUI);
		}

		{///硬卧
			CLabelUI *pTxtUI = new CLabelUI();
			pTxtUI->SetManager(&m_pm, NULL, false);
			pTxtUI->SetAttribute(_T("style"), _T("listitem_style"));
			if (it->GetYwNum().Compare(_T("--")) && it->GetYzNum().Compare(_T("无")))
			{
				pTxtUI->SetFont(2);
				pTxtUI->SetTextColor(0xFF00AA00);
			}
			pTxtUI->SetText(it->GetYwNum());
			pListItem->Add(pTxtUI);
		}

		{///软座
			CLabelUI *pTxtUI = new CLabelUI();
			pTxtUI->SetManager(&m_pm, NULL, false);
			pTxtUI->SetAttribute(_T("style"), _T("listitem_style"));
			if (it->GetRzNum().Compare(_T("--")) && it->GetYzNum().Compare(_T("无")))
			{
				pTxtUI->SetFont(2);
				pTxtUI->SetTextColor(0xFF00AA00);
			}
			pTxtUI->SetText(it->GetRzNum());
			pListItem->Add(pTxtUI);
		}

		{///硬座
			CLabelUI *pTxtUI = new CLabelUI();
			pTxtUI->SetManager(&m_pm, NULL, false);
			pTxtUI->SetAttribute(_T("style"), _T("listitem_style"));
			if (it->GetYzNum().Compare(_T("--")) && it->GetYzNum().Compare(_T("无")))
			{
				pTxtUI->SetFont(2);
				pTxtUI->SetTextColor(0xFF00AA00);
			}
			pTxtUI->SetText(it->GetYzNum());
			pListItem->Add(pTxtUI);
		}

		{///无座
			CLabelUI *pTxtUI = new CLabelUI();
			pTxtUI->SetManager(&m_pm, NULL, false);
			pTxtUI->SetAttribute(_T("style"), _T("listitem_style"));
			if (it->GetWzNum().Compare(_T("--")) && it->GetWzNum().Compare(_T("无")))
			{
				pTxtUI->SetFont(2);
				pTxtUI->SetTextColor(0xFF00AA00);
			}
			pTxtUI->SetText(it->GetWzNum());
			pListItem->Add(pTxtUI);
		}

		{///其它
			CLabelUI *pTxtUI = new CLabelUI();
			pTxtUI->SetManager(&m_pm, NULL, false);
			pTxtUI->SetAttribute(_T("style"), _T("listitem_style"));
			if (it->GetQtNum().Compare(_T("--")) && it->GetQtNum().Compare(_T("无")))
			{
				pTxtUI->SetFont(2);
				pTxtUI->SetTextColor(0xFF00AA00);
			}
			pTxtUI->SetText(it->GetQtNum());
			pListItem->Add(pTxtUI);
		}

		{///备注
			if (it->GetBtnTextInfo() == _T("预订"))
			{

				CButtonUI *pBtnUI = new CButtonUI();
				CDuiString strIndex;
				strIndex.SmallFormat(_T("%d"), iIndex);

				pBtnUI->SetName(_T("OrderTicketBtn"));
				pBtnUI->SetManager(&m_pm, NULL, false);
				pBtnUI->SetAttribute(_T("style"), _T("btn_style"));
				pBtnUI->SetFixedHeight(30);
				pBtnUI->SetFixedWidth(50);


				pBtnUI->SetUserData(strIndex);

				pBtnUI->SetText(_T("预订"));

				pListItem->Add(pBtnUI);
			}
			else
			{
				CLabelUI *pTxtUI = new CLabelUI();
				pTxtUI->SetManager(&m_pm, NULL, false);
				pTxtUI->SetAttribute(_T("style"), _T("listitem_style"));
				pTxtUI->SetFont(2);
				pTxtUI->SetTextColor(0xFFFFFFFF);
				pTxtUI->SetText(it->GetBtnTextInfo());
				pListItem->Add(pTxtUI);
			}

		}


	}

	return SUCCESS;
}


bool CMainFrame::IsShowTrain(CDuiString trainNo)
{
	_TRAIN_TYPE tmpType;
	switch (trainNo.GetAt(0))
	{
	case 'G':
		tmpType = _GC;
		break;
	case 'D':
		tmpType = _D;
		break;
	case 'Z':
		tmpType = _Z;
		break;
	case 'T':
		tmpType = _T;
		break;
	case 'K':
		tmpType = _K;
		break;
	}


	std::set< _TRAIN_TYPE >::iterator it = m_setShowTrainType.find(tmpType);
	if (it == m_setShowTrainType.end())
		return false;
	
	
	return true;
}


void CMainFrame::RefreshAllTrainCHeckBox(bool flag)
{
	CDuiString trainAll[] = { _T("train_gc"),_T("train_d"), _T("train_z"), _T("train_t"), _T("train_k"),_T("train_o") };
	for (int i = 0; i < 6; ++i)
	{
		CCheckBoxUI *pCheckBoxUI = static_cast<CCheckBoxUI*>(m_pm.FindControl(trainAll[i]));
		pCheckBoxUI->SetCheck(flag);
	}

	if (flag)
	{
		m_setShowTrainType.insert(_GC);
		m_setShowTrainType.insert(_D);
		m_setShowTrainType.insert(_Z);
		m_setShowTrainType.insert(_K);
		m_setShowTrainType.insert(_T);
		m_setShowTrainType.insert(_O);
	}
	else
		m_setShowTrainType.clear();
}



///////////////////////
CWorker::CWorker()
{

}
CWorker::~CWorker()
{

}

void CWorker::run()
{
	while (1)
	{
		m_vecTicket->clear();
		Client12306Manager::Instance()->QueryLeftTicket(UnicodeToUtf8(m_strBegPlace.GetData()),
														UnicodeToUtf8(m_strEndPlace.GetData()),
														UnicodeToUtf8(m_strTravelTime.GetData()), 
																		*m_vecTicket, m_ticketType);


		m_progressDlg->Close(0);
		break;

	}
}


///参考12306订单ui
int CMainFrame::RefreshOrderListView()
{
	CListUI* pOderListView = static_cast<CListUI*>(m_pm.FindControl(_T("orderListView")));

	pOderListView->RemoveAll();

	
	{
		for (int i = 0; i < 2; ++i)
		{
			CDuiString  btnChkName;
			btnChkName.Format(_T("ORDER_TICKET_%d"), i);

			///添加行
			CListContainerElementUI* pListItem = new CListContainerElementUI();
			pListItem->SetChildVAlign(DT_VCENTER);
			pListItem->SetFixedHeight(20);
			pListItem->SetManager(&m_pm, NULL, false);

			pOderListView->Add(pListItem);



			///
			CVerticalLayoutUI* tmpVLayout = new CVerticalLayoutUI();
			tmpVLayout->SetAttribute(_T("hscrollbar"), _T("true"));
			tmpVLayout->SetAttribute(_T("bordersize"), _T("1,1,1,1"));
			tmpVLayout->SetAttribute(_T("bordercolor"), _T("#F"));
			tmpVLayout->SetAttribute(_T("inset"), _T("2,2,2,2"));

			pListItem->Add(tmpVLayout);

			{
				CHorizontalLayoutUI* tmpHLayout = new CHorizontalLayoutUI();
				tmpHLayout->SetAttribute(_T("height"), _T("20"));
				tmpHLayout->SetAttribute(_T("bkcolor"), _T("#FFB4EEB4"));
				tmpHLayout->SetAttribute(_T("inset"), _T("2,2,2,2"));

				tmpVLayout->Add(tmpHLayout);

				{
					///展开按钮
					CCheckBoxUI *checkBox = new CCheckBoxUI();

					checkBox->SetAttribute(_T("width"), _T("20"));
					checkBox->SetAttribute(_T("normalimage"), _T("file='common/scrollbar.bmp' source='0,90,15,105' dest='0,0,20,20'"));
					checkBox->SetAttribute(_T("selectedimage"), _T("file='common/scrollbar.bmp' source='0,109,15,124' dest='0,0,20,20'"));
					checkBox->SetUserData(btnChkName);
					checkBox->SetName(_T("ORDER_EXPAND_BTN"));

					tmpHLayout->Add(checkBox);

					/////订单日期标签
					CLabelUI *txtOrderDate = new CLabelUI();
					txtOrderDate->SetText(_T("订单日期:"));
					txtOrderDate->SetAttribute(_T("width"), _T("80"));
					txtOrderDate->SetAttribute(_T("align"), _T("left"));
					tmpHLayout->Add(txtOrderDate);

					/////订单日期内容
					CLabelUI *txtOrderDateCont = new CLabelUI();
					txtOrderDateCont->SetText(_T("2012-12-12"));
					txtOrderDateCont->SetAttribute(_T("width"), _T("100"));
					txtOrderDateCont->SetAttribute(_T("align"), _T("left"));
					txtOrderDateCont->SetAttribute(_T("font"), _T("2"));
					tmpHLayout->Add(txtOrderDateCont);


					/////订单编号
					CLabelUI *txtOrderNo = new CLabelUI();
					txtOrderNo->SetText(_T("订单编号:"));
					txtOrderNo->SetAttribute(_T("width"), _T("80"));
					txtOrderNo->SetAttribute(_T("align"), _T("left"));
					tmpHLayout->Add(txtOrderNo);

					/////订单编号内容
					CLabelUI *txtOrderNoCont = new CLabelUI();
					txtOrderNoCont->SetText(_T("OX201212121631001"));
					txtOrderNoCont->SetAttribute(_T("width"), _T("180"));
					txtOrderNoCont->SetAttribute(_T("align"), _T("left"));
					txtOrderNoCont->SetAttribute(_T("font"), _T("2"));
					tmpHLayout->Add(txtOrderNoCont);


					/////订票人
					CLabelUI *txtOrderMen = new CLabelUI();
					txtOrderMen->SetText(_T("chengliang"));
					txtOrderMen->SetAttribute(_T("width"), _T("150"));
					txtOrderMen->SetAttribute(_T("align"), _T("left"));
					txtOrderMen->SetAttribute(_T("font"), _T("2"));
					tmpHLayout->Add(txtOrderMen);

					/////始发站
					CLabelUI *txtBegStation = new CLabelUI();
					txtBegStation->SetText(_T("北京西"));
					txtBegStation->SetAttribute(_T("width"), _T("60"));
					txtBegStation->SetAttribute(_T("align"), _T("left"));
					txtBegStation->SetAttribute(_T("font"), _T("2"));
					tmpHLayout->Add(txtBegStation);


					/////箭头
					CLabelUI *txtArrow = new CLabelUI();
					txtArrow->SetText(_T("=>"));
					txtArrow->SetAttribute(_T("width"), _T("30"));
					txtArrow->SetAttribute(_T("align"), _T("center"));
					txtBegStation->SetAttribute(_T("font"), _T("2"));
					tmpHLayout->Add(txtArrow);


					/////结束站
					CLabelUI *txtEndStation = new CLabelUI();
					txtEndStation->SetText(_T("石家庄"));
					txtEndStation->SetAttribute(_T("width"), _T("60"));
					txtEndStation->SetAttribute(_T("align"), _T("left"));
					txtEndStation->SetAttribute(_T("font"), _T("2"));
					tmpHLayout->Add(txtEndStation);

					/////乘车日期
					CLabelUI *txtTravelDate = new CLabelUI();
					txtTravelDate->SetText(_T("乘车日期:"));
					txtTravelDate->SetAttribute(_T("width"), _T("100"));
					txtTravelDate->SetAttribute(_T("align"), _T("right"));
					txtTravelDate->SetAttribute(_T("font"), _T("2"));
					tmpHLayout->Add(txtTravelDate);

					/////乘车日期内容
					CLabelUI *txtTravelDateCont = new CLabelUI();
					txtTravelDateCont->SetText(_T("2016-12-12"));
					txtTravelDateCont->SetAttribute(_T("width"), _T("100"));
					txtTravelDateCont->SetAttribute(_T("align"), _T("left"));
					txtTravelDateCont->SetAttribute(_T("font"), _T("2"));
					tmpHLayout->Add(txtTravelDateCont);
				}

				CLabelUI *txtSpeLine = new CLabelUI();
				txtSpeLine->SetAttribute(_T("height"), _T("2"));
				txtSpeLine->SetAttribute(_T("bkimage"), _T("common/hor_line.png"));

				tmpVLayout->Add(txtSpeLine);

				////
				CListUI *lstTickUI = new CListUI();

				lstTickUI->SetAttribute(_T("vscrollbar"), _T("true"));
				lstTickUI->SetAttribute(_T("hscrollbar"), _T("true"));
				lstTickUI->SetAttribute(_T("itemvalign"), _T("vcenter"));

				lstTickUI->SetName(btnChkName);
				lstTickUI->SetVisible(false);

				tmpVLayout->Add(lstTickUI);




				{

					CListHeaderUI  *headUI = new CListHeaderUI();
					headUI->SetAttribute(_T("height"), _T("20"));
					lstTickUI->Add(headUI);

					{
						CListHeaderItemUI *headerItemUI = new CListHeaderItemUI();
						headerItemUI->SetText(_T("车次信息"));
						headerItemUI->SetFixedWidth(150);
						headerItemUI->SetHotImage(_T("res='common/list_header_hot.png'"));
						headerItemUI->SetPushedImage(_T("res='common/list_header_pushed.png'"));
						headerItemUI->SetSepImage(_T("res='common/list_header_sep.png'"));
						headerItemUI->SetSepWidth(1);
						headerItemUI->SetAttribute(_T("align"), _T("center"));
						headUI->Add(headerItemUI);


						headerItemUI = new CListHeaderItemUI();
						headerItemUI->SetText(_T("席位信息"));
						headerItemUI->SetFixedWidth(150);
						headerItemUI->SetHotImage(_T("res='common/list_header_hot.png'"));
						headerItemUI->SetPushedImage(_T("res='common/list_header_pushed.png'"));
						headerItemUI->SetSepImage(_T("res='common/list_header_sep.png'"));
						headerItemUI->SetSepWidth(1);
						headerItemUI->SetAttribute(_T("align"), _T("center"));
						headUI->Add(headerItemUI);

						headerItemUI = new CListHeaderItemUI();
						headerItemUI->SetText(_T("旅客信息"));
						headerItemUI->SetFixedWidth(150);
						headerItemUI->SetHotImage(_T("res='common/list_header_hot.png'"));
						headerItemUI->SetPushedImage(_T("res='common/list_header_pushed.png'"));
						headerItemUI->SetSepImage(_T("res='common/list_header_sep.png'"));
						headerItemUI->SetSepWidth(1);
						headerItemUI->SetAttribute(_T("align"), _T("center"));
						headUI->Add(headerItemUI);

						headerItemUI = new CListHeaderItemUI();
						headerItemUI->SetText(_T("票款金额"));
						headerItemUI->SetFixedWidth(100);
						headerItemUI->SetHotImage(_T("res='common/list_header_hot.png'"));
						headerItemUI->SetPushedImage(_T("res='common/list_header_pushed.png'"));
						headerItemUI->SetSepImage(_T("res='common/list_header_sep.png'"));
						headerItemUI->SetSepWidth(1);
						headerItemUI->SetAttribute(_T("align"), _T("center"));
						headUI->Add(headerItemUI);

						headerItemUI = new CListHeaderItemUI();
						headerItemUI->SetText(_T("车票状态"));
						headerItemUI->SetFixedWidth(100);
						headerItemUI->SetHotImage(_T("res='common/list_header_hot.png'"));
						headerItemUI->SetPushedImage(_T("res='common/list_header_pushed.png'"));
						headerItemUI->SetSepImage(_T("res='common/list_header_sep.png'"));
						headerItemUI->SetSepWidth(1);
						headerItemUI->SetAttribute(_T("align"), _T("center"));
						headUI->Add(headerItemUI);

						headerItemUI = new CListHeaderItemUI();
						headerItemUI->SetText(_T("操作"));
						headerItemUI->SetFixedWidth(150);
						headerItemUI->SetHotImage(_T("res='common/list_header_hot.png'"));
						headerItemUI->SetPushedImage(_T("res='common/list_header_pushed.png'"));
						headerItemUI->SetSepImage(_T("res='common/list_header_sep.png'"));
						headerItemUI->SetSepWidth(1);
						headerItemUI->SetAttribute(_T("align"), _T("center"));
						headUI->Add(headerItemUI);
					}

				}

			}
		}
	}

	return SUCCESS;
}
