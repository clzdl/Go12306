#pragma once

#include "Passenger.h"

class CMainFrame;

class CPassengerManagerUI
{
public:
	CPassengerManagerUI(CMainFrame *mainFrame);
	~CPassengerManagerUI();

	_ERRNO RefreshPassengerListView(std::map<std::string, CPassenger> &objMap);


private:

	CMainFrame *m_mainFrame;

	CListUI *m_pPassengerListView;
};