
#include "stdafx.h"
#include "ProgressDlg.h"



DUI_BEGIN_MESSAGE_MAP(CProgressDlg, WindowImplBase)
DUI_END_MESSAGE_MAP()

CProgressDlg::CProgressDlg(void)
{
}

CProgressDlg::~CProgressDlg(void)
{
}

void CProgressDlg::OnFinalMessage(HWND hWnd)
{
	__super::OnFinalMessage(hWnd);
	delete this;
}

DuiLib::CDuiString CProgressDlg::GetSkinFile()
{
	return _T("XML_PROGRESS_DLG");
}

LPCTSTR CProgressDlg::GetWindowClassName(void) const
{
	return _T("ProgressDLg");
}


LRESULT CProgressDlg::HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	bHandled = FALSE;
	return 0;
}

void CProgressDlg::Notify(TNotifyUI &msg)
{
	return WindowImplBase::Notify(msg);
}

LRESULT CProgressDlg::OnSysCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	bHandled = FALSE;
	return 0L;
}

void CProgressDlg::InitWindow()
{
}


CProgressDlg* CProgressDlg::CreateDlg(HWND hParent)
{
	CProgressDlg* pDlg = new CProgressDlg();
	pDlg->Create(hParent, _T("ProgressDLg"), WS_POPUP | WS_CLIPCHILDREN, WS_EX_TOOLWINDOW);
	pDlg->CenterWindow();

	return pDlg;
}