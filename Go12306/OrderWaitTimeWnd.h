#pragma once

#include "Client12306Manager.h"

class COrderWaitTimeWnd : public WindowImplBase
{
public:
	static COrderWaitTimeWnd* GetInstance(HWND hParent, LPCTSTR lpstrTitle , std::string token )
	{
		COrderWaitTimeWnd* pWnd = new COrderWaitTimeWnd(token);
		pWnd->Create(hParent, _T("msgwnd"), WS_POPUP | WS_CLIPCHILDREN, WS_EX_TOOLWINDOW);
		pWnd->CenterWindow();
		pWnd->SetTitle(lpstrTitle);
		return pWnd;
		
	}


public:

	COrderWaitTimeWnd(std::string token);

	~COrderWaitTimeWnd(void);

	void SetMsg(LPCTSTR lpstrMsg);
	void SetTitle(LPCTSTR lpstrTitle);

	CDuiString GetErrString() { return  m_sErrInfo; }

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
	/*@action:
	*/
	void InitilizeTimer();

	/*@action:
	*/
	void QueryOrderWaitTime();

	CButtonUI* m_pCloseBtn;
	CButtonUI* m_pMaxBtn;
	CButtonUI* m_pRestoreBtn;
	CButtonUI* m_pMinBtn;
	CButtonUI* m_pMenuBtn;

	std::string m_tokenString;

	CQueryOrderWaitTimeResult m_orderWaitTimeResult;

	int m_iNextRequestTime;
	int m_iDispatchTime;
	CDuiString m_sErrInfo;
};
