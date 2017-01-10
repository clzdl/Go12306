#pragma once


///
#define MSGID_OK		1
#define MSGID_CANCEL	0
class CShowTicketWnd : public WindowImplBase
{
public:

	static void ShowMessageBox(HWND hParent, RECT rect)
	{
		CShowTicketWnd* pWnd = new CShowTicketWnd();
		pWnd->Create(hParent, _T("ShowTicketWnd"), UI_WNDSTYLE_FRAME, 0);
		
		RECT winRect;
		::GetWindowRect(pWnd->GetHWND(), &winRect);

		POINT orgPt;

		if (rect.right > winRect.right - winRect.left)
		{
			///¿¿°´Å¥×ó²àÏÔÊ¾
			orgPt.x = rect.right - (winRect.right - winRect.left);
			orgPt.y = rect.top - (winRect.bottom - winRect.top);
		}
		else
		{
			///¿¿°´Å¥ÓÒ²àÏÔÊ¾
			orgPt.x = rect.left;
			orgPt.y = rect.top - (winRect.bottom - winRect.top);

		}

		::ClientToScreen(hParent, &orgPt);


		::SetWindowPos(pWnd->GetHWND(), NULL, orgPt.x, orgPt.y, -1, -1, SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE);
		pWnd->ShowWindow(true);
	}

public:
	CShowTicketWnd(void);
	~CShowTicketWnd(void);


public:
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
	CButtonUI* m_pMaxBtn;
	CButtonUI* m_pRestoreBtn;
	CButtonUI* m_pMinBtn;
	CButtonUI* m_pMenuBtn;
};

