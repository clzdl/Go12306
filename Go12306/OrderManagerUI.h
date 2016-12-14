#pragma once



class CMainFrame;
class COrderManagerUI
{
public:
	COrderManagerUI(CMainFrame *mainFrame);

	/*@action:更新订单列表
	*/
	int	RefreshOrderListView();

	/*@action: 更新具体订单信息
	*/
	int RefreshOrderDetailList(CListUI *dListUI, bool check);

private:


	CHorizontalLayoutUI* CreateDetailListHeader(CDuiString lstTicketListName);


	CListUI* CreateTicketList(CDuiString lstTicketListName);

	CListHeaderUI* CreateTicketListHeader();

	CListContainerElementUI* CreateListContainerEleUI();

	CMainFrame *m_mainFrame;


	CListUI *m_pOderListView;
};
