#pragma once

#include "Code12306Cert.h"
#define MSGID_OK		1
#define MSGID_CANCEL	0
class CLoginWnd : public WindowImplBase
{
public:
	
	CLoginWnd(void);
	~CLoginWnd(void);

	void SetMsg(LPCTSTR lpstrMsg);
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

private:


	CButtonUI* m_pCloseBtn;
	CButtonUI* m_pMaxBtn;
	CButtonUI* m_pRestoreBtn;
	CButtonUI* m_pMinBtn;
	CButtonUI* m_pMenuBtn;


	CEditUI* m_pEdtUserName;
	CEditUI* m_pEdtUserPass;


	CCode12306CertUI *m_code12306UI;
};

