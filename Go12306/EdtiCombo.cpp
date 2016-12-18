#include "stdafx.h"
#include "EditCombo.h"

namespace DuiLib
{
	/////////////////////////
	class CComboWnd : public CWindowWnd, public INotifyUI
	{
	public:
		void Init(CEditComboUI* pOwner);

		RECT CalcRect();

		void Refresh();


		LPCTSTR GetWindowClassName() const;
		void OnFinalMessage(HWND hWnd);
		

		LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
		void Notify(TNotifyUI& msg) override;

		void EnsureVisible(int iIndex);
		void Scroll(int dx, int dy);

#if(_WIN32_WINNT >= 0x0501)
		virtual UINT GetClassStyle() const;
#endif
		bool IsHitItem(POINT ptMouse);
	public:
		CPaintManagerUI m_pm;
		CEditComboUI* m_pOwner;
		CVerticalLayoutUI* m_pLayout;
		int m_iOldSel;
		bool m_bHitItem;

		RECT m_rcPaint;
	};



	void CComboWnd::Notify(TNotifyUI& msg)
	{
		if (msg.sType == _T("windowinit"))
		{
			EnsureVisible(m_iOldSel);
		}
		else if (msg.sType == _T("click")) {
			// 测试代码
			CDuiString sName = msg.pSender->GetName();
			CControlUI* pCtrl = msg.pSender;
			while (pCtrl != NULL) {
				IListItemUI* pListItem = (IListItemUI*)pCtrl->GetInterface(DUI_CTR_LISTITEM);
				if (pListItem != NULL) {
					break;
				}
				pCtrl = pCtrl->GetParent();
			}
		}
	}

	RECT CComboWnd::CalcRect()
	{
		m_bHitItem = false;
				 
		m_iOldSel = m_pOwner->GetCurSel();

		// Position the popup window in absolute space
		SIZE szDrop = m_pOwner->GetDropBoxSize();
		RECT rcOwner = m_pOwner->GetPos();
		RECT rc = rcOwner;
		rc.top = rc.bottom;		// 父窗口left、bottom位置作为弹出窗口起点
		rc.bottom = rc.top + szDrop.cy;	// 计算弹出窗口高度
		if (szDrop.cx > 0) rc.right = rc.left + szDrop.cx;	// 计算弹出窗口宽度

		SIZE szAvailable = { rc.right - rc.left, rc.bottom - rc.top };
		int cyFixed = 0;
		//for (int it = 0; it < m_pOwner->GetCount(); it++) {
		for (int i = 0; i < m_pOwner->m_vecBoxItem.size(); ++i)
		{
			CControlUI* pControl = static_cast<CControlUI*>(m_pOwner->m_vecBoxItem.at(i));
			if (!pControl->IsVisible()) continue;
			SIZE sz = pControl->EstimateSize(szAvailable);
			cyFixed += sz.cy;
		}
		cyFixed += 4; // CVerticalLayoutUI 默认的Inset 调整
		rc.bottom = rc.top + MIN(cyFixed, szDrop.cy);

		::MapWindowRect(m_pOwner->m_pManager->GetPaintWindow(), HWND_DESKTOP, &rc);

		MONITORINFO oMonitor = {};
		oMonitor.cbSize = sizeof(oMonitor);
		::GetMonitorInfo(::MonitorFromWindow(*this, MONITOR_DEFAULTTOPRIMARY), &oMonitor);
		CDuiRect rcWork = oMonitor.rcWork;
		if (rc.bottom > rcWork.bottom) {
			rc.left = rcOwner.left;
			rc.right = rcOwner.right;
			if (szDrop.cx > 0) rc.right = rc.left + szDrop.cx;
			rc.top = rcOwner.top - MIN(cyFixed, szDrop.cy);
			rc.bottom = rcOwner.top;
			::MapWindowRect(m_pOwner->m_pManager->GetPaintWindow(), HWND_DESKTOP, &rc);
		}

		return rc;
	}

	void CComboWnd::Init(CEditComboUI* pOwner)
	{
		m_pLayout = NULL;
		m_pOwner = pOwner;

		m_rcPaint = CalcRect();

		Create(pOwner->m_pManager->GetPaintWindow(), NULL, WS_POPUP, WS_EX_TOOLWINDOW, m_rcPaint);
		
		Refresh();

		// HACK: Don't deselect the parent's caption
		HWND hWndParent = m_hWnd;
		while (::GetParent(hWndParent) != NULL) hWndParent = ::GetParent(hWndParent);
		
		ShowWindow(true);
		//::ShowWindow(m_hWnd, SW_SHOWNOACTIVATE);
		::SendMessage(hWndParent, WM_NCACTIVATE, TRUE, 0L);
	}


	

	LPCTSTR CComboWnd::GetWindowClassName() const
	{
		return _T("EditComboWnd");
	}

	void CComboWnd::OnFinalMessage(HWND hWnd)
	{
		m_pOwner->m_pComboWindow = NULL;
		m_pOwner->m_uButtonState &= ~UISTATE_PUSHED;
		m_pOwner->Invalidate();
		delete this;
		//::DestroyWindow(m_hWnd);
	}

	bool CComboWnd::IsHitItem(POINT ptMouse)
	{
		CControlUI* pControl = m_pm.FindControl(ptMouse);
		if (pControl != NULL) {
			LPVOID pInterface = pControl->GetInterface(DUI_CTR_SCROLLBAR);
			if (pInterface) return false;

			while (pControl != NULL) {
				IListItemUI* pListItem = (IListItemUI*)pControl->GetInterface(DUI_CTR_LISTITEM);
				if (pListItem != NULL) {
					return true;
				}
				pControl = pControl->GetParent();
			}
		}

		return false;
	}

	void CComboWnd::Refresh()
	{
		m_pLayout->RemoveAll();
	/*	for (int i = 0; i < m_pOwner->GetCount(); i++) {
			m_pLayout->Add(static_cast<CControlUI*>(m_pOwner->GetItemAt(i)));
		}*/

		for (int i = 0; i < m_pOwner->m_vecBoxItem.size(); ++i)
		{
			m_pLayout->Add(static_cast<CControlUI*>(m_pOwner->m_vecBoxItem.at(i)));

		}

		RECT rect = CalcRect();

		::SetWindowPos(m_hWnd , NULL , rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, SWP_NOACTIVATE);

	}

	LRESULT CComboWnd::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		if (uMsg == WM_CREATE) {
			m_pm.SetForceUseSharedRes(true);
			m_pm.Init(m_hWnd);
			// The trick is to add the items to the new container. Their owner gets
			// reassigned by this operation - which is why it is important to reassign
			// the items back to the righfull owner/manager when the window closes.
			m_pLayout = new CVerticalLayoutUI;
			m_pLayout->SetName(_T("ComboVerticalLayout"));
			m_pLayout->SetManager(&m_pm, NULL, true);
			LPCTSTR pDefaultAttributes = m_pOwner->m_pManager->GetDefaultAttributeList(_T("VerticalLayout"));
			if (pDefaultAttributes) {
				m_pLayout->ApplyAttributeList(pDefaultAttributes);
			}
			m_pLayout->SetInset(CDuiRect(1, 1, 1, 1));
			m_pLayout->SetBkColor(0xFFFFFFFF);
			m_pLayout->SetBorderColor(0xFFC6C7D2);
			m_pLayout->SetBorderSize(1);
			m_pLayout->SetAutoDestroy(false);
			m_pLayout->EnableScrollBar();
			m_pLayout->ApplyAttributeList(m_pOwner->GetDropBoxAttributeList());
			for (int i = 0; i < m_pOwner->GetCount(); i++) {
				m_pLayout->Add(static_cast<CControlUI*>(m_pOwner->GetItemAt(i)));
			}
			m_pm.AttachDialog(m_pLayout);
			m_pm.AddNotifier(this);
			return 0;
		}
		else if (uMsg == WM_CLOSE) {
			DUI__Trace(_T("CComboWnd::WM_CLOSE"));
			m_pOwner->SetManager(m_pOwner->GetManager(), m_pOwner->GetParent(), false);
			RECT rcNull = { 0 };
			for (int i = 0; i < m_pOwner->GetCount(); i++) 
				static_cast<CControlUI*>(m_pOwner->GetItemAt(i))->SetPos(rcNull);

		
		}
		else if (uMsg == WM_LBUTTONDOWN) {
			DUI__Trace(_T("CComboWnd::WM_LBUTTONDOWN"));
			POINT pt = { 0 };
			::GetCursorPos(&pt);
			::ScreenToClient(m_pm.GetPaintWindow(), &pt);
			m_bHitItem = IsHitItem(pt);

			

		}
		else if (uMsg == WM_LBUTTONUP) {
			DUI__Trace(_T("CComboWnd::WM_LBUTTONUP"));
			POINT pt = { 0 };
			::GetCursorPos(&pt);
			::ScreenToClient(m_pm.GetPaintWindow(), &pt);
			if (m_bHitItem && IsHitItem(pt)) {
				//PostMessage(WM_KILLFOCUS);
				PostMessage(WM_CLOSE);
				m_pOwner->EditClose();
			}
			
			m_bHitItem = false;
		}
		else if (uMsg == WM_MOUSELEAVE)
		{
			DUI__Trace(_T("CComboWnd::WM_MOUSELEAVE"));

		}
		else if (uMsg == WM_MOUSEHOVER)
		{
			DUI__Trace(_T("CComboWnd::WM_MOUSEHOVER"));

		}
		else if (uMsg == WM_KEYDOWN) {
			switch (wParam) {
			case VK_ESCAPE:
				m_pOwner->SelectItem(m_iOldSel, true);
				EnsureVisible(m_iOldSel);
			case VK_RETURN:
				PostMessage(WM_KILLFOCUS);
				break;
			default:
				TEventUI event;
				event.Type = UIEVENT_KEYDOWN;
				event.chKey = (TCHAR)wParam;
				m_pOwner->DoEvent(event);
				EnsureVisible(m_pOwner->GetCurSel());

				return 0;
			}
		}
		else if (uMsg == WM_MOUSEWHEEL) {
			int zDelta = (int)(short)HIWORD(wParam);
			TEventUI event = { 0 };
			event.Type = UIEVENT_SCROLLWHEEL;
			event.wParam = MAKELPARAM(zDelta < 0 ? SB_LINEDOWN : SB_LINEUP, 0);
			event.lParam = lParam;
			event.dwTimestamp = ::GetTickCount();
			if (m_pOwner->GetScrollSelect()) {
				m_pOwner->DoEvent(event);
				EnsureVisible(m_pOwner->GetCurSel());
				return 0;
			}
			else {
				m_pLayout->DoEvent(event);
				
				return 0;
			}
		
		}
		else if (uMsg == WM_PAINT)
		{
			DUI__Trace(_T("CComboWnd::WM_PAINT"));
			////去掉后不能实时更新
			m_pLayout->NeedUpdate();

		}
		else if (uMsg == WM_KILLFOCUS) {
			//if (m_hWnd != (HWND)wParam) PostMessage(WM_CLOSE);
		
		}



		LRESULT lRes = 0;
		if (m_pm.MessageHandler(uMsg, wParam, lParam, lRes)) return lRes;

		return CWindowWnd::HandleMessage(uMsg, wParam, lParam);
	}

	void CComboWnd::EnsureVisible(int iIndex)
	{
		if (m_pOwner->GetCurSel() < 0) return;
		m_pLayout->FindSelectable(m_pOwner->GetCurSel(), false);
		RECT rcItem = m_pLayout->GetItemAt(iIndex)->GetPos();
		RECT rcList = m_pLayout->GetPos();
		CScrollBarUI* pHorizontalScrollBar = m_pLayout->GetHorizontalScrollBar();
		if (pHorizontalScrollBar && pHorizontalScrollBar->IsVisible()) rcList.bottom -= pHorizontalScrollBar->GetFixedHeight();
		int iPos = m_pLayout->GetScrollPos().cy;
		if (rcItem.top >= rcList.top && rcItem.bottom < rcList.bottom) return;
		int dx = 0;
		if (rcItem.top < rcList.top) dx = rcItem.top - rcList.top;
		if (rcItem.bottom > rcList.bottom) dx = rcItem.bottom - rcList.bottom;
		Scroll(0, dx);
	}

	void CComboWnd::Scroll(int dx, int dy)
	{
		if (dx == 0 && dy == 0) return;
		SIZE sz = m_pLayout->GetScrollPos();
		m_pLayout->SetScrollPos(CDuiSize(sz.cx + dx, sz.cy + dy));
	}

#if(_WIN32_WINNT >= 0x0501)
	UINT CComboWnd::GetClassStyle() const
	{
		return __super::GetClassStyle() | CS_DROPSHADOW;
	}
#endif
	////////////////////////////////////////////////////////



	class CEditWnd : public CWindowWnd
	{
	public:
		CEditWnd();

		void Init(CEditComboUI* pOwner);
		RECT CalPos();

		LPCTSTR GetWindowClassName() const;
		LPCTSTR GetSuperClassName() const;
		void OnFinalMessage(HWND hWnd);

		LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
		LRESULT OnKillFocus(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
		LRESULT OnEditChanged(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	protected:
		CEditComboUI* m_pOwner;
		HBRUSH m_hBkBrush;
		bool m_bInit;
		bool m_bDrawCaret;

		
	};


	CEditWnd::CEditWnd()
		: m_pOwner(NULL),
		m_hBkBrush(NULL),
		m_bInit(false), 
		m_bDrawCaret(false)
	{
	}

	void CEditWnd::Init(CEditComboUI* pOwner)
	{
		m_pOwner = pOwner;
		RECT rcPos = CalPos();
		UINT uStyle = 0;
		if (m_pOwner->m_pManager->IsLayered()) {
			uStyle = WS_POPUP | ES_AUTOHSCROLL | WS_VISIBLE;
			RECT rcWnd = { 0 };
			::GetWindowRect(m_pOwner->m_pManager->GetPaintWindow(), &rcWnd);
			rcPos.left += rcWnd.left;
			rcPos.right += rcWnd.left;
			rcPos.top += rcWnd.top - 1;
			rcPos.bottom += rcWnd.top - 1;
		}
		else {
			uStyle = WS_CHILD | ES_AUTOHSCROLL;
		}
		UINT uTextStyle = m_pOwner->GetTextStyle();
		if (uTextStyle & DT_LEFT) uStyle |= ES_LEFT;
		else if (uTextStyle & DT_CENTER) uStyle |= ES_CENTER;
		else if (uTextStyle & DT_RIGHT) uStyle |= ES_RIGHT;
		if (m_pOwner->IsPasswordMode()) uStyle |= ES_PASSWORD;
		Create(m_pOwner->m_pManager->GetPaintWindow(), NULL, uStyle, 0, rcPos);
		HFONT hFont = NULL;
		int iFontIndex = m_pOwner->GetFont();
		if (iFontIndex != -1)
			hFont = m_pOwner->m_pManager->GetFont(iFontIndex);
		if (hFont == NULL)
			hFont = m_pOwner->m_pManager->GetDefaultFontInfo()->hFont;

		SetWindowFont(m_hWnd, hFont, TRUE);
		Edit_LimitText(m_hWnd, m_pOwner->GetMaxChar());
		if (m_pOwner->IsPasswordMode()) Edit_SetPasswordChar(m_hWnd, m_pOwner->GetPasswordChar());
		Edit_SetText(m_hWnd, m_pOwner->GetText());
		Edit_SetModify(m_hWnd, FALSE);
		SendMessage(EM_SETMARGINS, EC_LEFTMARGIN | EC_RIGHTMARGIN, MAKELPARAM(0, 0));
		Edit_Enable(m_hWnd, m_pOwner->IsEnabled() == true);
		Edit_SetReadOnly(m_hWnd, m_pOwner->IsReadOnly() == true);

		//Styls
		LONG styleValue = ::GetWindowLong(m_hWnd, GWL_STYLE);
		styleValue |= pOwner->GetWindowStyls();
		::SetWindowLong(GetHWND(), GWL_STYLE, styleValue);
		::ShowWindow(m_hWnd, SW_SHOWNOACTIVATE);

		int cchLen = ::GetWindowTextLength(m_hWnd);
		if (cchLen <= 0) cchLen = 1;
		::SetFocus(m_hWnd);
		::SendMessage(m_hWnd, EM_SETSEL, 0, cchLen);
		m_bInit = true;
	}

	RECT CEditWnd::CalPos()
	{
		CDuiRect rcPos = m_pOwner->GetPos();
		RECT rcInset = m_pOwner->GetTextPadding();
		rcPos.left += rcInset.left;
		rcPos.top += rcInset.top;
		rcPos.right -= rcInset.right;
		rcPos.bottom -= rcInset.bottom;
		/*LONG lEditHeight = m_pOwner->m_pManager->GetFontInfo(m_pOwner->GetFont())->tm.tmHeight;
		if (lEditHeight < rcPos.GetHeight()) {
			rcPos.top += (rcPos.GetHeight() - lEditHeight) / 2;
			rcPos.bottom = rcPos.top + lEditHeight;
		}*/

		CControlUI* pParent = m_pOwner;
		RECT rcParent;
		while (pParent = pParent->GetParent()) {
			if (!pParent->IsVisible()) {
				rcPos.left = rcPos.top = rcPos.right = rcPos.bottom = 0;
				break;
			}
			rcParent = pParent->GetClientPos();
			if (!::IntersectRect(&rcPos, &rcPos, &rcParent)) {
				rcPos.left = rcPos.top = rcPos.right = rcPos.bottom = 0;
				break;
			}
		}

		return rcPos;
	}

	LPCTSTR CEditWnd::GetWindowClassName() const
	{
		return _T("EditWnd");
	}

	LPCTSTR CEditWnd::GetSuperClassName() const
	{
		return WC_EDIT;
	}

	void CEditWnd::OnFinalMessage(HWND hWnd)
	{
		
		// Clear reference and die
		if (m_hBkBrush != NULL) ::DeleteObject(m_hBkBrush);
		if (m_pOwner->m_pManager->IsLayered()) {
			m_pOwner->m_pManager->RemovePaintChildWnd(hWnd);
		}
		m_pOwner->m_pWindow = NULL;
		m_pOwner->Invalidate();
		delete this;
	}

	LRESULT CEditWnd::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		LRESULT lRes = 0;
		BOOL bHandled = TRUE;
		if (uMsg == WM_CREATE) {
			if (m_pOwner->m_pManager->IsLayered()) {
				m_pOwner->m_pManager->AddPaintChildWnd(m_hWnd);
				::SetTimer(m_hWnd, CARET_TIMERID, ::GetCaretBlinkTime(), NULL);
			}
			bHandled = FALSE;
		}
		else if (uMsg == WM_KILLFOCUS) lRes = OnKillFocus(uMsg, wParam, lParam, bHandled);
		else if (uMsg == OCM_COMMAND) {
			if (GET_WM_COMMAND_CMD(wParam, lParam) == EN_CHANGE) lRes = OnEditChanged(uMsg, wParam, lParam, bHandled);
			else if (GET_WM_COMMAND_CMD(wParam, lParam) == EN_UPDATE) {
				RECT rcClient;
				::GetClientRect(m_hWnd, &rcClient);
				::InvalidateRect(m_hWnd, &rcClient, FALSE);
			}
		}
		else if (uMsg == WM_KEYDOWN && TCHAR(wParam) == VK_RETURN) {
			m_pOwner->m_pManager->SendNotify(m_pOwner, DUI_MSGTYPE_RETURN);
		}
		else if (uMsg == WM_KEYDOWN && TCHAR(wParam) == VK_TAB) {
			if (m_pOwner->m_pManager->IsLayered()) {
				m_pOwner->m_pManager->SetNextTabControl();
			}
		}

		else if (uMsg == OCM__BASE + WM_CTLCOLOREDIT || uMsg == OCM__BASE + WM_CTLCOLORSTATIC) {
			if (m_pOwner->GetNativeEditBkColor() == 0xFFFFFFFF) return NULL;
			::SetBkMode((HDC)wParam, TRANSPARENT);

			DWORD dwTextColor;
			if (m_pOwner->GetNativeEditTextColor() != 0x000000)
				dwTextColor = m_pOwner->GetNativeEditTextColor();
			else
				dwTextColor = m_pOwner->GetTextColor();

			::SetTextColor((HDC)wParam, RGB(GetBValue(dwTextColor), GetGValue(dwTextColor), GetRValue(dwTextColor)));
			if (m_hBkBrush == NULL) {
				DWORD clrColor = m_pOwner->GetNativeEditBkColor();
				m_hBkBrush = ::CreateSolidBrush(RGB(GetBValue(clrColor), GetGValue(clrColor), GetRValue(clrColor)));
			}
			return (LRESULT)m_hBkBrush;
		}
		else if (uMsg == WM_PRINT) {
			if (m_pOwner->m_pManager->IsLayered()) {
				lRes = CWindowWnd::HandleMessage(uMsg, wParam, lParam);
				if (m_pOwner->IsEnabled() && m_bDrawCaret) {
					RECT rcClient;
					::GetClientRect(m_hWnd, &rcClient);
					POINT ptCaret;
					::GetCaretPos(&ptCaret);
					RECT rcCaret = { ptCaret.x, ptCaret.y, ptCaret.x, ptCaret.y + rcClient.bottom - rcClient.top };
					CRenderEngine::DrawLine((HDC)wParam, rcCaret, 1, 0xFF000000);
				}
				return lRes;
			}
			bHandled = FALSE;
		}
		else if (uMsg == WM_TIMER) {
			if (wParam == CARET_TIMERID) {
				m_bDrawCaret = !m_bDrawCaret;
				RECT rcClient;
				::GetClientRect(m_hWnd, &rcClient);
				::InvalidateRect(m_hWnd, &rcClient, FALSE);
				return 0;
			}
			bHandled = FALSE;
		}
		else bHandled = FALSE;

		if (!bHandled) return CWindowWnd::HandleMessage(uMsg, wParam, lParam);
		return lRes;
	}

	LRESULT CEditWnd::OnKillFocus(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{

		
		//  must comment it
		//LRESULT lRes = ::DefWindowProc(m_hWnd, uMsg, wParam, lParam);
		//PostMessage(WM_CLOSE);

		//m_pOwner->BoxClose();
		
		DUI__Trace(_T("CEditWnd::OnKillFocus"));
		
		///diliver this msg
		bHandled = FALSE;
		return 0;
	}

	LRESULT CEditWnd::OnEditChanged(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
	{
		if (!m_bInit) return 0;
		if (m_pOwner == NULL) return 0;
		// Copy text back
		int cchLen = ::GetWindowTextLength(m_hWnd) + 1;
		LPTSTR pstr = static_cast<LPTSTR>(_alloca(cchLen * sizeof(TCHAR)));
		ASSERT(pstr);
		if (pstr == NULL) return 0;
		::GetWindowText(m_hWnd, pstr, cchLen);
		m_pOwner->m_sText = pstr;
		m_pOwner->m_pManager->SendNotify(m_pOwner, DUI_MSGTYPE_TEXTCHANGED);
		if (m_pOwner->m_pManager->IsLayered()) 
			m_pOwner->Invalidate();


		m_pOwner->EditTextChg();


		DUI__Trace(_T("CEditWnd::OnEditChanged"));
		return 0;
	}

	




	IMPLEMENT_DUICONTROL(CEditComboUI)


	CEditComboUI::CEditComboUI()
		: m_pWindow(NULL), 
		m_pComboWindow(NULL),
		m_uMaxChar(255), 
		m_bReadOnly(false),
		m_bPasswordMode(false), 
		m_cPasswordChar(_T('*')),
		m_uButtonState(0),
		m_dwEditbkColor(0xFFFFFFFF),
		m_dwEditTextColor(0x00000000),
		m_iWindowStyls(0), 
		m_dwTipValueColor(0xFFBAC0C5),
		m_iCurSel(-1),
		m_bResEditChg(false)
	{
		SetTextPadding(CDuiRect(4, 3, 4, 3));
		SetBkColor(0xFFFFFFFF);

		m_szDropBox = CDuiSize(0, 150);
		::ZeroMemory(&m_rcTextPadding, sizeof(m_rcTextPadding));

		m_ListInfo.nColumns = 0;
		m_ListInfo.nFont = -1;
		m_ListInfo.uTextStyle = DT_VCENTER;
		m_ListInfo.dwTextColor = 0xFF000000;
		m_ListInfo.dwBkColor = 0;
		m_ListInfo.bAlternateBk = false;
		m_ListInfo.dwSelectedTextColor = 0xFF000000;
		m_ListInfo.dwSelectedBkColor = 0xFFC1E3FF;
		m_ListInfo.dwHotTextColor = 0xFF000000;
		m_ListInfo.dwHotBkColor = 0xFFE9F5FF;
		m_ListInfo.dwDisabledTextColor = 0xFFCCCCCC;
		m_ListInfo.dwDisabledBkColor = 0xFFFFFFFF;
		m_ListInfo.dwLineColor = 0;
		m_ListInfo.bShowHtml = false;
		m_ListInfo.bMultiExpandable = false;
		::ZeroMemory(&m_ListInfo.rcTextPadding, sizeof(m_ListInfo.rcTextPadding));
		::ZeroMemory(&m_ListInfo.rcColumn, sizeof(m_ListInfo.rcColumn));

	}
	CEditComboUI::~CEditComboUI()
	{

	}

	LPCTSTR CEditComboUI::GetClass() const
	{
		return _T("EditComboUI");
	}
	LPVOID CEditComboUI::GetInterface(LPCTSTR pstrName)
	{
		if (_tcsicmp(pstrName, _T("EditCombo")) == 0) return static_cast<CEditComboUI*>(this);
		if (_tcsicmp(pstrName, _T("IListOwner")) == 0) return static_cast<IListOwnerUI*>(this);
		return CContainerUI::GetInterface(pstrName);
	}

	UINT CEditComboUI::GetControlFlags() const
	{
		if (!IsEnabled()) return CControlUI::GetControlFlags();

		return UIFLAG_SETCURSOR | UIFLAG_TABSTOP;
	}

	void CEditComboUI::DoEvent(TEventUI& event)
	{
		if (!IsMouseEnabled() && event.Type > UIEVENT__MOUSEBEGIN && event.Type < UIEVENT__MOUSEEND) {
			if (m_pParent != NULL) m_pParent->DoEvent(event);
			else CContainerUI::DoEvent(event);
			return;
		}

		if (event.Type == UIEVENT_SETCURSOR && IsEnabled())
		{
			::SetCursor(::LoadCursor(NULL, MAKEINTRESOURCE(IDC_IBEAM)));
			return;
		}
		if (event.Type == UIEVENT_WINDOWSIZE)
		{
			if (m_pWindow != NULL) m_pManager->SetFocusNeeded(this);
		}
		if (event.Type == UIEVENT_SCROLLWHEEL)
		{
			if (m_pWindow != NULL) return;
		}
		if (event.Type == UIEVENT_SETFOCUS && IsEnabled())
		{
			DUI__Trace(_T("CEditComboUI::UIEVENT_SETFOCUS"));
			//
			if (m_pWindow) return;
			m_pWindow = new CEditWnd();
			ASSERT(m_pWindow);
			m_pWindow->Init(this);
			
			Invalidate();

			
		}
		if (event.Type == UIEVENT_KILLFOCUS && IsEnabled())
		{
			DUI__Trace(_T("CEditComboUI::UIEVENT_KILLFOCUS"));
			Invalidate();
		}
		if (event.Type == UIEVENT_BUTTONDOWN || event.Type == UIEVENT_DBLCLICK || event.Type == UIEVENT_RBUTTONDOWN)
		{
			

			if (IsEnabled()) {
				m_pManager->ReleaseCapture();
				if (IsFocused() && m_pWindow == NULL)
				{
					m_pWindow = new CEditWnd();
					ASSERT(m_pWindow);
					m_pWindow->Init(this);

					if (PtInRect(&m_rcItem, event.ptMouse))
					{
						int nSize = GetWindowTextLength(*m_pWindow);
						if (nSize == 0)
							nSize = 1;

						Edit_SetSel(*m_pWindow, 0, nSize);
					}
				}
				else if (m_pWindow != NULL)
				{
#if 1
					int nSize = GetWindowTextLength(*m_pWindow);
					if (nSize == 0)
						nSize = 1;

					Edit_SetSel(*m_pWindow, 0, nSize);
#else
					POINT pt = event.ptMouse;
					pt.x -= m_rcItem.left + m_rcTextPadding.left;
					pt.y -= m_rcItem.top + m_rcTextPadding.top;
					::SendMessage(*m_pWindow, WM_LBUTTONDOWN, event.wParam, MAKELPARAM(pt.x, pt.y));
#endif
				}

				ActivateCoboBox();
			}

			

			
			return;
		}
		if (event.Type == UIEVENT_MOUSEMOVE)
		{
			return;
		}
		if (event.Type == UIEVENT_BUTTONUP)
		{
			return;
		}
		if (event.Type == UIEVENT_CONTEXTMENU)
		{
			return;
		}
		if (event.Type == UIEVENT_MOUSEENTER)
		{
			if (IsEnabled()) {
				m_uButtonState |= UISTATE_HOT;
				Invalidate();
			}
			return;
		}
		if (event.Type == UIEVENT_MOUSELEAVE)
		{
			if (IsEnabled()) {
				m_uButtonState &= ~UISTATE_HOT;
				Invalidate();
			}
			return;
		}
		CContainerUI::DoEvent(event);
	}

	int CEditComboUI::GetWindowStyls() const
	{
		return m_iWindowStyls;
	}

	void CEditComboUI::SetNumberOnly(bool bNumberOnly)
	{
		if (bNumberOnly)
		{
			m_iWindowStyls |= ES_NUMBER;
		}
		else
		{
			m_iWindowStyls &= ~ES_NUMBER;
		}
	}

	bool CEditComboUI::IsPasswordMode() const
	{
		return m_bPasswordMode;
	}

	UINT CEditComboUI::GetMaxChar()
	{
		return m_uMaxChar;
	}

	TCHAR CEditComboUI::GetPasswordChar() const
	{
		return m_cPasswordChar;
	}

	bool CEditComboUI::IsReadOnly() const
	{
		return m_bReadOnly;
	}

	DWORD CEditComboUI::GetNativeEditBkColor() const
	{
		return m_dwEditbkColor;
	}

	DWORD CEditComboUI::GetNativeEditTextColor() const
	{
		return m_dwEditTextColor;
	}

	UINT CEditComboUI::GetTextStyle() const
	{
		return m_uTextStyle;
	}
	int CEditComboUI::GetFont() const
	{
		return m_iFont;
	}

	RECT CEditComboUI::GetTextPadding() const
	{
		return m_rcTextPadding;
	}
	void CEditComboUI::SetTextColor(DWORD dwTextColor)
	{
		m_dwTextColor = dwTextColor;
		Invalidate();
	}

	DWORD CEditComboUI::GetTextColor() const
	{
		return m_dwTextColor;
	}

	void CEditComboUI::SetTextPadding(RECT rc)
	{
		m_rcTextPadding = rc;
		Invalidate();
	}


	UINT CEditComboUI::GetListType()
	{
		return LT_COMBO;
	}

	TListInfoUI* CEditComboUI::GetListInfo()
	{
		return &m_ListInfo;
	}

	int CEditComboUI::GetCurSel() const
	{
		return m_iCurSel;
	}

	bool CEditComboUI::SelectItem(int iIndex, bool bTakeFocus)
	{
		if (iIndex == m_iCurSel) return true;
		int iOldSel = m_iCurSel;
		if (m_iCurSel >= 0) {
			CControlUI* pControl = static_cast<CControlUI*>(m_items[m_iCurSel]);
			if (!pControl) return false;
			IListItemUI* pListItem = static_cast<IListItemUI*>(pControl->GetInterface(_T("ListItem")));
			if (pListItem != NULL) pListItem->Select(false);
			m_iCurSel = -1;
		}
		if (iIndex < 0) return false;
		if (m_items.GetSize() == 0) return false;
		if (iIndex >= m_items.GetSize()) iIndex = m_items.GetSize() - 1;
		CControlUI* pControl = static_cast<CControlUI*>(m_items[iIndex]);
		if (!pControl || !pControl->IsEnabled()) return false;
		IListItemUI* pListItem = static_cast<IListItemUI*>(pControl->GetInterface(_T("ListItem")));
		if (pListItem == NULL) return false;
		m_iCurSel = iIndex;
		if (m_pWindow != NULL || bTakeFocus) pControl->SetFocus();
		pListItem->Select(true);
		if (m_pManager != NULL) m_pManager->SendNotify(this, DUI_MSGTYPE_ITEMSELECT, m_iCurSel, iOldSel);
		Invalidate();

		return true;
	}

	bool CEditComboUI::SelectMultiItem(int iIndex, bool bTakeFocus)
	{
		return SelectItem(iIndex, bTakeFocus);
	}

	bool CEditComboUI::UnSelectItem(int iIndex, bool bOthers)
	{
		return false;
	}

	bool CEditComboUI::SetItemIndex(CControlUI* pControl, int iIndex)
	{
		int iOrginIndex = GetItemIndex(pControl);
		if (iOrginIndex == -1) return false;
		if (iOrginIndex == iIndex) return true;

		IListItemUI* pSelectedListItem = NULL;
		if (m_iCurSel >= 0) pSelectedListItem =
			static_cast<IListItemUI*>(GetItemAt(m_iCurSel)->GetInterface(_T("ListItem")));
		if (!CContainerUI::SetItemIndex(pControl, iIndex)) return false;
		int iMinIndex = min(iOrginIndex, iIndex);
		int iMaxIndex = max(iOrginIndex, iIndex);
		for (int i = iMinIndex; i < iMaxIndex + 1; ++i) {
			CControlUI* p = GetItemAt(i);
			IListItemUI* pListItem = static_cast<IListItemUI*>(p->GetInterface(_T("ListItem")));
			if (pListItem != NULL) {
				pListItem->SetIndex(i);
			}
		}
		if (m_iCurSel >= 0 && pSelectedListItem != NULL) m_iCurSel = pSelectedListItem->GetIndex();
		return true;
	}

	SIZE CEditComboUI::GetDropBoxSize() const
	{
		return m_szDropBox;
	}

	CDuiString CEditComboUI::GetDropBoxAttributeList()
	{
		return m_sDropBoxAttributes;
	}

	bool CEditComboUI::GetScrollSelect()
	{
		return m_bScrollSelect;
	}


	bool CEditComboUI::Add(CControlUI* pControl)
	{
		IListItemUI* pListItem = static_cast<IListItemUI*>(pControl->GetInterface(_T("ListItem")));
		if (pListItem != NULL)
		{
			pListItem->SetOwner(this);
			pListItem->SetIndex(m_items.GetSize());
		}
	

		return CContainerUI::Add(pControl);
	}

	bool CEditComboUI::AddAt(CControlUI* pControl, int iIndex)
	{
		if (!CContainerUI::AddAt(pControl, iIndex)) return false;

		// The list items should know about us
		IListItemUI* pListItem = static_cast<IListItemUI*>(pControl->GetInterface(_T("ListItem")));
		if (pListItem != NULL) {
			pListItem->SetOwner(this);
			pListItem->SetIndex(iIndex);
		}

		for (int i = iIndex + 1; i < GetCount(); ++i) {
			CControlUI* p = GetItemAt(i);
			pListItem = static_cast<IListItemUI*>(p->GetInterface(_T("ListItem")));
			if (pListItem != NULL) {
				pListItem->SetIndex(i);
			}
		}
		if (m_iCurSel >= iIndex) m_iCurSel += 1;



		return true;
	}

	bool CEditComboUI::Remove(CControlUI* pControl)
	{
		int iIndex = GetItemIndex(pControl);
		if (iIndex == -1) return false;

		if (!CContainerUI::RemoveAt(iIndex)) return false;

		for (int i = iIndex; i < GetCount(); ++i) {
			CControlUI* p = GetItemAt(i);
			IListItemUI* pListItem = static_cast<IListItemUI*>(p->GetInterface(_T("ListItem")));
			if (pListItem != NULL) {
				pListItem->SetIndex(i);
			}
		}

		if (iIndex == m_iCurSel && m_iCurSel >= 0) {
			int iSel = m_iCurSel;
			m_iCurSel = -1;
			SelectItem(FindSelectable(iSel, false));
		}
		else if (iIndex < m_iCurSel) m_iCurSel -= 1;
		return true;
	}

	bool CEditComboUI::RemoveAt(int iIndex)
	{
		if (!CContainerUI::RemoveAt(iIndex)) return false;

		for (int i = iIndex; i < GetCount(); ++i) {
			CControlUI* p = GetItemAt(i);
			IListItemUI* pListItem = static_cast<IListItemUI*>(p->GetInterface(_T("ListItem")));
			if (pListItem != NULL) pListItem->SetIndex(i);
		}

		if (iIndex == m_iCurSel && m_iCurSel >= 0) {
			int iSel = m_iCurSel;
			m_iCurSel = -1;
			SelectItem(FindSelectable(iSel, false));
		}
		else if (iIndex < m_iCurSel) m_iCurSel -= 1;
		return true;
	}

	void CEditComboUI::RemoveAll()
	{
		m_iCurSel = -1;
		CContainerUI::RemoveAll();
	}

	bool CEditComboUI::ActivateCoboBox()
	{
		if (!CControlUI::Activate()) return false;
		if (m_pComboWindow)
		{
			m_pComboWindow->Refresh();
			//m_pComboWindow->ShowWindow(true , false);
			m_pComboWindow->ShowWindow(true);
			

			return true;
		}

		m_vecBoxItem.clear();
		for (int i = 0; i < GetCount(); ++i)
			m_vecBoxItem.push_back(static_cast<CControlUI*>(GetItemAt(i)));

		m_pComboWindow = new CComboWnd();
		ASSERT(m_pComboWindow);
		m_pComboWindow->Init(this);
		if (m_pManager != NULL) m_pManager->SendNotify(this, DUI_MSGTYPE_DROPDOWN);
		//Invalidate();
		return true;
	}

	void CEditComboUI::DoPaint(HDC hDC, const RECT& rcPaint)
	{
		DUI__Trace(_T("CEditComboUI::DoPaint(%d,%d,%d,%d)") , rcPaint.left, rcPaint.top, rcPaint.right, rcPaint.bottom);
		CControlUI::DoPaint(hDC, rcPaint);

	}

	void CEditComboUI::SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue)
	{

		if (_tcsicmp(pstrName, _T("readonly")) == 0) SetReadOnly(_tcsicmp(pstrValue, _T("true")) == 0);
		else if (_tcsicmp(pstrName, _T("numberonly")) == 0) SetNumberOnly(_tcsicmp(pstrValue, _T("true")) == 0);
		else if (_tcsicmp(pstrName, _T("password")) == 0) SetPasswordMode(_tcsicmp(pstrValue, _T("true")) == 0);
		else if (_tcsicmp(pstrName, _T("passwordchar")) == 0) SetPasswordChar(*pstrValue);
		else if (_tcsicmp(pstrName, _T("maxchar")) == 0) SetMaxChar(_ttoi(pstrValue));
		else if (_tcsicmp(pstrName, _T("normalimage")) == 0) SetNormalImage(pstrValue);
		else if (_tcsicmp(pstrName, _T("hotimage")) == 0) SetHotImage(pstrValue);
		else if (_tcsicmp(pstrName, _T("focusedimage")) == 0) SetFocusedImage(pstrValue);
		else if (_tcsicmp(pstrName, _T("disabledimage")) == 0) SetDisabledImage(pstrValue);
		else if (_tcsicmp(pstrName, _T("tipvalue")) == 0) SetTipValue(pstrValue);
		else if (_tcsicmp(pstrName, _T("tipvaluecolor")) == 0) SetTipValueColor(pstrValue);
		else if (_tcsicmp(pstrName, _T("nativetextcolor")) == 0) SetNativeEditTextColor(pstrValue);
		else if (_tcsicmp(pstrName, _T("nativebkcolor")) == 0) {
			if (*pstrValue == _T('#')) pstrValue = ::CharNext(pstrValue);
			LPTSTR pstr = NULL;
			DWORD clrColor = _tcstoul(pstrValue, &pstr, 16);
			SetNativeEditBkColor(clrColor);
		}
		else if (_tcsicmp(pstrName, _T("align")) == 0) {
			if (_tcsstr(pstrValue, _T("left")) != NULL) {
				m_uTextStyle &= ~(DT_CENTER | DT_RIGHT | DT_SINGLELINE);
				m_uTextStyle |= DT_LEFT;
			}
			if (_tcsstr(pstrValue, _T("center")) != NULL) {
				m_uTextStyle &= ~(DT_LEFT | DT_RIGHT);
				m_uTextStyle |= DT_CENTER;
			}
			if (_tcsstr(pstrValue, _T("right")) != NULL) {
				m_uTextStyle &= ~(DT_LEFT | DT_CENTER | DT_SINGLELINE);
				m_uTextStyle |= DT_RIGHT;
			}
		}
		else if (_tcsicmp(pstrName, _T("valign")) == 0) {
			if (_tcsstr(pstrValue, _T("top")) != NULL) {
				m_uTextStyle &= ~(DT_BOTTOM | DT_VCENTER);
				m_uTextStyle |= (DT_TOP | DT_SINGLELINE);
			}
			if (_tcsstr(pstrValue, _T("vcenter")) != NULL) {
				m_uTextStyle &= ~(DT_TOP | DT_BOTTOM);
				m_uTextStyle |= (DT_VCENTER | DT_SINGLELINE);
			}
			if (_tcsstr(pstrValue, _T("bottom")) != NULL) {
				m_uTextStyle &= ~(DT_TOP | DT_VCENTER);
				m_uTextStyle |= (DT_BOTTOM | DT_SINGLELINE);
			}
		}
		else if (_tcsicmp(pstrName, _T("endellipsis")) == 0) {
			if (_tcsicmp(pstrValue, _T("true")) == 0) m_uTextStyle |= DT_END_ELLIPSIS;
			else m_uTextStyle &= ~DT_END_ELLIPSIS;
		}
		else if (_tcsicmp(pstrName, _T("wordbreak")) == 0) {
			if (_tcsicmp(pstrValue, _T("true")) == 0) {
				m_uTextStyle &= ~DT_SINGLELINE;
				m_uTextStyle |= DT_WORDBREAK | DT_EDITCONTROL;
			}
			else {
				m_uTextStyle &= ~DT_WORDBREAK & ~DT_EDITCONTROL;
				m_uTextStyle |= DT_SINGLELINE;
			}
		}
		else if (_tcsicmp(pstrName, _T("font")) == 0) SetFont(_ttoi(pstrValue));
		else if (_tcsicmp(pstrName, _T("textcolor")) == 0) {
			if (*pstrValue == _T('#')) pstrValue = ::CharNext(pstrValue);
			LPTSTR pstr = NULL;
			DWORD clrColor = _tcstoul(pstrValue, &pstr, 16);
			SetTextColor(clrColor);
		}
		else if (_tcsicmp(pstrName, _T("disabledtextcolor")) == 0) {
			if (*pstrValue == _T('#')) pstrValue = ::CharNext(pstrValue);
			LPTSTR pstr = NULL;
			DWORD clrColor = _tcstoul(pstrValue, &pstr, 16);
			SetDisabledTextColor(clrColor);
		}
		else if (_tcsicmp(pstrName, _T("textpadding")) == 0) {
			RECT rcTextPadding = { 0 };
			LPTSTR pstr = NULL;
			rcTextPadding.left = _tcstol(pstrValue, &pstr, 10);  ASSERT(pstr);
			rcTextPadding.top = _tcstol(pstr + 1, &pstr, 10);    ASSERT(pstr);
			rcTextPadding.right = _tcstol(pstr + 1, &pstr, 10);  ASSERT(pstr);
			rcTextPadding.bottom = _tcstol(pstr + 1, &pstr, 10); ASSERT(pstr);
			SetTextPadding(rcTextPadding);
		}
		else if (_tcsicmp(pstrName, _T("showhtml")) == 0) SetShowHtml(_tcsicmp(pstrValue, _T("true")) == 0);
		else if (_tcsicmp(pstrName, _T("normalimage")) == 0) SetNormalImage(pstrValue);
		else if (_tcsicmp(pstrName, _T("hotimage")) == 0) SetHotImage(pstrValue);
		else if (_tcsicmp(pstrName, _T("pushedimage")) == 0) SetPushedImage(pstrValue);
		else if (_tcsicmp(pstrName, _T("focusedimage")) == 0) SetFocusedImage(pstrValue);
		else if (_tcsicmp(pstrName, _T("disabledimage")) == 0) SetDisabledImage(pstrValue);
		else if (_tcsicmp(pstrName, _T("scrollselect")) == 0) SetScrollSelect(_tcsicmp(pstrValue, _T("true")) == 0);
		else if (_tcsicmp(pstrName, _T("dropbox")) == 0) SetDropBoxAttributeList(pstrValue);
		else if (_tcsicmp(pstrName, _T("dropboxsize")) == 0)
		{
			SIZE szDropBoxSize = { 0 };
			LPTSTR pstr = NULL;
			szDropBoxSize.cx = _tcstol(pstrValue, &pstr, 10);  ASSERT(pstr);
			szDropBoxSize.cy = _tcstol(pstr + 1, &pstr, 10);    ASSERT(pstr);
			SetDropBoxSize(szDropBoxSize);
		}
		else if (_tcsicmp(pstrName, _T("itemfont")) == 0) m_ListInfo.nFont = _ttoi(pstrValue);
		else if (_tcsicmp(pstrName, _T("itemalign")) == 0) {
			if (_tcsstr(pstrValue, _T("left")) != NULL) {
				m_ListInfo.uTextStyle &= ~(DT_CENTER | DT_RIGHT);
				m_ListInfo.uTextStyle |= DT_LEFT;
			}
			if (_tcsstr(pstrValue, _T("center")) != NULL) {
				m_ListInfo.uTextStyle &= ~(DT_LEFT | DT_RIGHT);
				m_ListInfo.uTextStyle |= DT_CENTER;
			}
			if (_tcsstr(pstrValue, _T("right")) != NULL) {
				m_ListInfo.uTextStyle &= ~(DT_LEFT | DT_CENTER);
				m_ListInfo.uTextStyle |= DT_RIGHT;
			}
		}
		else if (_tcsicmp(pstrName, _T("itemendellipsis")) == 0) {
			if (_tcsicmp(pstrValue, _T("true")) == 0) m_ListInfo.uTextStyle |= DT_END_ELLIPSIS;
			else m_ListInfo.uTextStyle &= ~DT_END_ELLIPSIS;
		}
		else if (_tcsicmp(pstrName, _T("itemtextpadding")) == 0) {
			RECT rcTextPadding = { 0 };
			LPTSTR pstr = NULL;
			rcTextPadding.left = _tcstol(pstrValue, &pstr, 10);  ASSERT(pstr);
			rcTextPadding.top = _tcstol(pstr + 1, &pstr, 10);    ASSERT(pstr);
			rcTextPadding.right = _tcstol(pstr + 1, &pstr, 10);  ASSERT(pstr);
			rcTextPadding.bottom = _tcstol(pstr + 1, &pstr, 10); ASSERT(pstr);
			SetItemTextPadding(rcTextPadding);
		}
		else if (_tcsicmp(pstrName, _T("itemtextcolor")) == 0) {
			if (*pstrValue == _T('#')) pstrValue = ::CharNext(pstrValue);
			LPTSTR pstr = NULL;
			DWORD clrColor = _tcstoul(pstrValue, &pstr, 16);
			SetItemTextColor(clrColor);
		}
		else if (_tcsicmp(pstrName, _T("itembkcolor")) == 0) {
			if (*pstrValue == _T('#')) pstrValue = ::CharNext(pstrValue);
			LPTSTR pstr = NULL;
			DWORD clrColor = _tcstoul(pstrValue, &pstr, 16);
			SetItemBkColor(clrColor);
		}
		else if (_tcsicmp(pstrName, _T("itembkimage")) == 0) SetItemBkImage(pstrValue);
		else if (_tcsicmp(pstrName, _T("itemaltbk")) == 0) SetAlternateBk(_tcsicmp(pstrValue, _T("true")) == 0);
		else if (_tcsicmp(pstrName, _T("itemselectedtextcolor")) == 0) {
			if (*pstrValue == _T('#')) pstrValue = ::CharNext(pstrValue);
			LPTSTR pstr = NULL;
			DWORD clrColor = _tcstoul(pstrValue, &pstr, 16);
			SetSelectedItemTextColor(clrColor);
		}
		else if (_tcsicmp(pstrName, _T("itemselectedbkcolor")) == 0) {
			if (*pstrValue == _T('#')) pstrValue = ::CharNext(pstrValue);
			LPTSTR pstr = NULL;
			DWORD clrColor = _tcstoul(pstrValue, &pstr, 16);
			SetSelectedItemBkColor(clrColor);
		}
		else if (_tcsicmp(pstrName, _T("itemselectedimage")) == 0) SetSelectedItemImage(pstrValue);
		else if (_tcsicmp(pstrName, _T("itemhottextcolor")) == 0) {
			if (*pstrValue == _T('#')) pstrValue = ::CharNext(pstrValue);
			LPTSTR pstr = NULL;
			DWORD clrColor = _tcstoul(pstrValue, &pstr, 16);
			SetHotItemTextColor(clrColor);
		}
		else if (_tcsicmp(pstrName, _T("itemhotbkcolor")) == 0) {
			if (*pstrValue == _T('#')) pstrValue = ::CharNext(pstrValue);
			LPTSTR pstr = NULL;
			DWORD clrColor = _tcstoul(pstrValue, &pstr, 16);
			SetHotItemBkColor(clrColor);
		}
		else if (_tcsicmp(pstrName, _T("itemhotimage")) == 0) SetHotItemImage(pstrValue);
		else if (_tcsicmp(pstrName, _T("itemdisabledtextcolor")) == 0) {
			if (*pstrValue == _T('#')) pstrValue = ::CharNext(pstrValue);
			LPTSTR pstr = NULL;
			DWORD clrColor = _tcstoul(pstrValue, &pstr, 16);
			SetDisabledItemTextColor(clrColor);
		}
		else if (_tcsicmp(pstrName, _T("itemdisabledbkcolor")) == 0) {
			if (*pstrValue == _T('#')) pstrValue = ::CharNext(pstrValue);
			LPTSTR pstr = NULL;
			DWORD clrColor = _tcstoul(pstrValue, &pstr, 16);
			SetDisabledItemBkColor(clrColor);
		}
		else if (_tcsicmp(pstrName, _T("itemdisabledimage")) == 0) SetDisabledItemImage(pstrValue);
		else if (_tcsicmp(pstrName, _T("itemlinecolor")) == 0) {
			if (*pstrValue == _T('#')) pstrValue = ::CharNext(pstrValue);
			LPTSTR pstr = NULL;
			DWORD clrColor = _tcstoul(pstrValue, &pstr, 16);
			SetItemLineColor(clrColor);
		}
		else if (_tcsicmp(pstrName, _T("itemshowhtml")) == 0) SetItemShowHtml(_tcsicmp(pstrValue, _T("true")) == 0);
		else CContainerUI::SetAttribute(pstrName, pstrValue);
	}

	void CEditComboUI::SetFont(int index)
	{
		m_iFont = index;
		Invalidate();
	}

	void CEditComboUI::SetDisabledTextColor(DWORD dwTextColor)
	{
		m_dwDisabledTextColor = dwTextColor;
		Invalidate();
	}

	void CEditComboUI::SetShowHtml(bool bShowHtml)
	{
		if (m_bShowHtml == bShowHtml) return;

		m_bShowHtml = bShowHtml;
		Invalidate();
	}

	void CEditComboUI::SetNormalImage(LPCTSTR pStrImage)
	{
		m_sNormalImage = pStrImage;
		Invalidate();
	}

	void CEditComboUI::SetHotImage(LPCTSTR pStrImage)
	{
		m_sHotImage = pStrImage;
		Invalidate();
	}

	void CEditComboUI::SetPushedImage(LPCTSTR pStrImage)
	{
		m_sPushedImage = pStrImage;
		Invalidate();
	}

	void CEditComboUI::SetFocusedImage(LPCTSTR pStrImage)
	{
		m_sFocusedImage = pStrImage;
		Invalidate();
	}

	void CEditComboUI::SetDisabledImage(LPCTSTR pStrImage)
	{
		m_sDisabledImage = pStrImage;
		Invalidate();
	}

	void CEditComboUI::SetScrollSelect(bool bScrollSelect)
	{
		m_bScrollSelect = bScrollSelect;
	}

	void CEditComboUI::SetDropBoxAttributeList(LPCTSTR pstrList)
	{
		m_sDropBoxAttributes = pstrList;
	}

	void CEditComboUI::SetDropBoxSize(SIZE szDropBox)
	{
		m_szDropBox = szDropBox;
	}

	void CEditComboUI::SetItemTextPadding(RECT rc)
	{
		m_ListInfo.rcTextPadding = rc;
		Invalidate();
	}

	void CEditComboUI::SetItemTextColor(DWORD dwTextColor)
	{
		m_ListInfo.dwTextColor = dwTextColor;
		Invalidate();
	}

	void CEditComboUI::SetItemBkColor(DWORD dwBkColor)
	{
		m_ListInfo.dwBkColor = dwBkColor;
	}

	void CEditComboUI::SetItemBkImage(LPCTSTR pStrImage)
	{
		m_ListInfo.sBkImage = pStrImage;
	}

	void CEditComboUI::SetAlternateBk(bool bAlternateBk)
	{
		m_ListInfo.bAlternateBk = bAlternateBk;
	}

	void CEditComboUI::SetSelectedItemTextColor(DWORD dwTextColor)
	{
		m_ListInfo.dwSelectedTextColor = dwTextColor;
	}

	void CEditComboUI::SetSelectedItemBkColor(DWORD dwBkColor)
	{
		m_ListInfo.dwSelectedBkColor = dwBkColor;
	}

	void CEditComboUI::SetSelectedItemImage(LPCTSTR pStrImage)
	{
		m_ListInfo.sSelectedImage = pStrImage;
	}

	void CEditComboUI::SetHotItemTextColor(DWORD dwTextColor)
	{
		m_ListInfo.dwHotTextColor = dwTextColor;
	}

	void CEditComboUI::SetHotItemBkColor(DWORD dwBkColor)
	{
		m_ListInfo.dwHotBkColor = dwBkColor;
	}

	void CEditComboUI::SetHotItemImage(LPCTSTR pStrImage)
	{
		m_ListInfo.sHotImage = pStrImage;
	}

	void CEditComboUI::SetDisabledItemTextColor(DWORD dwTextColor)
	{
		m_ListInfo.dwDisabledTextColor = dwTextColor;
	}

	void CEditComboUI::SetDisabledItemBkColor(DWORD dwBkColor)
	{
		m_ListInfo.dwDisabledBkColor = dwBkColor;
	}

	void CEditComboUI::SetDisabledItemImage(LPCTSTR pStrImage)
	{
		m_ListInfo.sDisabledImage = pStrImage;
	}

	void CEditComboUI::SetItemLineColor(DWORD dwLineColor)
	{
		m_ListInfo.dwLineColor = dwLineColor;
	}

	void CEditComboUI::SetItemShowHtml(bool bShowHtml)
	{
		if (m_ListInfo.bShowHtml == bShowHtml) return;

		m_ListInfo.bShowHtml = bShowHtml;
		Invalidate();
	}

	void CEditComboUI::SetPos(RECT rc, bool bNeedInvalidate)
	{
		////// 隐藏下拉窗口, 父窗口重绘，就会关闭，有问题 ,待解决
	/*	if (m_pComboWindow && ::IsWindow(m_pComboWindow->GetHWND()))
			m_pComboWindow->Close();
*/

		// 所有元素大小置为0
		RECT rcNull = { 0 };
		for (int i = 0; i < m_items.GetSize(); i++)
			static_cast<CControlUI*>(m_items[i])->SetPos(rcNull);
		// 调整位置
		CControlUI::SetPos(rc, bNeedInvalidate);
	}


	void CEditComboUI::PaintStatusImage(HDC hDC)
	{
		if (IsFocused()) m_uButtonState |= UISTATE_FOCUSED;
		else m_uButtonState &= ~UISTATE_FOCUSED;
		if (!IsEnabled()) m_uButtonState |= UISTATE_DISABLED;
		else m_uButtonState &= ~UISTATE_DISABLED;

		if ((m_uButtonState & UISTATE_DISABLED) != 0) {
			if (!m_sDisabledImage.IsEmpty()) {
				if (!DrawImage(hDC, (LPCTSTR)m_sDisabledImage)) {}
				else return;
			}
		}
		else if ((m_uButtonState & UISTATE_PUSHED) != 0) {
			if (!m_sPushedImage.IsEmpty()) {
				if (!DrawImage(hDC, (LPCTSTR)m_sPushedImage)) {}
				else return;
			}
		}
		else if ((m_uButtonState & UISTATE_HOT) != 0) {
			if (!m_sHotImage.IsEmpty()) {
				if (!DrawImage(hDC, (LPCTSTR)m_sHotImage)) {}
				else return;
			}
		}
		else if ((m_uButtonState & UISTATE_FOCUSED) != 0) {
			if (!m_sFocusedImage.IsEmpty()) {
				if (!DrawImage(hDC, (LPCTSTR)m_sFocusedImage)) {}
				else return;
			}
		}

		if (!m_sNormalImage.IsEmpty()) {
			if (!DrawImage(hDC, (LPCTSTR)m_sNormalImage)) {}
			else return;
		}
	}

	SIZE CEditComboUI::EstimateSize(SIZE szAvailable)
	{
		if (m_cxyFixed.cy == 0) return CDuiSize(m_cxyFixed.cx, m_pManager->GetDefaultFontInfo()->tm.tmHeight + 12);
		return CControlUI::EstimateSize(szAvailable);
	}

	CDuiString CEditComboUI::GetText() const
	{
		if (m_iCurSel < 0)
		{
			return m_sText;
		}
		CControlUI* pControl = static_cast<CControlUI*>(m_items[m_iCurSel]);
	
		return pControl->GetText();
	}

	LPCTSTR  CEditComboUI::GetUserData() const
	{
		if (m_iCurSel < 0)
		{
			return _T("");
		}
		CControlUI* pControl = static_cast<CControlUI*>(m_items[m_iCurSel]);

		return pControl->GetUserData();
	}



	void CEditComboUI::PaintText(HDC hDC)
	{
		DWORD mCurTextColor = m_dwTextColor;

		if (m_dwTextColor == 0) mCurTextColor = m_dwTextColor = m_pManager->GetDefaultFontColor();
		if (m_dwDisabledTextColor == 0) m_dwDisabledTextColor = m_pManager->GetDefaultDisabledColor();

		CDuiString sDrawText = GetText();
		CDuiString sTipValue = GetTipValue();
		if (sDrawText == sTipValue || sDrawText == _T("")) {
			mCurTextColor = m_dwTipValueColor;
			sDrawText = sTipValue;
		}
		else {
			CDuiString sTemp = sDrawText;
			if (m_bPasswordMode) {
				sDrawText.Empty();
				LPCTSTR pStr = sTemp.GetData();
				while (*pStr != _T('\0')) {
					sDrawText += m_cPasswordChar;
					pStr = ::CharNext(pStr);
				}
			}
		}

		RECT rc = m_rcItem;
		rc.left += m_rcTextPadding.left;
		rc.right -= m_rcTextPadding.right;
		rc.top += m_rcTextPadding.top;
		rc.bottom -= m_rcTextPadding.bottom;
		if (IsEnabled()) {
			CRenderEngine::DrawText(hDC, m_pManager, rc, sDrawText, mCurTextColor, \
				m_iFont, DT_SINGLELINE | m_uTextStyle);
		}
		else {
			CRenderEngine::DrawText(hDC, m_pManager, rc, sDrawText, m_dwDisabledTextColor, \
				m_iFont, DT_SINGLELINE | m_uTextStyle);
		}
	}

	LPCTSTR CEditComboUI::GetTipValue()
	{
		if (!IsResourceText())
			return m_sTipValue;
		return CResourceManager::GetInstance()->GetText(m_sTipValue);
	}

	void CEditComboUI::SetReadOnly(bool bReadOnly)
	{
		if (m_bReadOnly == bReadOnly) return;

		m_bReadOnly = bReadOnly;
		if (m_pWindow != NULL) Edit_SetReadOnly(*m_pWindow, m_bReadOnly);
		Invalidate();
	}

	void CEditComboUI::SetPasswordMode(bool bPasswordMode)
	{
		if (m_bPasswordMode == bPasswordMode) return;
		m_bPasswordMode = bPasswordMode;
		Invalidate();
		if (m_pWindow != NULL)
		{
			LONG styleValue = ::GetWindowLong(*m_pWindow, GWL_STYLE);
			bPasswordMode ? styleValue |= ES_PASSWORD : styleValue &= ~ES_PASSWORD;
			::SetWindowLong(*m_pWindow, GWL_STYLE, styleValue);
		}
	}

	void CEditComboUI::SetPasswordChar(TCHAR cPasswordChar)
	{
		if (m_cPasswordChar == cPasswordChar) return;
		m_cPasswordChar = cPasswordChar;
		if (m_pWindow != NULL) Edit_SetPasswordChar(*m_pWindow, m_cPasswordChar);
		Invalidate();
	}

	void CEditComboUI::SetMaxChar(UINT uMax)
	{
		m_uMaxChar = uMax;
		if (m_pWindow != NULL) Edit_LimitText(*m_pWindow, m_uMaxChar);
	}

	void CEditComboUI::SetTipValue(LPCTSTR pStrTipValue)
	{
		m_sTipValue = pStrTipValue;
	}

	void CEditComboUI::SetTipValueColor(LPCTSTR pStrColor)
	{
		if (*pStrColor == _T('#')) pStrColor = ::CharNext(pStrColor);
		LPTSTR pstr = NULL;
		DWORD clrColor = _tcstoul(pStrColor, &pstr, 16);

		m_dwTipValueColor = clrColor;
	}

	void CEditComboUI::SetNativeEditBkColor(DWORD dwBkColor)
	{
		m_dwEditbkColor = dwBkColor;
	}

	void CEditComboUI::SetNativeEditTextColor(LPCTSTR pStrColor)
	{
		if (*pStrColor == _T('#')) pStrColor = ::CharNext(pStrColor);
		LPTSTR pstr = NULL;
		DWORD clrColor = _tcstoul(pStrColor, &pstr, 16);

		m_dwEditTextColor = clrColor;
	}

	void CEditComboUI::EditTextChg()
	{
		if (m_bResEditChg)
		{
			m_vecBoxItem.clear();
			for (int i = 0; i < GetCount(); i++) {

				int nCmpLen = min(GetItemAt(i)->GetText().GetLength(), m_sText.GetLength());
				if (!m_sText.Left(nCmpLen).CompareNoCase(GetItemAt(i)->GetText().Left(nCmpLen)))
					m_vecBoxItem.push_back(static_cast<CControlUI*>(GetItemAt(i)));
			}
	
			ActivateCoboBox();
			
		}

		
		
	}

	void CEditComboUI::BoxClose()
	{
		if (m_pComboWindow && IsWindow(m_pComboWindow->GetHWND()))
			m_pComboWindow->Close();
	}


	void CEditComboUI::Refresh()
	{
		m_vecBoxItem.clear();
		for (int i = 0; i < GetCount(); i++) 
		{
			m_vecBoxItem.push_back(static_cast<CControlUI*>(GetItemAt(i)));
			
		}
		
	}

	void CEditComboUI::Invalidate()
	{
		CContainerUI::Invalidate();

	

	}

	void CEditComboUI::EditClose()
	{
		if(m_pWindow && IsWindow(m_pWindow->GetHWND()))
			m_pWindow->Close();
	}

}

