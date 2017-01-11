#pragma once


///
class CMainFrame;
class CTicketModel;

class CShowSeatTypeWnd : public WindowImplBase
{
public:

	static void ShowMessageBox(CMainFrame *frame, RECT rect);

public:
	CShowSeatTypeWnd(CMainFrame *frame);
	~CShowSeatTypeWnd(void);


public:
	virtual void OnFinalMessage(HWND);
	virtual CDuiString GetSkinFile();
	virtual LPCTSTR GetWindowClassName(void) const;
	virtual void Notify(TNotifyUI &msg);
	virtual void InitWindow();

	DUI_DECLARE_MESSAGE_MAP()
		virtual void OnClick(TNotifyUI& msg);

	void OnBtnSeatTypeYDZ(TNotifyUI& msg);
	void OnBtnSeatTypeEDZ(TNotifyUI& msg);
	void OnBtnSeatTypeTDZ(TNotifyUI& msg);
	void OnBtnSeatTypeSWZ(TNotifyUI& msg);
	void OnBtnSeatTypeYW(TNotifyUI& msg);
	void OnBtnSeatTypeRW(TNotifyUI& msg);
	void OnBtnSeatTypeYZ(TNotifyUI& msg);
	void OnBtnSeatTypeRZ(TNotifyUI& msg);
	void OnBtnSeatTypeGJRW(TNotifyUI& msg);


	virtual LRESULT OnSysCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

private:

	void AddSeatType(CButtonUI *pBtnUI);

	CButtonUI* m_pCloseBtn;
	CButtonUI* m_pMaxBtn;
	CButtonUI* m_pRestoreBtn;
	CButtonUI* m_pMinBtn;
	

	CMainFrame *m_mainFrame;


	CButtonUI* m_pBtnSeatTypeYDZ;
	CButtonUI* m_pBtnSeatTypeEDZ;
	CButtonUI* m_pBtnSeatTypeTDZ;
	CButtonUI* m_pBtnSeatTypeSWZ;
	CButtonUI* m_pBtnSeatTypeYW;
	CButtonUI* m_pBtnSeatTypeRW;
	CButtonUI* m_pBtnSeatTypeYZ;
	CButtonUI* m_pBtnSeatTypeRZ;
	CButtonUI* m_pBtnSeatTypeGJRW;
	

};
