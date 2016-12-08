#include "stdafx.h"
#include "MainFrame.h"
#include "Resource.h"
#include "MsgWnd.h"
#include "SkinFrame.h"
#include <sstream>
#include "OthFunc.h"


#include "Client12306Manager.h"

#include "TicketModel.h"

CDuiString CMainFrame::m_transLiShi[] = { _T("当日到达"),_T("次日到达"),_T("两日到达"),_T("三日到达") };

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

	Client12306Manager::Instance()->QueryLeftTicket(UnicodeToUtf8(begPlace.GetData()) , 
													UnicodeToUtf8(endPlace.GetData()) ,
													UnicodeToUtf8(travelTime.GetData()) , m_vecTicket, _ADULT);


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
				pTxtUI->SetTextColor(0xFF5F5F5F);
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

				pTxtUI->SetText(_T("--"));
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