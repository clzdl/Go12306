// Go12306.cpp : ����Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "Go12306.h"
#include "MainFrame.h"

#include "OthFunc.h"


CSkinManager* CSkinManager::m_pSkinManager = NULL;



int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE /*hPrevInstance*/, LPSTR /*lpCmdLine*/, int nCmdShow)
{
	HRESULT Hr = ::CoInitialize(NULL);
	if (FAILED(Hr)) return 0;
	HRESULT hRes = ::OleInitialize(NULL);
	// ��ʼ��UI������
	CPaintManagerUI::SetInstance(hInstance);
	// ��ʼ����Դ
	InitResource();

	CMainFrame* pFrame = new CMainFrame();
	if (pFrame == NULL) return 0;
	pFrame->Create(NULL, _T("gogo 12306"), UI_WNDSTYLE_FRAME, 0L, 0, 0, 800, 572);
	pFrame->CenterWindow();
	ShowWindow(*pFrame, SW_SHOW);
	CPaintManagerUI::MessageLoop();
	delete pFrame;
	pFrame = NULL;
	CResourceManager::GetInstance()->Release();

	OleUninitialize();
	::CoUninitialize();
	return 0;
}