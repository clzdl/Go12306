#pragma once


#include "ProgressDlg.h"
#include "OrderModel.h"
#include "Client12306Manager.h"


class  CMainFrame;
////ticket worker
class CTicketWorker : public Runnable
{
public:
	CTicketWorker(CMainFrame *mainFrame);
	~CTicketWorker();

	void run();



	void SetTicketContainer(std::map<CDuiString,CTicketModel> *mapTicket)
	{
		m_mapTicket = mapTicket;
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

	std::map<CDuiString,CTicketModel> *m_mapTicket;

	CMainFrame *m_mainFrame;


};



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


	void SetQueryParam(CDuiString begDate, CDuiString endDate, CDuiString type, CDuiString seqTrainName)
	{
		m_strBegDate = begDate;
		m_strEndDate = endDate;
		m_strType = type;
		m_strSeqTrainName = seqTrainName;
	}

private:
	CDuiString  m_strBegDate;
	CDuiString  m_strEndDate;
	CDuiString  m_strType;
	CDuiString  m_strSeqTrainName;

	std::map<string, COrderModel> *m_mapTicket;

	CMainFrame *m_mainFrame;


};


////myorder worker
class CPollTicketWorker : public Runnable
{
public:
	CPollTicketWorker(CMainFrame *mainFrame);
	~CPollTicketWorker();

	void run();


	void SetTicketContainer(std::map<CDuiString, CTicketModel> *mapTicket)
	{
		m_mapTicket = mapTicket;
	}


	void SetQueryParam(CDuiString begPlace, CDuiString endPlace, CDuiString travelTime, _TICKET_TYPE ticketType)
	{
		m_strBegPlace = begPlace;
		m_strEndPlace = endPlace;
		m_strTravelTime = travelTime;
		m_ticketType = ticketType;
	}

private:

	CDuiString GetLeftTicketSeatType(CTicketModel &tm);
	
	void CreatePassengerTicket(std::vector<CPassengerTicket> &vecPT ,CDuiString seatType, CTicketModel &tm);

	CDuiString  m_strBegPlace;
	CDuiString  m_strEndPlace;
	CDuiString  m_strTravelTime;
	_TICKET_TYPE m_ticketType;

	std::map<CDuiString ,CTicketModel> *m_mapTicket;

	CMainFrame *m_mainFrame;

	///需要订购的车次车票
	std::set<CDuiString> m_setTrain;

	///需要订购的乘客信息
	std::set<CPassenger> m_setPassenger;

	///需要订购的席别
	std::set<CDuiString> m_setSeatType;

};