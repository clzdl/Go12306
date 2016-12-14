#pragma once
#include "SkinManager.h"
#include "TicketModel.h"
#include "OrderModel.h"
#include <set>

#include "Poco/ThreadPool.h"
#include "Poco/Thread.h"
#include "Poco/Runnable.h"


using Poco::Thread;
using Poco::ThreadPool;
using Poco::Runnable;


class COrderManagerUI;
class CTicketManager;
class CTicketWorker;
class COrderWorker;
class CProgressDlg;

class CMainFrame : public WindowImplBase, public CWebBrowserEventHandler, public SkinChangedReceiver
{

public:
	CMainFrame();
	CControlUI* CreateControl(LPCTSTR pstrClass);

	void InitWindow();

	virtual BOOL Receive(SkinChangedParam param);

	/////////////////////////////////////////////////////////////////////////
public:
	virtual HRESULT STDMETHODCALLTYPE UpdateUI(void)
	{
		return S_OK;
	}
	virtual HRESULT STDMETHODCALLTYPE GetHostInfo(CWebBrowserUI* pWeb,
		/* [out][in] */ DOCHOSTUIINFO __RPC_FAR *pInfo)
	{
		if (pInfo != NULL) {
			pInfo->dwFlags |= DOCHOSTUIFLAG_NO3DBORDER | DOCHOSTUIFLAG_NO3DOUTERBORDER | DOCHOSTUIFLAG_SCROLL_NO;
		}
		return S_OK;
	}
	virtual HRESULT STDMETHODCALLTYPE ShowContextMenu(CWebBrowserUI* pWeb,
		/* [in] */ DWORD dwID,
		/* [in] */ POINT __RPC_FAR *ppt,
		/* [in] */ IUnknown __RPC_FAR *pcmdtReserved,
		/* [in] */ IDispatch __RPC_FAR *pdispReserved)
	{
		return E_NOTIMPL;
		//返回 E_NOTIMPL 正常弹出系统右键菜单
		//返回S_OK 则可屏蔽系统右键菜单
	}
public:
	DuiLib::CDuiString GetSkinFile();

	LPCTSTR GetWindowClassName() const;

	UINT GetClassStyle() const;

	void OnFinalMessage(HWND hWnd);

	LPCTSTR QueryControlText(LPCTSTR lpstrId, LPCTSTR lpstrType);

	LRESULT OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled);

	LRESULT HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	void Notify(TNotifyUI& msg);
	
	void OnLClick(CControlUI *pControl);

	CPaintManagerUI* GetPaintManagerUI() { return &m_pm; }

private:

	int QueryTicket(CDuiString begPlace , CDuiString endPlace, CDuiString travelTime );


	int QueryMyOrder();


	/*@action: 刷新余票列表
	*/
	int RefreshTicketListView();


	/*@action: 是否需要显示
	
	*/
	bool IsShowTrain(CDuiString trainNo);

	/*@action: 刷新车票类型
	*/
	void RefreshAllTrainCHeckBox(bool flag);
	 
	/*@action: 刷新我的订单
	*/
	int RefreshMyOrderListView();


	CButtonUI* m_pCloseBtn;
	CButtonUI* m_pMaxBtn;
	CButtonUI* m_pRestoreBtn;
	CButtonUI* m_pMinBtn;
	CButtonUI* m_pSkinBtn;
	CMenuWnd* m_pMenu;
	std::map<CDuiString, bool> m_MenuInfos;
	CTrayIcon m_trayIcon;


	COptionUI *m_pOptTicketQuery;
	COptionUI *m_pOptOrderManage;
	COptionUI *m_pOptUserManage;

	
		 
	std::vector<CTicketModel> m_vecTicket;
	std::map<string, COrderModel> m_mapMyOrder;

	////显示车次类型
	std::set< _TRAIN_TYPE > m_setShowTrainType;

	bool m_bAllTrainType;

	ThreadPool m_tpWorker;

	CTicketWorker *m_tWorker;

	COrderWorker *m_tOrderWorker;

	COrderManagerUI *m_pOrderManagerUI;
	CTicketManager *m_pTicketManagerUI;

	CProgressDlg* m_pProgressDlg;
};


#include "ProgressDlg.h"

////ticket worker
class CTicketWorker : public Runnable
{
public:
	CTicketWorker(CMainFrame *mainFrame);
	~CTicketWorker();

	void run();

	

	void SetVecTicket(std::vector<CTicketModel> *vecTicket)
	{
		m_vecTicket = vecTicket;
	}

	void SetQueryParam(CDuiString begPlace, CDuiString endPlace, CDuiString travelTime, _TICKET_TYPE ticketType)
	{
		m_strBegPlace = begPlace;
		m_strEndPlace = endPlace;
		m_strTravelTime = travelTime;
		m_ticketType = ticketType;
	}

private:
	CDuiString  m_strBegPlace;
	CDuiString  m_strEndPlace;
	CDuiString  m_strTravelTime;
	_TICKET_TYPE m_ticketType;

	std::vector<CTicketModel> *m_vecTicket;

	CMainFrame *m_mainFrame;

	
};


#include "OrderModel.h"
////myorder worker
class COrderWorker : public Runnable
{
public:
	COrderWorker(CMainFrame *mainFrame);
	~COrderWorker();

	void run();


	void SetMapOrder(std::map<string, COrderModel>  *mapTicket)
	{
		m_mapTicket = mapTicket;
	}


	void SetQueryParam(CDuiString begDate, CDuiString endDate, CDuiString seqTrainName)
	{
		m_strBegDate = begDate;
		m_strEndDate = endDate;
		m_strSeqTrainName = seqTrainName;
	}

private:
	CDuiString  m_strBegDate;
	CDuiString  m_strEndDate;
	CDuiString  m_strSeqTrainName;

	std::map<string,COrderModel> *m_mapTicket;

	CMainFrame *m_mainFrame;


};