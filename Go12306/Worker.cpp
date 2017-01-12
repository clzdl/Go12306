#include "stdafx.h"
#include "Worker.h"
#include "MainFrame.h"
#include "OthFunc.h"
#include "CertCodeWnd.h"
#include "MsgWnd.h"
#include "OrderWaitTimeWnd.h"

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
		m_mapTicket->clear();


		int iRetFlag = Client12306Manager::Instance()->QueryLeftTicket(UnicodeToUtf8(m_strBegPlace.GetData()),
			UnicodeToUtf8(m_strEndPlace.GetData()),
			UnicodeToUtf8(m_strTravelTime.GetData()),
			*m_mapTicket, m_ticketType);


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


///////////////////////


CPollTicketWorker::CPollTicketWorker(CMainFrame *mainFrame)
	:m_mainFrame(mainFrame)
{
	m_setTrain.insert(_T("K5202"));
}
CPollTicketWorker::~CPollTicketWorker()
{

}

#define CHECK_ERROR(frame , ret)		if(E_OK != ret ){   \
											SendMessage(frame->GetHWND(), WM_POLL_TICKET_PROCESS, ret, NULL); \
											SendMessage(frame->GetHWND(), WM_PROGRESS_CLOSE, NULL, NULL); \
											break; }

void CPollTicketWorker::run()
{
	while (1)
	{
		m_mapTicket->clear();
		
		_ERRNO err = E_OK;
		
		err = Client12306Manager::Instance()->QueryLeftTicket(UnicodeToUtf8(m_strBegPlace.GetData()),
			UnicodeToUtf8(m_strEndPlace.GetData()),
			UnicodeToUtf8(m_strTravelTime.GetData()),
			*m_mapTicket);

		CHECK_ERROR(m_mainFrame , err);


		for (std::set<CDuiString>::iterator it = m_setTrain.begin(); it != m_setTrain.end(); ++it)
		{
			std::map<CDuiString, CTicketModel>::iterator v = m_mapTicket->find( *it );

			if (v == m_mapTicket->end())
				continue;


			CTicketModel &tm = v->second;

			std::string token, leftTicketString, keyCheckIsChange;


			err = Client12306Manager::Instance()->CheckUser();
			CHECK_ERROR(m_mainFrame, err);

			err = Client12306Manager::Instance()->SubmitOrderRequest(&tm);
			CHECK_ERROR(m_mainFrame, err);


			err = Client12306Manager::Instance()->InitDc(token, leftTicketString, keyCheckIsChange);
			CHECK_ERROR(m_mainFrame, err);

			err = Client12306Manager::Instance()->QueryPassenger();
			CHECK_ERROR(m_mainFrame, err);

			std::set<CPassenger> ;
			CDuiString seatType = GetLeftTicketSeatType(tm);

			if (seatType.IsEmpty())
				continue;

			std::vector<CPassengerTicket> vecPT;

			CreatePassengerTicket(vecPT, seatType, tm);

			CCheckOrderInfoResult result;
			err = Client12306Manager::Instance()->CheckOrderInfo(vecPT, result);
				
			CHECK_ERROR(m_mainFrame, err);

			if (result.GetSubmitStatus() != "true" && result.GetSubmitStatus() != "TRUE")
			{
				CMsgWnd::MessageBox(NULL, _T("提示"), _T("出票失败"));

				break;

			}

			char randCode[128] = { 0 };
			if (result.GetIfShowPassCode() == "Y")
			{///需要显示验证码

				std::vector<CDuiPoint> selPoint;
				if (MSGID_CANCEL == CCertCodeWnd::MessageBox(NULL, _T("请输入验证码"), selPoint))
					break;


				///
				if (selPoint.empty())
				{
					CMsgWnd::MessageBox(NULL, _T("提示"), _T("验证码未选择"));

					break;
				}


				for (std::vector<CDuiPoint>::iterator it = selPoint.begin(); it != selPoint.end(); ++it)
				{
					if (it == selPoint.begin())
						sprintf(randCode, "%d,%d", it->x, it->y);
					else
						sprintf(randCode, "%s,%d,%d", randCode, it->x, it->y);
				}

			}


			///

			CGetQueqeCountResult queRes;
			if (E_OK != Client12306Manager::Instance()->getQueueCount(&tm, token, leftTicketString, UnicodeToUtf8(vecPT[0].GetSeatType().GetData()), queRes))
			{
				CMsgWnd::MessageBox(NULL, _T("提示"), Utf8ToUnicode(Client12306Manager::Instance()->GetLastErrInfo()).c_str());
				break;
			}

			if (queRes.GetOp2() == "true")
			{
				CMsgWnd::MessageBox(NULL, _T("提示"), _T("目前排队人数已经超过余票张数，请您选择其他席别或车次"));
				break;
			}
			/*else
			{
				if (atoi(queRes.GetCountT().c_str()) > 0)
				{
					CDuiString tmp;
					tmp.Format(_T("目前排队人数 %s 人"), Utf8ToUnicode(queRes.GetCountT()).c_str());
					CMsgWnd::MessageBox(NULL , _T("提示"), tmp);
				}

			}*/


			/////
			err = Client12306Manager::Instance()->ConfirmSingleForQueue(vecPT, &tm, token, leftTicketString, keyCheckIsChange, randCode);
			CHECK_ERROR(m_mainFrame, err);


			COrderWaitTimeWnd *pWaitTimeWnd = COrderWaitTimeWnd::GetInstance(NULL, _T("稍等片刻"), token);

			std::auto_ptr<COrderWaitTimeWnd> ptrWnd(pWaitTimeWnd);

			err = (_ERRNO)ptrWnd->ShowModal();

			CHECK_ERROR(m_mainFrame, err);
			
		}


		break;

	}

	SendMessage(m_mainFrame->GetHWND(), WM_PROGRESS_CLOSE, NULL, NULL);

}


CDuiString CPollTicketWorker::GetLeftTicketSeatType(CTicketModel &tm)
{
	CDuiString seatType;
	for (std::set<CDuiString>::iterator s = m_setSeatType.begin(); s != m_setSeatType.end(); ++s)
	{
		if (*s == _SEAT_TYPE_YDZ)
		{////一等座
			if (tm.GetZyNum() == _T("--") || tm.GetZyNum() == _T("无"))
				continue;

			seatType = _SEAT_TYPE_YDZ;
			break;
		}
		else if (*s == _SEAT_TYPE_EDZ)
		{////二等座
			if (tm.GetZeNum() == _T("--") || tm.GetZeNum() == _T("无"))
				continue;

			seatType = _SEAT_TYPE_EDZ;
			break;
		}
		else if (*s == _SEAT_TYPE_TDZ)
		{////特等座
			if (tm.GetTzNum() == _T("--") || tm.GetTzNum() == _T("无"))
				continue;

			seatType = _SEAT_TYPE_TDZ;
			break;
		}
		else if (*s == _SEAT_TYPE_SWZ)
		{////商务座
			if (tm.GetSwzNum() == _T("--") || tm.GetSwzNum() == _T("无"))
				continue;

			seatType = _SEAT_TYPE_SWZ;
			break;
		}
		else if (*s == _SEAT_TYPE_YW)
		{////硬卧
			if (tm.GetYwNum() == _T("--") || tm.GetYwNum() == _T("无"))
				continue;

			seatType = _SEAT_TYPE_YW;
			break;
		}
		else if (*s == _SEAT_TYPE_RW)
		{////软卧
			if (tm.GetRwNum() == _T("--") || tm.GetRwNum() == _T("无"))
				continue;

			seatType = _SEAT_TYPE_RW;
			break;
		}
		else if (*s == _SEAT_TYPE_YZ)
		{////硬座
			if (tm.GetYzNum() == _T("--") || tm.GetYzNum() == _T("无"))
				continue;

			seatType = _SEAT_TYPE_YZ;
			break;
		}
		else if (*s == _SEAT_TYPE_RZ)
		{////软座
			if (tm.GetRzNum() == _T("--") || tm.GetRzNum() == _T("无"))
				continue;

			seatType = _SEAT_TYPE_RZ;
			break;
		}
		else if (*s == _SEAT_TYPE_GJRW)
		{////高级软卧
			if (tm.GetGrNum() == _T("--") || tm.GetGrNum() == _T("无"))
				continue;

			seatType = _SEAT_TYPE_GJRW;
			break;
		}


	}

	return seatType;
}


void CPollTicketWorker::CreatePassengerTicket(std::vector<CPassengerTicket> &vecPT, CDuiString seatType, CTicketModel &tm)
{

	 for(std::set<CPassenger>::iterator it = m_setPassenger.begin(); it != m_setPassenger.end(); ++it)
	 {
		 CPassenger &passenger = const_cast<CPassenger&>(*it);
		 CPassengerTicket pt;
		 pt.SetSeatType(seatType);

		 

		 pt.SetTicketType(passenger.GetType());
		 pt.SetPassengerName(passenger.GetName());
		 pt.SetPassengerType(passenger.GetType());
		 pt.SetIdType(passenger.GetCardType());
		 pt.SetIdNo(passenger.GetCardNo());
		 pt.SetMobileNo(passenger.GetPhoneNo());

		 pt.SetSaveStatus(_T("N"));

		 vecPT.push_back(pt);
	 }


}

