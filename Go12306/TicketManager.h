#pragma once

class CMainFrame;
class CTicketModel;
class CTicketManager
{
public:
	CTicketManager(CMainFrame *mainFrame);

	_ERRNO RefreshTicketListView(CTicketModel *ticketModel);

	void ResetTicketList(); 

private:
	CMainFrame *m_mainFrame;

	CListUI *m_ticketListView;

	static CDuiString m_transLiShi[];
};