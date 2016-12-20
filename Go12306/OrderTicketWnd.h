#pragma once

class COrderTicketWnd : public WindowImplBase
{
public:

	static int MessageBox(HWND hParent)
	{
		COrderTicketWnd* pWnd = new COrderTicketWnd();
		pWnd->Create(hParent, _T("OrderTicketWnd"), WS_POPUP | WS_CLIPCHILDREN, WS_EX_TOOLWINDOW);
		pWnd->CenterWindow();

		return pWnd->ShowModal();
	}

	static void ShowMessageBox(HWND hParent)
	{
		COrderTicketWnd* pWnd = new COrderTicketWnd();
		pWnd->Create(hParent, _T("OrderTicketWnd"), UI_WNDSTYLE_FRAME, 0);
		pWnd->CenterWindow();

		pWnd->ShowWindow(true);
	}

public:


	COrderTicketWnd();
	~COrderTicketWnd();


	virtual void OnFinalMessage(HWND);
	virtual CDuiString GetSkinFile();
	virtual LPCTSTR GetWindowClassName(void) const;
	virtual void Notify(TNotifyUI &msg);
	virtual void InitWindow();


	DUI_DECLARE_MESSAGE_MAP()
		
	virtual void OnClick(TNotifyUI& msg);


	virtual LRESULT OnSysCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

private:

	CButtonUI* m_pCloseBtn;

};