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
#include "OrderManagerUI.h"
#include "TicketManager.h"







CMainFrame::CMainFrame()
:m_bAllTrainType(true),
 m_tWorker(new CTicketWorker(this)),
 m_tOrderWorker(new COrderWorker(this)),
 m_pOrderManagerUI(NULL),
 m_pTicketManagerUI(NULL)
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

	m_pBegPlaceCombo = static_cast<CComboUI*>(m_pm.FindControl(_T("begPlace")));

	
	CListLabelElementUI *b = new CListLabelElementUI();
	b->SetText(_T("AAAAAA"));
	m_pBegPlaceCombo->Add(b);
	////

	Client12306Manager::Instance()->LoginInit();

	Client12306Manager::Instance()->Query12306StationName();

	m_pOrderManagerUI = new COrderManagerUI(this);

	m_pTicketManagerUI = new CTicketManager(this);

	RefreshAllTrainCHeckBox(m_bAllTrainType);

	/*CLoginWnd* pLogin = new CLoginWnd();
	pLogin->Create(NULL, _T("LoginWnd"), WS_POPUP | WS_CLIPCHILDREN, WS_EX_TOOLWINDOW);
	pLogin->CenterWindow();


	switch (pLogin->ShowModal())
	{
	case MSGID_OK:
		break;
	case MSGID_CANCEL:
		PostQuitMessage(0L);
		break;

	}
	*/
	

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
	else if (uMsg == WM_PROGRESS_CLOSE)
	{
		bHandled = TRUE;
		if (m_pProgressDlg)
		{
			m_pProgressDlg->Close();

			m_pProgressDlg = NULL;
		}
		return 0;

	}
	else if (uMsg == WM_ORDER_QUERY)
	{
		bHandled = TRUE;
		if ((int)wParam != SUCCESS)
		{
			CMsgWnd::MessageBox(GetHWND(), _T("提示"), Utf8ToUnicode(Client12306Manager::Instance()->GetLastErrInfo()).c_str());
		}
		else
			RefreshMyOrderListView();



		return 0;
	}
	else if (uMsg == WM_TICKET_QUERY)
	{
		bHandled = TRUE; 
		if ((int)wParam != SUCCESS)
		{
			CMsgWnd::MessageBox(GetHWND(), _T("提示"), Utf8ToUnicode(Client12306Manager::Instance()->GetLastErrInfo()).c_str());
		}
		else
			RefreshTicketListView();

		return 0;

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

			m_pOrderManagerUI->RefreshOrderDetailList(pLstTicket, pOrderExpandBtn->GetCheck());

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
	else if (sName.CompareNoCase(_T("btnOrderQuery")) == 0)
	{
		QueryMyOrder();

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
	
	
	
	
}


int CMainFrame::QueryTicket(CDuiString begPlace, CDuiString endPlace, CDuiString travelTime )
{


	begPlace = _T("SJP");
	endPlace = _T("BJP");
	travelTime = _T("2016-12-15");

	m_pProgressDlg = CProgressDlg::CreateDlg(this->GetHWND());
	
	

	m_tWorker->SetVecTicket(&m_vecTicket);
	m_tWorker->SetQueryParam(begPlace, endPlace, travelTime,_ADULT);
	m_tpWorker.start(*m_tWorker);
	m_pProgressDlg->ShowModal();

		
	return 0;
}


int CMainFrame::RefreshTicketListView()
{
	m_pTicketManagerUI->ResetTicketList();

	int iIndex = 0;
	for (std::vector<CTicketModel>::iterator it = m_vecTicket.begin(); it != m_vecTicket.end(); ++it, ++iIndex)
	{
		DUI__Trace(it->GetTrainNo());

		if (!IsShowTrain(it->GetStationTrainCode()))
			continue;

		m_pTicketManagerUI->RefreshTicketListView(&(*it) , iIndex);

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

int CMainFrame::QueryMyOrder()
{
	CDuiString begDate = _T("2016-12-01");
	CDuiString endDate = _T("2016-12-14");
	CDuiString seqTrainName = _T("");

	
	m_pProgressDlg = CProgressDlg::CreateDlg(this->GetHWND());


	m_tOrderWorker->SetMapOrder(&m_mapMyOrder);
	m_tOrderWorker->SetQueryParam(begDate, endDate, seqTrainName);
	m_tpWorker.start(*m_tOrderWorker);

	m_pProgressDlg->ShowModal();

	

	return SUCCESS;
}


int CMainFrame::RefreshMyOrderListView()
{

	m_pOrderManagerUI->RefreshOrderListView(m_mapMyOrder);

	return SUCCESS;
}

///////////////////////
CTicketWorker::CTicketWorker(CMainFrame *mainFrame)
	:m_mainFrame(mainFrame)
{

}
CTicketWorker::~CTicketWorker()
{

}

void CTicketWorker::run()
{
	while (1)
	{
		m_vecTicket->clear();


		int iRetFlag = Client12306Manager::Instance()->QueryLeftTicket(UnicodeToUtf8(m_strBegPlace.GetData()),
														UnicodeToUtf8(m_strEndPlace.GetData()),
														UnicodeToUtf8(m_strTravelTime.GetData()), 
																		*m_vecTicket, m_ticketType);


		SendMessage(m_mainFrame->GetHWND(), WM_PROGRESS_CLOSE, NULL, NULL);
		SendMessage(m_mainFrame->GetHWND(), WM_TICKET_QUERY, iRetFlag, NULL);

		break;

	}
}


///////////////////////
COrderWorker::COrderWorker(CMainFrame *mainFrame)
	:m_mainFrame(mainFrame)
{

}
COrderWorker::~COrderWorker()
{

}

void COrderWorker::run()
{
	while (1)
	{
		

		
		int iRetFlag = Client12306Manager::Instance()->QueryMyOrder(UnicodeToUtf8(m_strBegDate.GetData()),
			UnicodeToUtf8(m_strEndDate.GetData()),
			UnicodeToUtf8(m_strSeqTrainName.GetData()) , *m_mapTicket);


		SendMessage(m_mainFrame->GetHWND(), WM_PROGRESS_CLOSE, NULL, NULL);

		
		SendMessage(m_mainFrame->GetHWND(), WM_ORDER_QUERY, iRetFlag, NULL);

		break;

	}
}


