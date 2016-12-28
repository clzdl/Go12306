#pragma once


#include "OrderModel.h"
class CMainFrame;
class COrderManagerUI
{
public:
	COrderManagerUI(CMainFrame *mainFrame);

	/*@action:���¶����б�
	*/
	_ERRNO	RefreshOrderListView(std::map<string, COrderModel> &objMap);

	/*@action: ���¾��嶩����Ϣ
	*/
	_ERRNO RefreshOrderDetailList(CListUI *dListUI, bool check);

private:


	CHorizontalLayoutUI* CreateDetailListHeader(CDuiString lstTicketListName , COrderModel &orderModel);


	CListUI* CreateTicketList(CDuiString lstTicketListName, COrderModel &orderModel);

	CListHeaderUI* CreateTicketListHeader();

	CListContainerElementUI* CreateListContainerEleUI(COrderModel &orderModel , COrderTicketModel &orderTicketModel);

	CMainFrame *m_mainFrame;


	CListUI *m_pOderListView;
};
