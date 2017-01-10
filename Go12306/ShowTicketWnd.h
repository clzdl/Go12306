#pragma once


///
class CMainFrame;
class CTicketModel;

class CShowTicketWnd : public WindowImplBase
{
public:

	static void ShowMessageBox(CMainFrame *frame, RECT rect);

public:
	CShowTicketWnd(CMainFrame *frame);
	~CShowTicketWnd(void);


public:
	virtual void OnFinalMessage(HWND);
	virtual CDuiString GetSkinFile();
	virtual LPCTSTR GetWindowClassName(void) const;
	virtual void Notify(TNotifyUI &msg);
	virtual void InitWindow();

	DUI_DECLARE_MESSAGE_MAP()
		virtual void OnClick(TNotifyUI& msg);

	void OnAddTrainCodeCb(TNotifyUI& msg);
	

	virtual LRESULT OnSysCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

private:
	CButtonUI* m_pCloseBtn;
	CButtonUI* m_pMaxBtn;
	CButtonUI* m_pRestoreBtn;
	CButtonUI* m_pMinBtn;
	CButtonUI* m_pMenuBtn;

	CMainFrame *m_mainFrame;

	CListUI *m_listTicket;
};

