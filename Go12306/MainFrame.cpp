#include "stdafx.h"
#include "MainFrame.h"
#include "Resource.h"
#include "MsgWnd.h"


CControlUI* CMainFrame::CreateControl(LPCTSTR pstrClass)
{
	
	return NULL;
}

void CMainFrame::InitWindow()
{
	// �����Խӿ�
	CResourceManager::GetInstance()->SetTextQueryInterface(this);
	CResourceManager::GetInstance()->LoadLanguage(_T("lan_cn.xml"));
	// Ƥ���ӿ�
	CSkinManager::GetSkinManager()->AddReceiver(this);



	// ע������ͼ��
	m_trayIcon.CreateTrayIcon(m_hWnd, IDR_MAINFRAME, _T("gogo 12306"));
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
	return _T("MainWnd");
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
			return _T("Duilib ʹ����ʾ v1.1");
		}
		else if (lstrcmpi(lpstrId, _T("hometext")) == 0) {
			return _T("{a}��ʾ����{/a}");
		}
	}

	return NULL;
}

LRESULT CMainFrame::OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
{
	m_trayIcon.DeleteTrayIcon();
	bHandled = FALSE;
	// �˳�����
	PostQuitMessage(0);
	return 0;
}
LRESULT CMainFrame::HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	// �رմ��ڣ��˳�����
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

			if (sMenuName.CompareNoCase(_T("lan")) == 0)
			{
				static bool bEn = false;
				if (!bEn) {
					CResourceManager::GetInstance()->SetLanguage(_T("en"));
					CResourceManager::GetInstance()->LoadLanguage(_T("lan_en.xml"));
				}
				else {
					CResourceManager::GetInstance()->SetLanguage(_T("cn_zh"));
					CResourceManager::GetInstance()->LoadLanguage(_T("lan_cn.xml"));
				}
				bEn = !bEn;
				CResourceManager::GetInstance()->ReloadText();
				InvalidateRect(m_hWnd, NULL, TRUE);
				m_pm.NeedUpdate();
			}
			else if (sMenuName == _T("qianting"))
			{
				if (bChecked)
				{
					CMsgWnd::MessageBox(m_hWnd, NULL, _T("��Ԥ����Ǳͧ����"));
				}
				else
				{
					CMsgWnd::MessageBox(m_hWnd, NULL, _T("��ȡ����Ǳͧ����"));
				}
			}
			else if (sMenuName == _T("exit")) {
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
			// ��̬��Ӻ��������ò˵��Ĵ�С
			m_pMenu->ResizeMenu();
		}
	}
	else if (uMsg == WM_DPICHANGED) {
		m_pm.SetDPI(LOWORD(wParam));  // Set the new DPI, retrieved from the wParam
		m_pm.ResetDPIAssets();
		RECT* const prcNewWindow = (RECT*)lParam;
		SetWindowPos(m_hWnd, NULL, prcNewWindow->left, prcNewWindow->top, prcNewWindow->right - prcNewWindow->left, prcNewWindow->bottom - prcNewWindow->top, SWP_NOZORDER | SWP_NOACTIVATE);
		if (m_pm.GetRoot() != NULL) m_pm.GetRoot()->NeedUpdate();
	}
	bHandled = FALSE;
	return 0;
}