#include "stdafx.h"
#include "OrderTicketWnd.h"
#include "Client12306Manager.h"
#include "OthFunc.h"

DUI_BEGIN_MESSAGE_MAP(COrderTicketWnd, WindowImplBase)
	DUI_ON_MSGTYPE(DUI_MSGTYPE_CLICK, OnClick)
	DUI_ON_MSGTYPE_CTRNAME(DUI_MSGTYPE_ITEMACTIVATE, _T("passengerListItem"), OnPassengerListItemClick)
	DUI_ON_CLICK_CTRNAME(_T("BTN_DELETE_ORDER") , OnDeleteOrderTicket)
DUI_END_MESSAGE_MAP()


COrderTicketWnd::COrderTicketWnd(CTicketModel *ticket)
	:m_pTicket(ticket)
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

	RefreshPassenger();
}

void COrderTicketWnd::RefreshPassenger()
{
	std::map < std::string , CPassenger> &objMap =  Client12306Manager::Instance()->GetPassenger();

	for (std::map<std::string, CPassenger>::iterator it = objMap.begin(); it != objMap.end(); ++it)
	{
		CPassenger &passenger = it->second;

		///添加行
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
	///添加行
	CListContainerElementUI* pListItem = new CListContainerElementUI();
	pListItem->SetName(_T("orderTicketListItem"));
	pListItem->SetChildVAlign(DT_VCENTER);
	pListItem->SetFixedHeight(20);
	pListItem->SetUserData(passenger->GetCardNo());

	m_pOrderListView->Add(pListItem);


	///席别
	pListItem->Add(CreateSeatTypeCombo(m_pTicket));

	///票种
	pListItem->Add(CreateTicketTypeCombo());

	///姓
	CLabelUI *txtName = new CLabelUI();
	txtName->SetAttribute(_T("align") , _T("center"));
	txtName->SetText(passenger->GetName());
	pListItem->Add(txtName);

	///证件类型
	CLabelUI *txtCardType = new CLabelUI();
	txtCardType->SetAttribute(_T("align"), _T("center"));
	txtCardType->SetText(passenger->GetCardType());
	pListItem->Add(txtCardType);

	///证件号码
	CLabelUI *txtCardNo = new CLabelUI();
	txtCardNo->SetAttribute(_T("align"), _T("center"));
	txtCardNo->SetText(passenger->GetCardNo());
	pListItem->Add(txtCardNo);

	///手机号码
	CLabelUI *txtPhoneNo = new CLabelUI();
	txtPhoneNo->SetAttribute(_T("align"), _T("center"));
	txtPhoneNo->SetText(passenger->GetPhoneNo());
	pListItem->Add(txtPhoneNo);


	///删除按钮
	CButtonUI *btnDelete = new CButtonUI();
	btnDelete->SetManager(&m_pm , NULL , false);
	btnDelete->SetName(_T("BTN_DELETE_ORDER"));
	btnDelete->SetAttribute(_T("style"), _T("btn_style"));
	btnDelete->SetText(_T("删除"));
	
	pListItem->Add(btnDelete);



}

CComboUI* COrderTicketWnd::CreateSeatTypeCombo(CTicketModel *ticket)
{
	CComboUI *pElement = new CComboUI();

	pElement->SetManager(&m_pm, NULL, false);
	pElement->SetAttribute(_T("style"), _T("combo_style"));
	CListLabelElementUI *pOpt = NULL;
	
	if (ticket->GetGrNum().Compare(_T("--")) && ticket->GetGrNum().Compare(_T("无")))
	{///高级软卧
		pOpt = new CListLabelElementUI();
		pOpt->SetText(_T("高级软卧"));
		pOpt->SetUserData(_T("gr"));
		pElement->Add(pOpt);
	}

	if (ticket->GetQtNum().Compare(_T("--")) && ticket->GetQtNum().Compare(_T("无")))
	{///其它
		pOpt = new CListLabelElementUI();
		pOpt->SetText(_T("其它"));
		pOpt->SetUserData(_T("qt"));
		pElement->Add(pOpt);
	}

	if (ticket->GetRwNum().Compare(_T("--")) && ticket->GetRwNum().Compare(_T("无")))
	{///软卧
		pOpt = new CListLabelElementUI();
		pOpt->SetText(_T("软卧"));
		pOpt->SetUserData(_T("rw"));
		pElement->Add(pOpt);
	}

	if (ticket->GetRzNum().Compare(_T("--")) && ticket->GetRzNum().Compare(_T("无")))
	{///软座
		pOpt = new CListLabelElementUI();
		pOpt->SetText(_T("软座"));
		pOpt->SetUserData(_T("rz"));
		pElement->Add(pOpt);
	}

	if (ticket->GetTzNum().Compare(_T("--")) && ticket->GetTzNum().Compare(_T("无")))
	{///特等座
		pOpt = new CListLabelElementUI();
		pOpt->SetText(_T("特等座"));
		pOpt->SetUserData(_T("tz"));
		pElement->Add(pOpt);
	}

	if (ticket->GetWzNum().Compare(_T("--")) && ticket->GetWzNum().Compare(_T("无")))
	{///无座
		pOpt = new CListLabelElementUI();
		pOpt->SetText(_T("无座"));
		pOpt->SetUserData(_T("wz"));
		pElement->Add(pOpt);
	}

	if (ticket->GetYwNum().Compare(_T("--")) && ticket->GetYwNum().Compare(_T("无")))
	{///硬卧
		pOpt = new CListLabelElementUI();
		pOpt->SetText(_T("硬卧"));
		pOpt->SetUserData(_T("yw"));
		pElement->Add(pOpt);
	}

	if (ticket->GetYzNum().Compare(_T("--")) && ticket->GetYzNum().Compare(_T("无")))
	{///硬座
		pOpt = new CListLabelElementUI();
		pOpt->SetText(_T("硬座"));
		pOpt->SetUserData(_T("yz"));
		pElement->Add(pOpt);
	}

	if (ticket->GetZeNum().Compare(_T("--")) && ticket->GetZeNum().Compare(_T("无")))
	{///二等座
		pOpt = new CListLabelElementUI();
		pOpt->SetText(_T("二等座"));
		pOpt->SetUserData(_T("ze"));
		pElement->Add(pOpt);
	}


	if (ticket->GetZyNum().Compare(_T("--")) && ticket->GetZyNum().Compare(_T("无")))
	{///一等座
		pOpt = new CListLabelElementUI();
		pOpt->SetText(_T("一等座"));
		pOpt->SetUserData(_T("zy"));
		pElement->Add(pOpt);
	}

	if (ticket->GetSwzNum().Compare(_T("--")) && ticket->GetSwzNum().Compare(_T("无")))
	{///商务座
		pOpt = new CListLabelElementUI();
		pOpt->SetText(_T("商务座"));
		pOpt->SetUserData(_T("swz"));
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
	pOpt->SetText(_T("成人"));
	pOpt->SetUserData(_T("0"));
	pElement->Add(pOpt);

	pOpt = new CListLabelElementUI();
	pOpt->SetText(_T("儿童票"));
	pOpt->SetUserData(_T("1"));
	pElement->Add(pOpt);

	pOpt = new CListLabelElementUI();
	pOpt->SetText(_T("学生票"));
	pOpt->SetUserData(_T("2"));
	pElement->Add(pOpt);

	pOpt = new CListLabelElementUI();
	pOpt->SetText(_T("残军票"));
	pOpt->SetUserData(_T("3"));

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