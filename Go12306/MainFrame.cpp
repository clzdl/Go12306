#include "stdafx.h"
#include "MainFrame.h"
#include "Resource.h"
#include "MsgWnd.h"
#include "SkinFrame.h"
#include <sstream>
#include "OthFunc.h"

#include "Poco/Net/HTTPSClientSession.h"
#include "Poco/Net/HTTPRequest.h"
#include "Poco/Net/HTTPResponse.h"
#include "Poco/Net/SecureStreamSocket.h"
#include "Poco/Net/Context.h"
#include "Poco/Net/Session.h"
#include "Poco/Net/SSLManager.h"
#include "Poco/Net/SSLException.h"
#include "Poco/SharedPtr.h"

#include "Poco/Net/SSLManager.h"
#include "Poco/Net/KeyConsoleHandler.h"
#include "Poco/Net/ConsoleCertificateHandler.h"
#include "Poco/Net/InvalidCertificateHandler.h"
#include "Poco/Net/AcceptCertificateHandler.h"


#include "Poco/StreamCopier.h"
#include "Poco/Exception.h"


using Poco::StreamCopier;
using Poco::Net::Context;
using Poco::Net::HTTPSClientSession;
using Poco::Net::HTTPRequest;
using Poco::Net::HTTPResponse;
using Poco::SharedPtr;
using Poco::Net::InvalidCertificateHandler;
using Poco::Net::SSLManager;
using Poco::Net::KeyConsoleHandler;
using Poco::Net::ConsoleCertificateHandler;
using Poco::Net::ConsoleCertificateHandler;





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
		else if (name.CompareNoCase(_T("train_all")) == 0 )
		{
			CDuiString trainAll[] = {_T("train_gc"),_T("train_d"), _T("train_z"), _T("train_t"), _T("train_k"),_T("train_o") };
			for (int i = 0; i < 6; ++i)
			{
				CCheckBoxUI *pCheckBoxUI =  static_cast<CCheckBoxUI*>(m_pm.FindControl(trainAll[i]));
				pCheckBoxUI->SetCheck(!pCheckBoxUI->GetCheck());
			}
		}
		else if (name.CompareNoCase(_T("train_gc")) == 0)
		{
		}
		else if (name.CompareNoCase(_T("train_d")) == 0)
		{
		}
		else if (name.CompareNoCase(_T("train_z")) == 0)
		{
		}
		else if (name.CompareNoCase(_T("train_t")) == 0)
		{
		}
		else if (name.CompareNoCase(_T("train_k")) == 0)
		{
		}
		else if (name.CompareNoCase(_T("train_o")) == 0)
		{
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
	
}


int CMainFrame::QueryTicket(CDuiString begPlace, CDuiString endPlace, CDuiString travelTime)
{
	
	CDuiString strDomain = _T("kyfw.12306.cn");
	CDuiString strUrl = _T("/otn/leftTicket/queryX?leftTicketDTO.train_date=2016-12-12&leftTicketDTO.from_station=SJP&leftTicketDTO.to_station=BXP&purpose_codes=0X00");


	try
	{

		SharedPtr<InvalidCertificateHandler> ptrCert = new ConsoleCertificateHandler(false);
		Context::Ptr ptrContext = new Context(Context::CLIENT_USE, "", "", "", Context::VERIFY_NONE, 9, true, "ALL:!ADH:!LOW:!EXP:!MD5:@STRENGTH");
		SSLManager::instance().initializeClient(0, ptrCert, ptrContext);

		HTTPSClientSession s("kyfw.12306.cn", 443);
		HTTPRequest request(HTTPRequest::HTTP_GET, "/otn/leftTicket/queryX?leftTicketDTO.train_date=2016-12-12&leftTicketDTO.from_station=SJP&leftTicketDTO.to_station=BXP&purpose_codes=0X00");
		s.sendRequest(request);
		HTTPResponse response;
		std::istream& rs = s.receiveResponse(response);
		std::ostringstream ostr;
		StreamCopier::copyStream(rs, ostr);
		DUI__Trace(_T("%s\n"), StringToWString(ostr.str()).c_str());
	}
	catch (Poco::Exception &e)
	{
		DUI__Trace(_T("%s\n"), StringToWString(e.displayText()).c_str());
	}



	
		
	return 0;
}