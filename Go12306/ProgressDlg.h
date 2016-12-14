#pragma once
//////////////////////////////////////////////////////////////////////////
///
#define MSGID_OK		1
#define MSGID_CANCEL	0
class CProgressDlg : public WindowImplBase
{
public:
	static CProgressDlg* CreateDlg(HWND hParent);

public:
	CProgressDlg(void);
	~CProgressDlg(void);

public:
	virtual void OnFinalMessage(HWND);
	virtual CDuiString GetSkinFile();
	virtual LPCTSTR GetWindowClassName(void) const;
	virtual void Notify(TNotifyUI &msg);
	virtual void InitWindow();

	


	DUI_DECLARE_MESSAGE_MAP()


	virtual LRESULT OnSysCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);


private:



};
