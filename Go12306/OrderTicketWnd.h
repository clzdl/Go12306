#pragma once

class CTicketModel;
class CPassenger;

class COrderTicketWnd : public WindowImplBase
{
public:

	static int MessageBox(HWND hParent , CTicketModel *ticket , CDuiString orderDate)
	{
		COrderTicketWnd* pWnd = new COrderTicketWnd(ticket , orderDate);
		pWnd->Create(hParent, _T("OrderTicketWnd"), WS_POPUP | WS_CLIPCHILDREN, WS_EX_TOOLWINDOW);
		pWnd->CenterWindow();

		return pWnd->ShowModal();
	}

	static void ShowMessageBox(HWND hParent, CTicketModel *ticket, CDuiString orderDate)
	{
		COrderTicketWnd* pWnd = new COrderTicketWnd(ticket , orderDate);
		pWnd->Create(hParent, _T("OrderTicketWnd"), UI_WNDSTYLE_FRAME, 0);
		pWnd->CenterWindow();

		pWnd->ShowWindow(true);
	}

public:

	COrderTicketWnd(CTicketModel *ticket, CDuiString orderDate);

	~COrderTicketWnd();


	virtual void OnFinalMessage(HWND);
	virtual CDuiString GetSkinFile();
	virtual LPCTSTR GetWindowClassName(void) const;
	virtual void Notify(TNotifyUI &msg);
	virtual void InitWindow();



	DUI_DECLARE_MESSAGE_MAP()
		
	virtual void OnClick(TNotifyUI& msg);

	void OnPassengerListItemClick(TNotifyUI& msg);
	void OnDeleteOrderTicket(TNotifyUI& msg);
	void OnTicketOrderSubmit(TNotifyUI& msg);

	LRESULT HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

private:

	void RefreshPassenger();

	void AddOrderTicketList(CPassenger *passenger);


	void RefreshTicketSeatInfo();

	CComboUI* CreateSeatTypeCombo(CTicketModel *ticket);

	CComboUI* CreateTicketTypeCombo();

	CButtonUI* m_pCloseBtn;
	CButtonUI* m_pMaxBtn;
	CButtonUI* m_pRestoreBtn;
	CListUI *m_pPassengerListView;


	CListUI *m_pOrderListView;

	CDuiString m_sOrderDate;

	CTicketModel *m_pTicket;

	std::string m_strToken;

	std::string m_strLeftTicketString;
	std::string m_strKeyCheckIsChange;
};