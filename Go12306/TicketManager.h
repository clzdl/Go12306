#pragma once

class CMainFrame;
class CTicketModel;
class CTicketManager
{
public:
	CTicketManager(CMainFrame *mainFrame);

	int RefreshTicketListView(CTicketModel *ticketModel, int iIndex);

	void ResetTicketList(); 

private:
	CMainFrame *m_mainFrame;

	CListUI *m_ticketListView;

	static CDuiString m_transLiShi[];
};