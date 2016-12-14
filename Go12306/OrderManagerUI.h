#pragma once



class CMainFrame;
class COrderManagerUI
{
public:
	COrderManagerUI(CMainFrame *mainFrame);

	/*@action:���¶����б�
	*/
	int	RefreshOrderListView();

	/*@action: ���¾��嶩����Ϣ
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
