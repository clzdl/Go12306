#include "stdafx.h"
#include "Worker.h"
#include "MainFrame.h"
#include "OthFunc.h"

///////////////////////
CTicketWorker::CTicketWorker(CMainFrame *mainFrame)
	:m_mainFrame(mainFrame)
{

}
CTicketWorker::~CTicketWorker()
{

}

void CTicketWorker::run()
{
	while (1)
	{
		m_vecTicket->clear();


		int iRetFlag = Client12306Manager::Instance()->QueryLeftTicket(UnicodeToUtf8(m_strBegPlace.GetData()),
			UnicodeToUtf8(m_strEndPlace.GetData()),
			UnicodeToUtf8(m_strTravelTime.GetData()),
			*m_vecTicket, m_ticketType);


		SendMessage(m_mainFrame->GetHWND(), WM_PROGRESS_CLOSE, NULL, NULL);
		SendMessage(m_mainFrame->GetHWND(), WM_TICKET_QUERY, iRetFlag, NULL);

		break;

	}
}


///////////////////////
COrderWorker::COrderWorker(CMainFrame *mainFrame)
	:m_mainFrame(mainFrame)
{

}
COrderWorker::~COrderWorker()
{

}

void COrderWorker::run()
{
	while (1)
	{



		int iRetFlag = Client12306Manager::Instance()->QueryMyOrder(UnicodeToUtf8(m_strBegDate.GetData()),
			UnicodeToUtf8(m_strEndDate.GetData()),
			UnicodeToUtf8(m_strType.GetData()),
			UnicodeToUtf8(m_strSeqTrainName.GetData()), *m_mapTicket);


		SendMessage(m_mainFrame->GetHWND(), WM_PROGRESS_CLOSE, NULL, NULL);


		SendMessage(m_mainFrame->GetHWND(), WM_ORDER_QUERY, iRetFlag, NULL);

		break;

	}


}



