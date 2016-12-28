#pragma once
#include "SkinManager.h"
#include "TicketModel.h"
#include "OrderModel.h"
#include <set>

#include "Poco/ThreadPool.h"
#include "Poco/Thread.h"
#include "Poco/Runnable.h"

#include "EditCombo.h"
#include "Worker.h"

using Poco::Thread;
using Poco::ThreadPool;
using Poco::Runnable;


class COrderManagerUI;
class CTicketManager;
class CTicketWorker;
class COrderWorker;
class CProgressDlg;
class CPassengerManagerUI;



class CMainFrame : public WindowImplBase, public SkinChangedReceiver
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
	
	//void OnLClick(CControlUI *pControl);

	void TicketQueryCb(TNotifyUI& msg);
	void OrderQueryCb(TNotifyUI& msg);
	void OrderTicketCb(TNotifyUI& msg);
	void TicketPlaceChgCb(TNotifyUI& msg);
	void MenuBtnCb(TNotifyUI& msg);


	
	void TrainAllChkBtnCb(TNotifyUI& msg);
	void TrainGcCkgBtnCb(TNotifyUI& msg);
	void TrainDCkgBtnCb(TNotifyUI& msg);
	void TrainZCkgBtnCb(TNotifyUI& msg);
	void TrainTCkgBtnCb(TNotifyUI& msg);
	void TrainKCkgBtnCb(TNotifyUI& msg);
	void TrainOCkgBtnCb(TNotifyUI& msg);
	void OrderExpandBtn(TNotifyUI& msg);

	void TxtChgBegPlaceCb(TNotifyUI& msg);
	void TxtChgEndPlaceCb(TNotifyUI& msg);

	void OnPollTicketProcessCb(TNotifyUI& msg);
	

	CPaintManagerUI* GetPaintManagerUI() { return &m_pm; }


	DUI_DECLARE_MESSAGE_MAP()


private:

	int QueryTicket(CDuiString begPlace , CDuiString endPlace, CDuiString travelTime, _TICKET_TYPE ticketType);


	_ERRNO QueryMyOrder(CDuiString begDate, CDuiString endDate , CDuiString type, CDuiString seqTrainName);


	/*@action: 刷新余票列表
	*/
	_ERRNO RefreshTicketListView();


	/*@action: 是否需要显示
	
	*/
	bool IsShowTrain(CDuiString trainNo);

	/*@action: 刷新车票类型
	*/
	void RefreshAllTrainCHeckBox(bool flag);
	 
	/*@action: 刷新我的订单
	*/
	_ERRNO RefreshMyOrderListView();

	/*@action:站点刷新
	*/
	_ERRNO StationComboRefresh(CEditComboUI *pEditComboUI , std::vector<CStation*> &vec);

	/*@action:
	*/
	_ERRNO PollTicketProcess();


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


	CEditComboUI *m_pBegPlaceCombo;
	CEditComboUI *m_pEndPlaceCombo;
	CDateTimeUI *m_pTicketLeaveTimeUI;

	COptionUI *m_pTikcetAdult;
	COptionUI *m_pTikcetStudent;
	CCheckBoxUI *m_pPollQueryTag;
	CButtonUI *m_btnQueryTicket;

	
		 
	std::map<CDuiString,CTicketModel> m_mapTicket;
	std::map<string, COrderModel> m_mapMyOrder;

	////显示车次类型
	std::set< _TRAIN_TYPE > m_setShowTrainType;

	bool m_bAllTrainType;

	ThreadPool m_tpWorker;

	CTicketWorker *m_tWorker;

	COrderWorker *m_tOrderWorker;

	CPollTicketWorker *m_tPollTicketWorker;

	COrderManagerUI *m_pOrderManagerUI;
	CTicketManager *m_pTicketManagerUI;
	CPassengerManagerUI *m_pPassengerManagerUI;


	CProgressDlg* m_pProgressDlg;

	int m_iWaitTime;
	const int m_iBaseTime;
	bool m_bPoolFlag;


	CDuiString m_sBegPlace;
	CDuiString m_sEndPlace;
	CDuiString m_sTravelTime;
	_TICKET_TYPE m_ticketType;

};

