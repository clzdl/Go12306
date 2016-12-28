#include "stdafx.h"
#include "OrderManagerUI.h"
#include "MainFrame.h"
#include "OrderModel.h"


COrderManagerUI::COrderManagerUI(CMainFrame *mainFrame)
	:m_mainFrame(mainFrame)
{
		m_pOderListView = static_cast<CListUI*>(m_mainFrame->GetPaintManagerUI()->FindControl(_T("orderListView")));
}


_ERRNO COrderManagerUI::RefreshOrderListView(std::map<string, COrderModel> &objMap)
{
	m_pOderListView->RemoveAll();

	
	{
		int i = 0;
		for (std::map<string, COrderModel>::iterator it = objMap.begin(); it != objMap.end(); ++it,++i)
		{
			CDuiString  btnChkName;

			COrderModel &orderModel = it->second;

			btnChkName.Format(_T("ORDER_TICKET_%d"), i);

			///�����
			CListContainerElementUI* pListItem = new CListContainerElementUI();
			pListItem->SetChildVAlign(DT_VCENTER);
			pListItem->SetFixedHeight(20);
		

			m_pOderListView->Add(pListItem);

			///
			CVerticalLayoutUI* tmpVLayout = new CVerticalLayoutUI();

			tmpVLayout->SetAttribute(_T("bordersize"), _T("1,1,1,1"));
			tmpVLayout->SetAttribute(_T("bordercolor"), _T("#F"));
			tmpVLayout->SetAttribute(_T("inset"), _T("2,2,2,2"));
			pListItem->Add(tmpVLayout);

			{
				////�б�ͷ
				tmpVLayout->Add(CreateDetailListHeader(btnChkName , orderModel));

				CLabelUI *txtSpeLine = new CLabelUI();
				txtSpeLine->SetAttribute(_T("height"), _T("2"));
				txtSpeLine->SetAttribute(_T("bkimage"), _T("common/hor_line.png"));
				tmpVLayout->Add(txtSpeLine);

				////�б��
				tmpVLayout->Add(CreateTicketList(btnChkName,orderModel));

			}
		}
	}

	//m_pOderListView->SetManager(m_mainFrame->GetPaintManagerUI(), NULL, false);
	return E_OK;
}

_ERRNO COrderManagerUI::RefreshOrderDetailList(CListUI *dListUI , bool check)
{
	if (check)
	{
		dListUI->SetVisible(true);
		dListUI->GetParent()->GetParent()->SetFixedHeight(120);
	}
	else
	{
		dListUI->SetVisible(false);
		dListUI->GetParent()->GetParent()->SetFixedHeight(20);

	}

	return E_OK;
}


CHorizontalLayoutUI* COrderManagerUI::CreateDetailListHeader(CDuiString lstTicketListName ,COrderModel &orderModel)
{
	CHorizontalLayoutUI* tmpHLayout = new CHorizontalLayoutUI();
	tmpHLayout->SetAttribute(_T("height"), _T("20"));
	tmpHLayout->SetAttribute(_T("bkcolor"), _T("#FFB4EEB4"));
	tmpHLayout->SetAttribute(_T("inset"), _T("2,2,2,2"));

	{
		///չ����ť
		CCheckBoxUI *checkBox = new CCheckBoxUI();

		checkBox->SetAttribute(_T("width"), _T("20"));
		checkBox->SetAttribute(_T("normalimage"), _T("file='common/scrollbar.bmp' source='0,90,15,105' dest='0,0,20,20'"));
		checkBox->SetAttribute(_T("selectedimage"), _T("file='common/scrollbar.bmp' source='0,109,15,124' dest='0,0,20,20'"));
		checkBox->SetUserData(lstTicketListName);
		checkBox->SetName(_T("ORDER_EXPAND_BTN"));

		tmpHLayout->Add(checkBox);

		/////�������ڱ�ǩ
		CLabelUI *txtOrderDate = new CLabelUI();
		txtOrderDate->SetText(_T("��������:"));
		txtOrderDate->SetAttribute(_T("width"), _T("80"));
		txtOrderDate->SetAttribute(_T("align"), _T("left"));
		tmpHLayout->Add(txtOrderDate);

		/////������������
		CLabelUI *txtOrderDateCont = new CLabelUI();
		txtOrderDateCont->SetText(orderModel.GetOrderDate());
		txtOrderDateCont->SetAttribute(_T("width"), _T("100"));
		txtOrderDateCont->SetAttribute(_T("align"), _T("left"));
		txtOrderDateCont->SetAttribute(_T("font"), _T("2"));
		tmpHLayout->Add(txtOrderDateCont);


		/////�������
		CLabelUI *txtOrderNo = new CLabelUI();
		txtOrderNo->SetText(_T("�������:"));
		txtOrderNo->SetAttribute(_T("width"), _T("80"));
		txtOrderNo->SetAttribute(_T("align"), _T("left"));
		tmpHLayout->Add(txtOrderNo);

		/////�����������
		CLabelUI *txtOrderNoCont = new CLabelUI();
		txtOrderNoCont->SetText(orderModel.GetOrderNo());
		txtOrderNoCont->SetAttribute(_T("width"), _T("180"));
		txtOrderNoCont->SetAttribute(_T("align"), _T("left"));
		txtOrderNoCont->SetAttribute(_T("font"), _T("2"));
		tmpHLayout->Add(txtOrderNoCont);


		/////��Ʊ��
		CLabelUI *txtOrderMen = new CLabelUI();
		txtOrderMen->SetText(orderModel.GetPassengerName());
		txtOrderMen->SetAttribute(_T("width"), _T("150"));
		txtOrderMen->SetAttribute(_T("align"), _T("left"));
		txtOrderMen->SetAttribute(_T("font"), _T("2"));
		tmpHLayout->Add(txtOrderMen);

		/////ʼ��վ
		CLabelUI *txtBegStation = new CLabelUI();
		txtBegStation->SetText(orderModel.GetFromStation());
		txtBegStation->SetAttribute(_T("width"), _T("60"));
		txtBegStation->SetAttribute(_T("align"), _T("left"));
		txtBegStation->SetAttribute(_T("font"), _T("2"));
		tmpHLayout->Add(txtBegStation);


		/////��ͷ
		CLabelUI *txtArrow = new CLabelUI();
		txtArrow->SetText(_T("=>"));
		txtArrow->SetAttribute(_T("width"), _T("30"));
		txtArrow->SetAttribute(_T("align"), _T("center"));
		txtBegStation->SetAttribute(_T("font"), _T("2"));
		tmpHLayout->Add(txtArrow);


		/////����վ
		CLabelUI *txtEndStation = new CLabelUI();
		txtEndStation->SetText(orderModel.GetToStateion());
		txtEndStation->SetAttribute(_T("width"), _T("60"));
		txtEndStation->SetAttribute(_T("align"), _T("left"));
		txtEndStation->SetAttribute(_T("font"), _T("2"));
		tmpHLayout->Add(txtEndStation);

		/////�˳�����
		CLabelUI *txtTravelDate = new CLabelUI();
		txtTravelDate->SetText(_T("�˳�����:"));
		txtTravelDate->SetAttribute(_T("width"), _T("100"));
		txtTravelDate->SetAttribute(_T("align"), _T("right"));
		txtTravelDate->SetAttribute(_T("font"), _T("2"));
		tmpHLayout->Add(txtTravelDate);

		/////�˳���������
		CLabelUI *txtTravelDateCont = new CLabelUI();
		txtTravelDateCont->SetText(orderModel.GetTravelDate());
		txtTravelDateCont->SetAttribute(_T("width"), _T("100"));
		txtTravelDateCont->SetAttribute(_T("align"), _T("left"));
		txtTravelDateCont->SetAttribute(_T("font"), _T("2"));
		tmpHLayout->Add(txtTravelDateCont);
	}

	return tmpHLayout;
}

CListUI* COrderManagerUI::CreateTicketList(CDuiString lstTicketListName, COrderModel &orderModel)
{
	////
	CListUI *lstTickUI = new CListUI();

	lstTickUI->SetManager(m_mainFrame->GetPaintManagerUI(), NULL, false);

	lstTickUI->SetAttribute(_T("vscrollbar"), _T("true"));

	lstTickUI->SetAttribute(_T("hscrollbar"), _T("true"));

	lstTickUI->SetAttribute(_T("itemvalign"), _T("vcenter"));

	lstTickUI->SetAttribute(_T("itemshowrowline"), _T("true"));

	lstTickUI->SetAttribute(_T("itemlinecolor"), _T("0x00FFFFFF"));


	lstTickUI->SetName(lstTicketListName);
	
	lstTickUI->SetVisible(false);
	
	lstTickUI->Add(CreateTicketListHeader());

	std::vector<COrderTicketModel>& vecOrderTicket = orderModel.GetOrderTicket();

	for (std::vector<COrderTicketModel>::iterator it = vecOrderTicket.begin(); it != vecOrderTicket.end(); ++it)
	{

		lstTickUI->Add(CreateListContainerEleUI(orderModel , *it));
	}

	return lstTickUI;
}


CListHeaderUI* COrderManagerUI::CreateTicketListHeader()
{
	CListHeaderUI  *headUI = new CListHeaderUI();
	headUI->SetAttribute(_T("height"), _T("20"));

	{
		CListHeaderItemUI *headerItemUI = new CListHeaderItemUI();
		headerItemUI->SetText(_T("������Ϣ"));
		headerItemUI->SetFont(2);
		headerItemUI->SetFixedWidth(150);
		headerItemUI->SetHotImage(_T("res='common/list_header_hot.png'"));
		headerItemUI->SetPushedImage(_T("res='common/list_header_pushed.png'"));
		headerItemUI->SetSepImage(_T("res='common/list_header_sep.png'"));
		headerItemUI->SetSepWidth(1);
		headerItemUI->SetAttribute(_T("align"), _T("center"));
		headUI->Add(headerItemUI);


		headerItemUI = new CListHeaderItemUI();
		headerItemUI->SetFont(2);
		headerItemUI->SetText(_T("ϯλ��Ϣ"));
		headerItemUI->SetFixedWidth(150);
		headerItemUI->SetHotImage(_T("res='common/list_header_hot.png'"));
		headerItemUI->SetPushedImage(_T("res='common/list_header_pushed.png'"));
		headerItemUI->SetSepImage(_T("res='common/list_header_sep.png'"));
		headerItemUI->SetSepWidth(1);
		headerItemUI->SetAttribute(_T("align"), _T("center"));
		headUI->Add(headerItemUI);

		headerItemUI = new CListHeaderItemUI();
		headerItemUI->SetFont(2);
		headerItemUI->SetText(_T("�ÿ���Ϣ"));
		headerItemUI->SetFixedWidth(150);
		headerItemUI->SetHotImage(_T("res='common/list_header_hot.png'"));
		headerItemUI->SetPushedImage(_T("res='common/list_header_pushed.png'"));
		headerItemUI->SetSepImage(_T("res='common/list_header_sep.png'"));
		headerItemUI->SetSepWidth(1);
		headerItemUI->SetAttribute(_T("align"), _T("center"));
		headUI->Add(headerItemUI);

		headerItemUI = new CListHeaderItemUI();
		headerItemUI->SetFont(2);
		headerItemUI->SetText(_T("Ʊ����"));
		headerItemUI->SetFixedWidth(100);
		headerItemUI->SetHotImage(_T("res='common/list_header_hot.png'"));
		headerItemUI->SetPushedImage(_T("res='common/list_header_pushed.png'"));
		headerItemUI->SetSepImage(_T("res='common/list_header_sep.png'"));
		headerItemUI->SetSepWidth(1);
		headerItemUI->SetAttribute(_T("align"), _T("center"));
		headUI->Add(headerItemUI);

		headerItemUI = new CListHeaderItemUI();
		headerItemUI->SetFont(2);
		headerItemUI->SetText(_T("��Ʊ״̬"));
		headerItemUI->SetFixedWidth(100);
		headerItemUI->SetHotImage(_T("res='common/list_header_hot.png'"));
		headerItemUI->SetPushedImage(_T("res='common/list_header_pushed.png'"));
		headerItemUI->SetSepImage(_T("res='common/list_header_sep.png'"));
		headerItemUI->SetSepWidth(1);
		headerItemUI->SetAttribute(_T("align"), _T("center"));
		headUI->Add(headerItemUI);

		headerItemUI = new CListHeaderItemUI();
		headerItemUI->SetFont(2);
		headerItemUI->SetText(_T("����"));
		headerItemUI->SetFixedWidth(150);
		headerItemUI->SetHotImage(_T("res='common/list_header_hot.png'"));
		headerItemUI->SetPushedImage(_T("res='common/list_header_pushed.png'"));
		headerItemUI->SetSepImage(_T("res='common/list_header_sep.png'"));
		headerItemUI->SetSepWidth(1);
		headerItemUI->SetAttribute(_T("align"), _T("center"));
		headUI->Add(headerItemUI);
	}

	return headUI;
}

CListContainerElementUI* COrderManagerUI::CreateListContainerEleUI(COrderModel &orderModel, COrderTicketModel &orderTicketModel)
{
	CListContainerElementUI* pListItem = new CListContainerElementUI();
	pListItem->SetChildVAlign(DT_VCENTER);
	pListItem->SetFixedHeight(60);

	{////������Ϣ
		CVerticalLayoutUI *trainVLayout = new CVerticalLayoutUI();

		trainVLayout->Add(new CControlUI());

		CLabelUI *trainLeaveTime = new CLabelUI();
		trainLeaveTime->SetFont(4);
		trainLeaveTime->SetAttribute(_T("align") , _T("center"));
		trainLeaveTime->SetText(orderModel.GetTravelDate());
		
		trainVLayout->Add(trainLeaveTime);

		trainVLayout->Add(new CControlUI());

		CLabelUI *trainInfo = new CLabelUI();
		trainInfo->SetFont(4);
		trainInfo->SetTextColor(0xFF00AA00);
		//trainInfo->SetText(_T("D904 ʯ��ׯ-������"));
		///todo
		trainInfo->SetText(orderModel.GetTrainCode() + _T(" ") + orderModel.GetFromStation() +_T("-") + orderModel.GetToStateion());
		trainInfo->SetAttribute(_T("align"), _T("center"));
		trainVLayout->Add(trainInfo);

		trainVLayout->Add(new CControlUI());

		pListItem->Add(trainVLayout);
	}

	{////ϯλ��Ϣ
		CVerticalLayoutUI *seatVLayout = new CVerticalLayoutUI();

		seatVLayout->Add(new CControlUI());

		CLabelUI *trainRoomInfo = new CLabelUI();
		trainRoomInfo->SetFont(4);
		trainRoomInfo->SetText(orderTicketModel.GetCoachName());
		trainRoomInfo->SetAttribute(_T("align"), _T("center"));
		seatVLayout->Add(trainRoomInfo);

		seatVLayout->Add(new CControlUI());

		CLabelUI *seatInfo = new CLabelUI();
		seatInfo->SetFont(4);
		seatInfo->SetText(orderTicketModel.GetSeatName());
		seatInfo->SetAttribute(_T("align"), _T("center"));
		seatVLayout->Add(seatInfo);

		seatVLayout->Add(new CControlUI());

		CLabelUI *seatCategory = new CLabelUI();
		seatCategory->SetFont(1);
		seatCategory->SetText(orderTicketModel.GetSeatTypeName());
		seatCategory->SetAttribute(_T("align"), _T("center"));
		seatVLayout->Add(seatCategory);

		seatVLayout->Add(new CControlUI());

		pListItem->Add(seatVLayout);
	}


	{////�ÿ���Ϣ
		CVerticalLayoutUI *travelPersonVLayout = new CVerticalLayoutUI();

		travelPersonVLayout->Add(new CControlUI());

		CLabelUI *travelPerson = new CLabelUI();
		travelPerson->SetFont(4);
		travelPerson->SetText(orderTicketModel.GetPassengerPersonName());
		travelPerson->SetTextColor(0xFF00AA00);
		travelPerson->SetAttribute(_T("align"), _T("center"));
		travelPersonVLayout->Add(travelPerson);

		travelPersonVLayout->Add(new CControlUI());

		CLabelUI *travelCardType = new CLabelUI();
		travelCardType->SetFont(1);
		travelCardType->SetText(orderTicketModel.GetPassengerIDTypeName());

		travelCardType->SetAttribute(_T("align"), _T("center"));
		travelPersonVLayout->Add(travelCardType);

		travelPersonVLayout->Add(new CControlUI());

		pListItem->Add(travelPersonVLayout);
	}

	{////Ʊ����
		CVerticalLayoutUI *ticketFeeVLayout = new CVerticalLayoutUI();

		ticketFeeVLayout->Add(new CControlUI());

		CLabelUI *ticketType = new CLabelUI();
		ticketType->SetFont(1);
		ticketType->SetText(orderTicketModel.GetTicketTypeName());
		ticketType->SetAttribute(_T("align"), _T("center"));
		ticketFeeVLayout->Add(ticketType);

		ticketFeeVLayout->Add(new CControlUI());

		CLabelUI *ticketFee = new CLabelUI();
		ticketFee->SetFont(1);
		ticketFee->SetText(orderTicketModel.GetTicketPrice());
		ticketFee->SetAttribute(_T("align"), _T("center"));
		ticketFeeVLayout->Add(ticketFee);

		ticketFeeVLayout->Add(new CControlUI());

		pListItem->Add(ticketFeeVLayout);
	}

	{////��Ʊ״̬
		

		CLabelUI *ticketStatus = new CLabelUI();
		ticketStatus->SetFont(2);
		ticketStatus->SetText(orderTicketModel.GetTicketPayStatus());
		ticketStatus->SetTextColor(0xFF00AA00);
		ticketStatus->SetAttribute(_T("align"), _T("center"));
		pListItem->Add(ticketStatus);
	}

	{////����
		CVerticalLayoutUI *operVLayout = new CVerticalLayoutUI();

		///ռλ
		operVLayout->Add(new CControlUI());

		////��Ʊ
		CButtonUI *ticketGiveBack = new CButtonUI();
		ticketGiveBack->SetManager(m_mainFrame->GetPaintManagerUI(), NULL, false);

		ticketGiveBack->SetName(_T("BTN_TICKET_GIVE_BACK"));
		ticketGiveBack->SetAttribute(_T("style"), _T("btn_style"));
		ticketGiveBack->SetText(_T("��Ʊ"));
		ticketGiveBack->SetUserData(_T(""));
		ticketGiveBack->SetFixedHeight(25);
		operVLayout->Add(ticketGiveBack);

		///ռλ
		operVLayout->Add(new CControlUI());

		////��ǩ
		CButtonUI *ticketChange = new CButtonUI();
		ticketChange->SetManager(m_mainFrame->GetPaintManagerUI(), NULL, false);
		
		ticketGiveBack->SetName(_T("BTN_TICKET_CHANGE"));
		ticketChange->SetAttribute(_T("style"), _T("btn_style"));
		ticketChange->SetText(_T("��ǩ"));
		ticketChange->SetUserData(_T(""));
		ticketChange->SetFixedHeight(25);
		operVLayout->Add(ticketChange);

		///ռλ
		operVLayout->Add(new CControlUI());



		pListItem->Add(operVLayout);

		

		
	}


	return pListItem;
}