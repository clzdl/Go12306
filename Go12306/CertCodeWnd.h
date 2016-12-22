#pragma once

class CCode12306CertUI;

#define MSGID_OK		1
#define MSGID_CANCEL	0
class CCertCodeWnd : public WindowImplBase
{
public:
	static int MessageBox(HWND hParent, LPCTSTR lpstrTitle , std::vector<CDuiPoint> &selPoint)
	{
		CCertCodeWnd* pWnd = new CCertCodeWnd();
		pWnd->Create(hParent, _T("CertCodeWnd"), WS_POPUP | WS_CLIPCHILDREN, WS_EX_TOOLWINDOW);
		pWnd->CenterWindow();
		pWnd->SetTitle(lpstrTitle);
		int ret =  pWnd->ShowModal();

		selPoint = pWnd->GetSelPoint();

		delete pWnd;

		return ret;
	}

	static void ShowMessageBox(HWND hParent, LPCTSTR lpstrTitle)
	{
		CCertCodeWnd* pWnd = new CCertCodeWnd();
		pWnd->Create(hParent, _T("CertCodeWnd"), UI_WNDSTYLE_FRAME, 0);
		pWnd->CenterWindow();
		pWnd->SetTitle(lpstrTitle);
		pWnd->ShowWindow(true);

		delete pWnd;
	}

public:
	CCertCodeWnd(void);
	~CCertCodeWnd(void);

	void SetTitle(LPCTSTR lpstrTitle);

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

	std::vector<CDuiPoint> GetSelPoint() { return m_vecSelPoint; }

private:
	CButtonUI* m_pCloseBtn;
	CButtonUI* m_pMaxBtn;
	CButtonUI* m_pRestoreBtn;
	CButtonUI* m_pMinBtn;
	CButtonUI* m_pMenuBtn;
	CCode12306CertUI *m_pCert12306UI;
	std::vector<CDuiPoint> m_vecSelPoint;
};