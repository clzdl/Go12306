#include "stdafx.h"
#include "OrderTicketWnd.h"
#include "Client12306Manager.h"
#include "OthFunc.h"
#include "MsgWnd.h"
#include "CertCodeWnd.h"
#include "OrderWaitTimeWnd.h"


DUI_BEGIN_MESSAGE_MAP(COrderTicketWnd, WindowImplBase)
	DUI_ON_MSGTYPE(DUI_MSGTYPE_CLICK, OnClick)
	DUI_ON_MSGTYPE_CTRNAME(DUI_MSGTYPE_ITEMACTIVATE, _T("passengerListItem"), OnPassengerListItemClick)
	DUI_ON_CLICK_CTRNAME(_T("BTN_DELETE_ORDER") , OnDeleteOrderTicket)
	DUI_ON_CLICK_CTRNAME(_T("TICKET_ORDER_SUBMIT"), OnTicketOrderSubmit)
	
DUI_END_MESSAGE_MAP()


COrderTicketWnd::COrderTicketWnd(CTicketModel *ticket , CDuiString orderDate)
	:m_pTicket(ticket),
	m_sOrderDate(orderDate)
{

}
COrderTicketWnd::~COrderTicketWnd()
{

}


void COrderTicketWnd::OnFinalMessage(HWND hWnd)
{
	__super::OnFinalMessage(hWnd);
	delete this;
}
CDuiString COrderTicketWnd::GetSkinFile()
{
	return _T("XML_ORDER_TICKET_WND");
}
LPCTSTR COrderTicketWnd::GetWindowClassName(void) const
{
	return _T("OrderTicketWnd");
}

void COrderTicketWnd::Notify(TNotifyUI &msg)
{
	return WindowImplBase::Notify(msg);
}
void COrderTicketWnd::InitWindow()
{
	m_pCloseBtn = static_cast<CButtonUI*>(m_pm.FindControl(_T("closebtn")));
	m_pMaxBtn = static_cast<CButtonUI*>(m_pm.FindControl(_T("maxbtn")));
	m_pRestoreBtn = static_cast<CButtonUI*>(m_pm.FindControl(_T("restorebtn")));
	m_pPassengerListView = static_cast<CListUI*>(m_pm.FindControl(_T("passengerListView")));
	
	m_pOrderListView = static_cast<CListUI*>(m_pm.FindControl(_T("orderListView")));

	CLabelUI *pLabelUI = static_cast<CLabelUI*>(m_pm.FindControl(_T("ticketDate")));
	pLabelUI->SetText(m_sOrderDate);

	pLabelUI = static_cast<CLabelUI*>(m_pm.FindControl(_T("trainNo")));
	pLabelUI->SetText(m_pTicket->GetStationTrainCode() + _T(" ��"));

	pLabelUI = static_cast<CLabelUI*>(m_pm.FindControl(_T("begPlace")));
	pLabelUI->SetText(m_pTicket->GetFromStationName() + _T(" վ(") + m_pTicket->GetStartTime() + _T(")"));

	pLabelUI = static_cast<CLabelUI*>(m_pm.FindControl(_T("endPlace")));
	pLabelUI->SetText(m_pTicket->GetToStationName() + _T(" վ(") + m_pTicket->GetArriveTime() + _T(")"));

	///�û�У��
	bool bCheck = false;


	_ERRNO err = E_OK;
	if (E_OK != (err = Client12306Manager::Instance()->CheckUser()))
	{
		if(E_OK != CheckErr(err))
			CMsgWnd::MessageBox(GetHWND() , _T("��ʾ") , Utf8ToUnicode( Client12306Manager::Instance()->GetLastErrInfo()).c_str());
		
		PostMessage(WM_CLOSE);

		return;
	}

	if (E_OK != (err = Client12306Manager::Instance()->SubmitOrderRequest(m_pTicket)))
	{
		if (E_OK != CheckErr(err))
			CMsgWnd::MessageBox(GetHWND(), _T("��ʾ"), Utf8ToUnicode(Client12306Manager::Instance()->GetLastErrInfo()).c_str());


		PostMessage(WM_CLOSE);

		return;
	}


	if (E_OK != (err = Client12306Manager::Instance()->InitDc(m_strToken , m_strLeftTicketString , m_strKeyCheckIsChange)))
	{
		if (E_OK != CheckErr(err))
			CMsgWnd::MessageBox(GetHWND(), _T("��ʾ"), Utf8ToUnicode(Client12306Manager::Instance()->GetLastErrInfo()).c_str());

		PostMessage(WM_CLOSE);

		return;
	}

	RefreshTicketSeatInfo();


	RefreshPassenger();
}

void COrderTicketWnd::RefreshPassenger()
{
	std::map < std::string , CPassenger> &objMap =  Client12306Manager::Instance()->GetPassenger();

	for (std::map<std::string, CPassenger>::iterator it = objMap.begin(); it != objMap.end(); ++it)
	{
		CPassenger &passenger = it->second;

		///�����
		CListContainerElementUI* pListItem = new CListContainerElementUI();
		pListItem->SetName(_T("passengerListItem"));
		pListItem->SetChildVAlign(DT_VCENTER);
		pListItem->SetFixedHeight(20);
		pListItem->SetUserData(passenger.GetCardNo());

		m_pPassengerListView->Add(pListItem);


		///
		CLabelUI *pElement = new CLabelUI();
		pElement->SetText(passenger.GetName());
		pElement->SetAttribute(_T("align"), _T("center"));
		pListItem->Add(pElement);

		///
		pElement = new CLabelUI();
		pElement->SetText(passenger.GetCardTypeName());
		pElement->SetAttribute(_T("align"), _T("center"));
		pListItem->Add(pElement);

		///
		pElement = new CLabelUI();
		pElement->SetText(passenger.GetCardNo());
		pElement->SetAttribute(_T("align"), _T("center"));
		pListItem->Add(pElement);

	}
}

void COrderTicketWnd::OnClick(TNotifyUI &msg)
{
	CDuiString sName = msg.pSender->GetName();
	sName.MakeLower();

	if (msg.pSender == m_pCloseBtn) {
		Close();
		return;
	}
	else if (msg.pSender == m_pMaxBtn) {
		SendMessage(WM_SYSCOMMAND, SC_MAXIMIZE, 0); return;
	}
	else if (msg.pSender == m_pRestoreBtn) {
		SendMessage(WM_SYSCOMMAND, SC_RESTORE, 0); return;
	}
	
	else if (sName.CompareNoCase(_T("confirm_btn")) == 0)
	{
		Close();
	}
	else if (sName.CompareNoCase(_T("cancel_btn")) == 0)
	{
		Close();
	}
}


LRESULT COrderTicketWnd::HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{


	bHandled = FALSE;
	return 0;
}

void COrderTicketWnd::OnPassengerListItemClick(TNotifyUI& msg)
{
	int iSel = m_pPassengerListView->GetCurSel();

	CListContainerElementUI *pEleContainer = static_cast<CListContainerElementUI*>(m_pPassengerListView->GetItemAt(iSel));

	CDuiString sCardNo =  pEleContainer->GetUserData();


	CPassenger *passenger = Client12306Manager::Instance()->GetPassengerByCardNo(UnicodeToUtf8(sCardNo.GetData()));

	if (!passenger)
		return;


	AddOrderTicketList(passenger );

	


}

void COrderTicketWnd::AddOrderTicketList(CPassenger *passenger)
{
	///�����
	CListContainerElementUI* pListItem = new CListContainerElementUI();
	pListItem->SetName(_T("orderTicketListItem"));
	pListItem->SetChildVAlign(DT_VCENTER);
	pListItem->SetFixedHeight(20);
	pListItem->SetUserData(passenger->GetCardNo());

	m_pOrderListView->Add(pListItem);


	///ϯ��
	pListItem->Add(CreateSeatTypeCombo(m_pTicket));

	///Ʊ��
	pListItem->Add(CreateTicketTypeCombo());

	///��
	CLabelUI *txtName = new CLabelUI();
	txtName->SetAttribute(_T("align") , _T("center"));
	txtName->SetText(passenger->GetName());
	txtName->SetUserData(passenger->GetType());
	pListItem->Add(txtName);

	///֤������
	CLabelUI *txtCardType = new CLabelUI();
	txtCardType->SetAttribute(_T("align"), _T("center"));
	txtCardType->SetText(passenger->GetCardTypeName());
	txtCardType->SetUserData(passenger->GetCardType());
	pListItem->Add(txtCardType);

	///֤������
	CLabelUI *txtCardNo = new CLabelUI();
	txtCardNo->SetAttribute(_T("align"), _T("center"));
	txtCardNo->SetText(passenger->GetCardNo());
	pListItem->Add(txtCardNo);

	///�ֻ�����
	CLabelUI *txtPhoneNo = new CLabelUI();
	txtPhoneNo->SetAttribute(_T("align"), _T("center"));
	txtPhoneNo->SetText(passenger->GetPhoneNo());
	pListItem->Add(txtPhoneNo);


	///ɾ����ť
	CButtonUI *btnDelete = new CButtonUI();
	btnDelete->SetManager(&m_pm , NULL , false);
	btnDelete->SetName(_T("BTN_DELETE_ORDER"));
	btnDelete->SetAttribute(_T("style"), _T("btn_style"));
	btnDelete->SetText(_T("ɾ��"));
	
	pListItem->Add(btnDelete);



}

CComboUI* COrderTicketWnd::CreateSeatTypeCombo(CTicketModel *ticket)
{
	CComboUI *pElement = new CComboUI();

	pElement->SetManager(&m_pm, NULL, false);
	pElement->SetAttribute(_T("style"), _T("combo_style"));
	CListLabelElementUI *pOpt = NULL;
	
	if (ticket->GetGrNum().Compare(_T("--")) && ticket->GetGrNum().Compare(_T("��")))
	{///�߼�����
		pOpt = new CListLabelElementUI();
		pOpt->SetText(_T("�߼�����"));
		pOpt->SetUserData(_SEAT_TYPE_GJRW);
		pElement->Add(pOpt);
	}

	if (ticket->GetQtNum().Compare(_T("--")) && ticket->GetQtNum().Compare(_T("��")))
	{///����
		pOpt = new CListLabelElementUI();
		pOpt->SetText(_T("����"));
		pOpt->SetUserData(_T("qt"));
		pElement->Add(pOpt);
	}

	if (ticket->GetRwNum().Compare(_T("--")) && ticket->GetRwNum().Compare(_T("��")))
	{///����
		pOpt = new CListLabelElementUI();
		pOpt->SetText(_T("����"));
		pOpt->SetUserData(_SEAT_TYPE_RW);
		pElement->Add(pOpt);
	}

	if (ticket->GetRzNum().Compare(_T("--")) && ticket->GetRzNum().Compare(_T("��")))
	{///����
		pOpt = new CListLabelElementUI();
		pOpt->SetText(_T("����"));
		pOpt->SetUserData(_SEAT_TYPE_RZ);
		pElement->Add(pOpt);
	}

	if (ticket->GetTzNum().Compare(_T("--")) && ticket->GetTzNum().Compare(_T("��")))
	{///�ص���
		pOpt = new CListLabelElementUI();
		pOpt->SetText(_T("�ص���"));
		pOpt->SetUserData(_SEAT_TYPE_TDZ);
		pElement->Add(pOpt);
	}

	if (ticket->GetWzNum().Compare(_T("--")) && ticket->GetWzNum().Compare(_T("��")))
	{///����
		pOpt = new CListLabelElementUI();
		pOpt->SetText(_T("����"));
		pOpt->SetUserData(_SEAT_TYPE_YZ);
		pElement->Add(pOpt);
	}

	if (ticket->GetYwNum().Compare(_T("--")) && ticket->GetYwNum().Compare(_T("��")))
	{///Ӳ��
		pOpt = new CListLabelElementUI();
		pOpt->SetText(_T("Ӳ��"));
		pOpt->SetUserData(_SEAT_TYPE_YW);
		pElement->Add(pOpt);
	}

	if (ticket->GetYzNum().Compare(_T("--")) && ticket->GetYzNum().Compare(_T("��")))
	{///Ӳ��
		pOpt = new CListLabelElementUI();
		pOpt->SetText(_T("Ӳ��"));
		pOpt->SetUserData(_SEAT_TYPE_YZ);
		pElement->Add(pOpt);
	}

	if (ticket->GetZeNum().Compare(_T("--")) && ticket->GetZeNum().Compare(_T("��")))
	{///������
		pOpt = new CListLabelElementUI();
		pOpt->SetText(_T("������"));
		pOpt->SetUserData(_SEAT_TYPE_EDZ);
		pElement->Add(pOpt);
	}


	if (ticket->GetZyNum().Compare(_T("--")) && ticket->GetZyNum().Compare(_T("��")))
	{///һ����
		pOpt = new CListLabelElementUI();
		pOpt->SetText(_T("һ����"));
		pOpt->SetUserData(_SEAT_TYPE_YDZ);
		pElement->Add(pOpt);
	}

	if (ticket->GetSwzNum().Compare(_T("--")) && ticket->GetSwzNum().Compare(_T("��")))
	{///������
		pOpt = new CListLabelElementUI();
		pOpt->SetText(_T("������"));
		pOpt->SetUserData(_SEAT_TYPE_SWZ);
		pElement->Add(pOpt);
	}

	if (pElement->GetCount() > 0)
		pElement->SelectItem(0);

	return pElement;
}


CComboUI* COrderTicketWnd::CreateTicketTypeCombo()
{
	CComboUI *pElement = new CComboUI();

	pElement->SetManager(&m_pm, NULL, false);
	pElement->SetAttribute(_T("style"), _T("combo_style"));
	CListLabelElementUI *pOpt = NULL;


	pOpt = new CListLabelElementUI();
	pOpt->SetText(_T("����"));
	pOpt->SetUserData(_TICKET_TYPE_CRP);
	pElement->Add(pOpt);

	pOpt = new CListLabelElementUI();
	pOpt->SetText(_T("��ͯƱ"));
	pOpt->SetUserData(_TICKET_TYPE_ETP);
	pElement->Add(pOpt);

	pOpt = new CListLabelElementUI();
	pOpt->SetText(_T("ѧ��Ʊ"));
	pOpt->SetUserData(_TICKET_TYPE_XSP);
	pElement->Add(pOpt);

	pOpt = new CListLabelElementUI();
	pOpt->SetText(_T("�о�Ʊ"));
	pOpt->SetUserData(_TICKET_TYPE_CJP);

	pElement->Add(pOpt);
	
	pElement->SelectItem(0);

	return pElement;
}

void COrderTicketWnd::OnDeleteOrderTicket(TNotifyUI& msg)
{
	CButtonUI *pBtn = static_cast<CButtonUI*>(msg.pSender);

	int index = m_pOrderListView->GetItemIndex(pBtn->GetParent());

	m_pOrderListView->RemoveAt(index);


}

void COrderTicketWnd::RefreshTicketSeatInfo()
{

	CHorizontalLayoutUI* pSeatInfoHLayout = static_cast<CHorizontalLayoutUI*>(m_pm.FindControl(_T("seatInfo")));

	CLabelUI *pTxtSeat = NULL;

	if (m_pTicket->GetGrNum().Compare(_T("--")) )
	{///�߼�����
		pTxtSeat = new CLabelUI();
		pTxtSeat->SetManager(&m_pm, NULL, false);
		pTxtSeat->SetFixedWidth(150);

		if (m_pTicket->GetGrNum().Compare(_T("��")))
		{
			pTxtSeat->SetFont(1);
			pTxtSeat->SetText(CDuiString(_T("�߼����� ")) + m_pTicket->GetGrNum() + _T("Ʊ"));
		}
		else
		{
			pTxtSeat->SetText(CDuiString(_T("�߼����� ")) + m_pTicket->GetGrNum() + _T("Ʊ"));

		}
		
		pSeatInfoHLayout->Add(pTxtSeat);
		
	}

	if (m_pTicket->GetQtNum().Compare(_T("--")) )
	{///����

		pTxtSeat = new CLabelUI();
		pTxtSeat->SetManager(&m_pm, NULL, false);
		pTxtSeat->SetFixedWidth(150);

		if (m_pTicket->GetQtNum().Compare(_T("��")))
		{
			pTxtSeat->SetFont(1);
			pTxtSeat->SetText(CDuiString(_T("���� ")) + m_pTicket->GetQtNum() + _T("Ʊ"));
		}
		else
		{
			pTxtSeat->SetText(CDuiString(_T("���� ")) + m_pTicket->GetQtNum() + _T("Ʊ"));

		}

		pSeatInfoHLayout->Add(pTxtSeat);

	}

	if (m_pTicket->GetRwNum().Compare(_T("--")) )
	{///����

		pTxtSeat = new CLabelUI();
		pTxtSeat->SetManager(&m_pm, NULL, false);
		pTxtSeat->SetFixedWidth(150);

		if (m_pTicket->GetRwNum().Compare(_T("��")))
		{
			pTxtSeat->SetFont(1);
			pTxtSeat->SetText(CDuiString(_T("���� ")) + m_pTicket->GetRwNum() + _T("Ʊ"));
		}
		else
		{
			pTxtSeat->SetText(CDuiString(_T("���� ")) + m_pTicket->GetRwNum() + _T("Ʊ"));

		}

		pSeatInfoHLayout->Add(pTxtSeat);

	}

	if (m_pTicket->GetRzNum().Compare(_T("--")))
	{///����

		pTxtSeat = new CLabelUI();
		pTxtSeat->SetManager(&m_pm, NULL, false);
		pTxtSeat->SetFixedWidth(150);

		if (m_pTicket->GetRzNum().Compare(_T("��")))
		{
			pTxtSeat->SetFont(1);
			pTxtSeat->SetText(CDuiString(_T("���� ")) + m_pTicket->GetRzNum() + _T("Ʊ"));
		}
		else
		{
			pTxtSeat->SetText(CDuiString(_T("���� ")) + m_pTicket->GetRzNum() + _T("Ʊ"));

		}

		pSeatInfoHLayout->Add(pTxtSeat);
	}

	if (m_pTicket->GetTzNum().Compare(_T("--")))
	{///�ص���

		pTxtSeat = new CLabelUI();
		pTxtSeat->SetManager(&m_pm, NULL, false);
		pTxtSeat->SetFixedWidth(150);

		if (m_pTicket->GetTzNum().Compare(_T("��")))
		{
			pTxtSeat->SetFont(1);
			pTxtSeat->SetText(CDuiString(_T("�ص��� ")) + m_pTicket->GetTzNum() + _T("Ʊ"));
		}
		else
		{
			pTxtSeat->SetText(CDuiString(_T("�ص��� ")) + m_pTicket->GetTzNum() + _T("Ʊ"));

		}

		pSeatInfoHLayout->Add(pTxtSeat);
	}

	if (m_pTicket->GetWzNum().Compare(_T("--")) )
	{///����

		pTxtSeat = new CLabelUI();
		pTxtSeat->SetManager(&m_pm, NULL, false);
		pTxtSeat->SetFixedWidth(150);

		if (m_pTicket->GetWzNum().Compare(_T("��")))
		{
			pTxtSeat->SetFont(1);
			pTxtSeat->SetText(CDuiString(_T("���� ")) + m_pTicket->GetWzNum() + _T("Ʊ"));
		}
		else
		{
			pTxtSeat->SetText(CDuiString(_T("���� ")) + m_pTicket->GetWzNum() + _T("Ʊ"));

		}

		pSeatInfoHLayout->Add(pTxtSeat);
	}

	if (m_pTicket->GetYwNum().Compare(_T("--")) )
	{///Ӳ��

		pTxtSeat = new CLabelUI();
		pTxtSeat->SetManager(&m_pm, NULL, false);
		pTxtSeat->SetFixedWidth(150);

		if (m_pTicket->GetYwNum().Compare(_T("��")))
		{
			pTxtSeat->SetFont(1);
			pTxtSeat->SetText(CDuiString(_T("Ӳ�� ")) + m_pTicket->GetYwNum() + _T("Ʊ"));
		}
		else
		{
			pTxtSeat->SetText(CDuiString(_T("Ӳ�� ")) + m_pTicket->GetYwNum() + _T("Ʊ"));

		}

		pSeatInfoHLayout->Add(pTxtSeat);
	}

	if (m_pTicket->GetYzNum().Compare(_T("--")) )
	{///Ӳ��

		pTxtSeat = new CLabelUI();
		pTxtSeat->SetManager(&m_pm, NULL, false);
		pTxtSeat->SetFixedWidth(150);

		if (m_pTicket->GetYzNum().Compare(_T("��")))
		{
			pTxtSeat->SetFont(1);
			pTxtSeat->SetText(CDuiString(_T("Ӳ�� ")) + m_pTicket->GetYzNum() + _T("Ʊ"));
		}
		else
		{
			pTxtSeat->SetText(CDuiString(_T("Ӳ�� ")) + m_pTicket->GetYzNum() + _T("Ʊ"));

		}

		pSeatInfoHLayout->Add(pTxtSeat);
	}

	if (m_pTicket->GetZeNum().Compare(_T("--")))
	{///������

		pTxtSeat = new CLabelUI();
		pTxtSeat->SetManager(&m_pm, NULL, false);
		pTxtSeat->SetFixedWidth(150);

		if (m_pTicket->GetZeNum().Compare(_T("��")))
		{
			pTxtSeat->SetFont(1);
			pTxtSeat->SetText(CDuiString(_T("������ ")) + m_pTicket->GetZeNum() + _T("Ʊ"));
		}
		else
		{
			pTxtSeat->SetText(CDuiString(_T("������ ")) + m_pTicket->GetZeNum() + _T("Ʊ"));

		}

		pSeatInfoHLayout->Add(pTxtSeat);
	}


	if (m_pTicket->GetZyNum().Compare(_T("--")) )
	{///һ����

		pTxtSeat = new CLabelUI();
		pTxtSeat->SetManager(&m_pm, NULL, false);
		pTxtSeat->SetFixedWidth(150);

		if (m_pTicket->GetZyNum().Compare(_T("��")))
		{
			pTxtSeat->SetFont(1);
			pTxtSeat->SetText(CDuiString(_T("һ���� ")) + m_pTicket->GetZyNum() + _T("Ʊ"));
		}
		else
		{
			pTxtSeat->SetText(CDuiString(_T("һ���� ")) + m_pTicket->GetZyNum() + _T("Ʊ"));

		}

		pSeatInfoHLayout->Add(pTxtSeat);
	}

	if (m_pTicket->GetSwzNum().Compare(_T("--")) )
	{///������

		pTxtSeat = new CLabelUI();
		pTxtSeat->SetManager(&m_pm, NULL, false);
		pTxtSeat->SetFixedWidth(150);

		if (m_pTicket->GetSwzNum().Compare(_T("��")))
		{
			pTxtSeat->SetFont(1);
			pTxtSeat->SetText(CDuiString(_T("������ ")) + m_pTicket->GetSwzNum() + _T("Ʊ"));
		}
		else
		{
			pTxtSeat->SetText(CDuiString(_T("������ ")) + m_pTicket->GetSwzNum() + _T("Ʊ"));

		}

		pSeatInfoHLayout->Add(pTxtSeat);
	}


	
}


void COrderTicketWnd::OnTicketOrderSubmit(TNotifyUI& msg)
{
	int iCnt = m_pOrderListView->GetCount();

	std::vector<CPassengerTicket> vecPT;

	for (int i = 0; i < iCnt; ++i)
	{
		CPassengerTicket pt;

		CListContainerElementUI *pEleContainer = static_cast<CListContainerElementUI*>(m_pOrderListView->GetItemAt(i));

		///
		CComboUI* pSeatCombo = static_cast<CComboUI*>(pEleContainer->GetItemAt(0));

		///ϯ��
		pt.SetSeatType(pSeatCombo->GetItemAt(pSeatCombo->GetCurSel())->GetUserData());


		///Ʊ��
		CComboUI* pTicketCombo = static_cast<CComboUI*>(pEleContainer->GetItemAt(1));
		pt.SetTicketType(pTicketCombo->GetItemAt(pTicketCombo->GetCurSel())->GetUserData());

		///����
		CLabelUI *pTxtName = static_cast<CLabelUI*>(pEleContainer->GetItemAt(2));
		pt.SetPassengerName(pTxtName->GetText());
		pt.SetPassengerType(pTxtName->GetUserData());

		///֤������
		CLabelUI *pTxtIdType = static_cast<CLabelUI*>(pEleContainer->GetItemAt(3));
		pt.SetIdType(pTxtIdType->GetUserData());

		///֤������
		CLabelUI *pTxtIdNo = static_cast<CLabelUI*>(pEleContainer->GetItemAt(4));
		pt.SetIdNo(pTxtIdNo->GetText());

		///�ֻ�����
		CLabelUI *pMobileNo = static_cast<CLabelUI*>(pEleContainer->GetItemAt(5));
		pt.SetMobileNo(pMobileNo->GetText());

		pt.SetSaveStatus(_T("N"));




		vecPT.push_back(pt);
	}

	_ERRNO err = E_FAILURE;
	do
	{
		CCheckOrderInfoResult result;
		if (E_OK != Client12306Manager::Instance()->CheckOrderInfo(vecPT, result))
		{
			CMsgWnd::MessageBox(GetHWND(), _T("��ʾ"), Utf8ToUnicode(Client12306Manager::Instance()->GetLastErrInfo()).c_str());
			break;
		}

		if (result.GetSubmitStatus() != "true" && result.GetSubmitStatus() != "TRUE")
		{
			CMsgWnd::MessageBox(GetHWND(), _T("��ʾ"), _T("��Ʊʧ��"));

			break;

		}

		char randCode[128] = { 0 };
		if (result.GetIfShowPassCode() == "Y")
		{///��Ҫ��ʾ��֤��

			std::vector<CDuiPoint> selPoint;
			if (MSGID_CANCEL == CCertCodeWnd::MessageBox(GetHWND(), _T("��������֤��"), selPoint))
				break;


			///
			if (selPoint.empty())
			{
				CMsgWnd::MessageBox(GetHWND(), _T("��ʾ"), _T("��֤��δѡ��"));

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
		if (E_OK != Client12306Manager::Instance()->getQueueCount(m_pTicket, m_strToken, m_strLeftTicketString, UnicodeToUtf8(vecPT[0].GetSeatType().GetData()), queRes))
		{
			CMsgWnd::MessageBox(GetHWND(), _T("��ʾ"), Utf8ToUnicode(Client12306Manager::Instance()->GetLastErrInfo()).c_str());
			break;
		}

		if (queRes.GetOp2() == "true")
		{
			CMsgWnd::MessageBox(GetHWND(), _T("��ʾ"), _T("Ŀǰ�Ŷ������Ѿ�������Ʊ����������ѡ������ϯ��򳵴�"));
			break;
		}
		else
		{
			if (atoi(queRes.GetCountT().c_str()) > 0)
			{
				CDuiString tmp;
				tmp.Format(_T("Ŀǰ�Ŷ����� %s ��"), Utf8ToUnicode(queRes.GetCountT()).c_str());
				CMsgWnd::MessageBox(GetHWND(), _T("��ʾ"), tmp);
			}

		}


		/////
		if (E_OK != Client12306Manager::Instance()->ConfirmSingleForQueue(vecPT, m_pTicket, m_strToken, m_strLeftTicketString, m_strKeyCheckIsChange, randCode))
		{
			CMsgWnd::MessageBox(GetHWND(), _T("��ʾ"), Utf8ToUnicode(Client12306Manager::Instance()->GetLastErrInfo()).c_str());
			break;
		}


		COrderWaitTimeWnd *pWaitTimeWnd = COrderWaitTimeWnd::GetInstance(GetHWND(), _T("�Ե�Ƭ��"), m_strToken);

		std::auto_ptr<COrderWaitTimeWnd> ptrWnd(pWaitTimeWnd);

		if (E_OK != (err = (_ERRNO)ptrWnd->ShowModal()))
		{
			CMsgWnd::MessageBox(GetHWND(), _T("��ʾ"), ptrWnd->GetErrString());
			break;
		}


		err = E_OK;




	} while (false);

	switch(err)
	{
		case E_OK:
			CMsgWnd::MessageBox(GetHWND(), _T("��ʾ"), _T("��ϲ������Ʊ�ɹ���"));
			break;
		default:
			CMsgWnd::MessageBox(GetHWND(), _T("��ʾ"), _T("��Ʊʧ�ܣ���Ҫ�ѹ�����ͷ����"));
			break;
	}

	Close();

}