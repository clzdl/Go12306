#include "stdafx.h"
#include "OrderManagerUI.h"
#include "MainFrame.h"


COrderManagerUI::COrderManagerUI(CMainFrame *mainFrame)
	:m_mainFrame(mainFrame)
{
		m_pOderListView = static_cast<CListUI*>(m_mainFrame->GetPaintManagerUI()->FindControl(_T("orderListView")));
}


int COrderManagerUI::RefreshOrderListView()
{
	m_pOderListView->RemoveAll();


	{
		for (int i = 0; i < 2; ++i)
		{
			CDuiString  btnChkName;
			btnChkName.Format(_T("ORDER_TICKET_%d"), i);

			///�����
			CListContainerElementUI* pListItem = new CListContainerElementUI();
			pListItem->SetChildVAlign(DT_VCENTER);
			pListItem->SetFixedHeight(20);
			pListItem->SetManager(m_mainFrame->GetPaintManagerUI(), NULL, false);

			m_pOderListView->Add(pListItem);



			///
			CVerticalLayoutUI* tmpVLayout = new CVerticalLayoutUI();
			tmpVLayout->SetAttribute(_T("hscrollbar"), _T("true"));
			tmpVLayout->SetAttribute(_T("bordersize"), _T("1,1,1,1"));
			tmpVLayout->SetAttribute(_T("bordercolor"), _T("#F"));
			tmpVLayout->SetAttribute(_T("inset"), _T("2,2,2,2"));

			pListItem->Add(tmpVLayout);

			{
				
				tmpVLayout->Add(CreateDetailListHeader(btnChkName));

				CLabelUI *txtSpeLine = new CLabelUI();
				txtSpeLine->SetAttribute(_T("height"), _T("2"));
				txtSpeLine->SetAttribute(_T("bkimage"), _T("common/hor_line.png"));

				tmpVLayout->Add(txtSpeLine);

				
				tmpVLayout->Add(CreateTicketList(btnChkName));

			}
		}
	}

	return SUCCESS;
}

int COrderManagerUI::RefreshOrderDetailList(CListUI *dListUI , bool check)
{
	if (check)
	{
		dListUI->SetVisible(true);
		dListUI->GetParent()->GetParent()->SetFixedHeight(100);
	}
	else
	{
		dListUI->SetVisible(false);
		dListUI->GetParent()->GetParent()->SetFixedHeight(20);

	}

	return SUCCESS;
}


CHorizontalLayoutUI* COrderManagerUI::CreateDetailListHeader(CDuiString lstTicketListName)
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
		txtOrderDateCont->SetText(_T("2012-12-12"));
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
		txtOrderNoCont->SetText(_T("OX201212121631001"));
		txtOrderNoCont->SetAttribute(_T("width"), _T("180"));
		txtOrderNoCont->SetAttribute(_T("align"), _T("left"));
		txtOrderNoCont->SetAttribute(_T("font"), _T("2"));
		tmpHLayout->Add(txtOrderNoCont);


		/////��Ʊ��
		CLabelUI *txtOrderMen = new CLabelUI();
		txtOrderMen->SetText(_T("chengliang"));
		txtOrderMen->SetAttribute(_T("width"), _T("150"));
		txtOrderMen->SetAttribute(_T("align"), _T("left"));
		txtOrderMen->SetAttribute(_T("font"), _T("2"));
		tmpHLayout->Add(txtOrderMen);

		/////ʼ��վ
		CLabelUI *txtBegStation = new CLabelUI();
		txtBegStation->SetText(_T("������"));
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
		txtEndStation->SetText(_T("ʯ��ׯ"));
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
		txtTravelDateCont->SetText(_T("2016-12-12"));
		txtTravelDateCont->SetAttribute(_T("width"), _T("100"));
		txtTravelDateCont->SetAttribute(_T("align"), _T("left"));
		txtTravelDateCont->SetAttribute(_T("font"), _T("2"));
		tmpHLayout->Add(txtTravelDateCont);
	}

	return tmpHLayout;
}

CListUI* COrderManagerUI::CreateTicketList(CDuiString lstTicketListName)
{
	////
	CListUI *lstTickUI = new CListUI();

	lstTickUI->SetAttribute(_T("vscrollbar"), _T("true"));
	lstTickUI->SetAttribute(_T("hscrollbar"), _T("true"));
	lstTickUI->SetAttribute(_T("itemvalign"), _T("vcenter"));

	lstTickUI->SetName(lstTicketListName);
	lstTickUI->SetVisible(false);
	
	lstTickUI->Add(CreateTicketListHeader());

	return lstTickUI;
}


CListHeaderUI* COrderManagerUI::CreateTicketListHeader()
{
	CListHeaderUI  *headUI = new CListHeaderUI();
	headUI->SetAttribute(_T("height"), _T("20"));

	{
		CListHeaderItemUI *headerItemUI = new CListHeaderItemUI();
		headerItemUI->SetText(_T("������Ϣ"));
		headerItemUI->SetFixedWidth(150);
		headerItemUI->SetHotImage(_T("res='common/list_header_hot.png'"));
		headerItemUI->SetPushedImage(_T("res='common/list_header_pushed.png'"));
		headerItemUI->SetSepImage(_T("res='common/list_header_sep.png'"));
		headerItemUI->SetSepWidth(1);
		headerItemUI->SetAttribute(_T("align"), _T("center"));
		headUI->Add(headerItemUI);


		headerItemUI = new CListHeaderItemUI();
		headerItemUI->SetText(_T("ϯλ��Ϣ"));
		headerItemUI->SetFixedWidth(150);
		headerItemUI->SetHotImage(_T("res='common/list_header_hot.png'"));
		headerItemUI->SetPushedImage(_T("res='common/list_header_pushed.png'"));
		headerItemUI->SetSepImage(_T("res='common/list_header_sep.png'"));
		headerItemUI->SetSepWidth(1);
		headerItemUI->SetAttribute(_T("align"), _T("center"));
		headUI->Add(headerItemUI);

		headerItemUI = new CListHeaderItemUI();
		headerItemUI->SetText(_T("�ÿ���Ϣ"));
		headerItemUI->SetFixedWidth(150);
		headerItemUI->SetHotImage(_T("res='common/list_header_hot.png'"));
		headerItemUI->SetPushedImage(_T("res='common/list_header_pushed.png'"));
		headerItemUI->SetSepImage(_T("res='common/list_header_sep.png'"));
		headerItemUI->SetSepWidth(1);
		headerItemUI->SetAttribute(_T("align"), _T("center"));
		headUI->Add(headerItemUI);

		headerItemUI = new CListHeaderItemUI();
		headerItemUI->SetText(_T("Ʊ����"));
		headerItemUI->SetFixedWidth(100);
		headerItemUI->SetHotImage(_T("res='common/list_header_hot.png'"));
		headerItemUI->SetPushedImage(_T("res='common/list_header_pushed.png'"));
		headerItemUI->SetSepImage(_T("res='common/list_header_sep.png'"));
		headerItemUI->SetSepWidth(1);
		headerItemUI->SetAttribute(_T("align"), _T("center"));
		headUI->Add(headerItemUI);

		headerItemUI = new CListHeaderItemUI();
		headerItemUI->SetText(_T("��Ʊ״̬"));
		headerItemUI->SetFixedWidth(100);
		headerItemUI->SetHotImage(_T("res='common/list_header_hot.png'"));
		headerItemUI->SetPushedImage(_T("res='common/list_header_pushed.png'"));
		headerItemUI->SetSepImage(_T("res='common/list_header_sep.png'"));
		headerItemUI->SetSepWidth(1);
		headerItemUI->SetAttribute(_T("align"), _T("center"));
		headUI->Add(headerItemUI);

		headerItemUI = new CListHeaderItemUI();
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