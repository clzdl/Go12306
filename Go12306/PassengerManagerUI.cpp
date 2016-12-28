#include "stdafx.h"
#include "PassengerManagerUI.h"
#include "MainFrame.h"
#include "OthFunc.h"

CPassengerManagerUI::CPassengerManagerUI(CMainFrame *mainFrame)
	:m_mainFrame(mainFrame)
{
	m_pPassengerListView = static_cast<CListUI*>(m_mainFrame->GetPaintManagerUI()->FindControl(_T("passengerListView")));
}
CPassengerManagerUI::~CPassengerManagerUI()
{

}


_ERRNO CPassengerManagerUI::RefreshPassengerListView(std::map<std::string, CPassenger> &objMap)
{
	m_pPassengerListView->RemoveAll();

	for (std::map<std::string, CPassenger>::iterator it = objMap.begin(); it != objMap.end(); ++it)
	{
		CPassenger &passenger = it->second;

		///Ìí¼ÓÐÐ
		CListContainerElementUI* pListItem = new CListContainerElementUI();
		pListItem->SetChildVAlign(DT_VCENTER);
		pListItem->SetFixedHeight(20);

		m_pPassengerListView->Add(pListItem);

	
		///
		CLabelUI *pElement = new CLabelUI();
		pElement->SetText( passenger.GetName() );
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

	return E_OK;
}