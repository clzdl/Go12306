#include "stdafx.h"
#include "TicketManager.h"

#include "MainFrame.h"
#include "TicketModel.h"

CDuiString CTicketManager::m_transLiShi[] = { _T("���յ���"),_T("���յ���"),_T("���յ���"),_T("���յ���") };

CTicketManager::CTicketManager(CMainFrame *mainFrame)
	:m_mainFrame(mainFrame)
{
	m_ticketListView = static_cast<CListUI*>(m_mainFrame->GetPaintManagerUI()->FindControl(_T("ticketListView")));
}

void CTicketManager::ResetTicketList() {
	if (m_ticketListView)
		m_ticketListView->RemoveAll();
}

_ERRNO CTicketManager::RefreshTicketListView(CTicketModel *ticketModel)
{
	CDuiString tmpString;

	///�����
	CListContainerElementUI* pListItem = new CListContainerElementUI();
	pListItem->SetChildVAlign(DT_VCENTER);

	pListItem->SetFixedHeight(40);

	pListItem->SetManager(m_mainFrame->GetPaintManagerUI(), NULL, false);

	m_ticketListView->Add(pListItem);

	{
		///����
		CLabelUI *pTxtUI = new CLabelUI();
		pTxtUI->SetManager(m_mainFrame->GetPaintManagerUI(), NULL, false);
		pTxtUI->SetAttribute(_T("style"), _T("listitem_style"));
		pTxtUI->SetFont(2);
		pTxtUI->SetText(ticketModel->GetStationTrainCode());
		pListItem->Add(pTxtUI);
	}


	{///����վ/����վ
		CVerticalLayoutUI *pStationVLayoutUI = new CVerticalLayoutUI();

		CLabelUI *pTxtUI = new CLabelUI();
		pTxtUI->SetManager(m_mainFrame->GetPaintManagerUI(), NULL, false);
		pTxtUI->SetAttribute(_T("style"), _T("listitem_style"));
		if (ticketModel->GetFromStationTelecode() == ticketModel->GetStartStationTelecode())
			tmpString = _T("(ʼ)");
		else
			tmpString = _T("(��)");

		pTxtUI->SetText(tmpString + ticketModel->GetFromStationName());
		pStationVLayoutUI->Add(pTxtUI);

		///////

		pTxtUI = new CLabelUI();
		pTxtUI->SetManager(m_mainFrame->GetPaintManagerUI(), NULL, false);
		pTxtUI->SetAttribute(_T("style"), _T("listitem_style"));

		if (ticketModel->GetToStationTelecode() == ticketModel->GetEndStationTelecode())
			tmpString = _T("(��)");
		else
			tmpString = _T("(��)");

		pTxtUI->SetText(tmpString + ticketModel->GetToStationName());
		pStationVLayoutUI->Add(pTxtUI);

		pListItem->Add(pStationVLayoutUI);
	}

	{///����ʱ��  ����ʱ��
		CVerticalLayoutUI *pTimeVLayoutUI = new CVerticalLayoutUI();
		CLabelUI *pTxtUI = new CLabelUI();
		pTxtUI->SetManager(m_mainFrame->GetPaintManagerUI(), NULL, false);
		pTxtUI->SetAttribute(_T("style"), _T("listitem_style"));

		pTxtUI->SetText(ticketModel->GetStartTime());
		pTimeVLayoutUI->Add(pTxtUI);

		pTxtUI = new CLabelUI();
		pTxtUI->SetManager(m_mainFrame->GetPaintManagerUI(), NULL, false);
		pTxtUI->SetAttribute(_T("style"), _T("listitem_style"));

		pTxtUI->SetText(ticketModel->GetArriveTime());
		pTimeVLayoutUI->Add(pTxtUI);

		pListItem->Add(pTimeVLayoutUI);


	}
	{///��ʱ

		CVerticalLayoutUI *pLiShiVLayoutUI = new CVerticalLayoutUI();
		CLabelUI *pTxtUI = new CLabelUI();
		pTxtUI->SetManager(m_mainFrame->GetPaintManagerUI(), NULL, false);
		pTxtUI->SetAttribute(_T("style"), _T("listitem_style"));

		pTxtUI->SetText(ticketModel->GetLiShi());
		pLiShiVLayoutUI->Add(pTxtUI);

		pTxtUI = new CLabelUI();
		pTxtUI->SetManager(m_mainFrame->GetPaintManagerUI(), NULL, false);
		pTxtUI->SetAttribute(_T("style"), _T("listitem_style"));
		pTxtUI->SetText(m_transLiShi[_wtoi(ticketModel->GetDayDifferent().GetData())]);
		pLiShiVLayoutUI->Add(pTxtUI);

		pListItem->Add(pLiShiVLayoutUI);
	}

	{///������
		CLabelUI *pTxtUI = new CLabelUI();
		pTxtUI->SetManager(m_mainFrame->GetPaintManagerUI(), NULL, false);
		pTxtUI->SetAttribute(_T("style"), _T("listitem_style"));
		if (ticketModel->GetSwzNum().Compare(_T("--")) && ticketModel->GetSwzNum().Compare(_T("��")))
		{
			pTxtUI->SetFont(2);
			pTxtUI->SetTextColor(0xFF00AA00);
		}
		pTxtUI->SetText(ticketModel->GetSwzNum());
		pListItem->Add(pTxtUI);
	}

	{///�ص���
		CLabelUI *pTxtUI = new CLabelUI();
		pTxtUI->SetManager(m_mainFrame->GetPaintManagerUI(), NULL, false);
		pTxtUI->SetAttribute(_T("style"), _T("listitem_style"));
		if (ticketModel->GetTzNum().Compare(_T("--")) && ticketModel->GetTzNum().Compare(_T("��")))
		{
			pTxtUI->SetFont(2);

			pTxtUI->SetTextColor(0xFF00AA00);
		}
		pTxtUI->SetText(ticketModel->GetTzNum());
		pListItem->Add(pTxtUI);
	}

	{///һ����
		CLabelUI *pTxtUI = new CLabelUI();
		pTxtUI->SetManager(m_mainFrame->GetPaintManagerUI(), NULL, false);
		pTxtUI->SetAttribute(_T("style"), _T("listitem_style"));
		if (ticketModel->GetZyNum().Compare(_T("--")) && ticketModel->GetZyNum().Compare(_T("��")))
		{
			pTxtUI->SetFont(2);
			pTxtUI->SetTextColor(0xFF00AA00);
		}
		pTxtUI->SetText(ticketModel->GetZyNum());
		pListItem->Add(pTxtUI);
	}

	{///������
		CLabelUI *pTxtUI = new CLabelUI();
		pTxtUI->SetManager(m_mainFrame->GetPaintManagerUI(), NULL, false);
		pTxtUI->SetAttribute(_T("style"), _T("listitem_style"));
		if (ticketModel->GetZeNum().Compare(_T("--")) && ticketModel->GetZeNum().Compare(_T("��")))
		{
			pTxtUI->SetFont(2);
			pTxtUI->SetTextColor(0xFF00AA00);
		}
		pTxtUI->SetText(ticketModel->GetZeNum());
		pListItem->Add(pTxtUI);
	}

	{///�߼�����
		CLabelUI *pTxtUI = new CLabelUI();
		pTxtUI->SetManager(m_mainFrame->GetPaintManagerUI(), NULL, false);
		pTxtUI->SetAttribute(_T("style"), _T("listitem_style"));
		if (ticketModel->GetGrNum().Compare(_T("--")) && ticketModel->GetGrNum().Compare(_T("��")))
		{
			pTxtUI->SetFont(2);
			pTxtUI->SetTextColor(0xFF00AA00);
		}
		pTxtUI->SetText(ticketModel->GetGrNum());
		pListItem->Add(pTxtUI);
	}

	{///����
		CLabelUI *pTxtUI = new CLabelUI();
		pTxtUI->SetManager(m_mainFrame->GetPaintManagerUI(), NULL, false);
		pTxtUI->SetAttribute(_T("style"), _T("listitem_style"));
		if (ticketModel->GetRwNum().Compare(_T("--")) && ticketModel->GetRwNum().Compare(_T("��")))
		{
			pTxtUI->SetFont(2);
			pTxtUI->SetTextColor(0xFF00AA00);
		}
		pTxtUI->SetText(ticketModel->GetRwNum());
		pListItem->Add(pTxtUI);
	}

	{///Ӳ��
		CLabelUI *pTxtUI = new CLabelUI();
		pTxtUI->SetManager(m_mainFrame->GetPaintManagerUI(), NULL, false);
		pTxtUI->SetAttribute(_T("style"), _T("listitem_style"));
		if (ticketModel->GetYwNum().Compare(_T("--")) && ticketModel->GetYzNum().Compare(_T("��")))
		{
			pTxtUI->SetFont(2);
			pTxtUI->SetTextColor(0xFF00AA00);
		}
		pTxtUI->SetText(ticketModel->GetYwNum());
		pListItem->Add(pTxtUI);
	}

	{///����
		CLabelUI *pTxtUI = new CLabelUI();
		pTxtUI->SetManager(m_mainFrame->GetPaintManagerUI(), NULL, false);
		pTxtUI->SetAttribute(_T("style"), _T("listitem_style"));
		if (ticketModel->GetRzNum().Compare(_T("--")) && ticketModel->GetYzNum().Compare(_T("��")))
		{
			pTxtUI->SetFont(2);
			pTxtUI->SetTextColor(0xFF00AA00);
		}
		pTxtUI->SetText(ticketModel->GetRzNum());
		pListItem->Add(pTxtUI);
	}

	{///Ӳ��
		CLabelUI *pTxtUI = new CLabelUI();
		pTxtUI->SetManager(m_mainFrame->GetPaintManagerUI(), NULL, false);
		pTxtUI->SetAttribute(_T("style"), _T("listitem_style"));
		if (ticketModel->GetYzNum().Compare(_T("--")) && ticketModel->GetYzNum().Compare(_T("��")))
		{
			pTxtUI->SetFont(2);
			pTxtUI->SetTextColor(0xFF00AA00);
		}
		pTxtUI->SetText(ticketModel->GetYzNum());
		pListItem->Add(pTxtUI);
	}

	{///����
		CLabelUI *pTxtUI = new CLabelUI();
		pTxtUI->SetManager(m_mainFrame->GetPaintManagerUI(), NULL, false);
		pTxtUI->SetAttribute(_T("style"), _T("listitem_style"));
		if (ticketModel->GetWzNum().Compare(_T("--")) && ticketModel->GetWzNum().Compare(_T("��")))
		{
			pTxtUI->SetFont(2);
			pTxtUI->SetTextColor(0xFF00AA00);
		}
		pTxtUI->SetText(ticketModel->GetWzNum());
		pListItem->Add(pTxtUI);
	}

	{///����
		CLabelUI *pTxtUI = new CLabelUI();
		pTxtUI->SetManager(m_mainFrame->GetPaintManagerUI(), NULL, false);
		pTxtUI->SetAttribute(_T("style"), _T("listitem_style"));
		if (ticketModel->GetQtNum().Compare(_T("--")) && ticketModel->GetQtNum().Compare(_T("��")))
		{
			pTxtUI->SetFont(2);
			pTxtUI->SetTextColor(0xFF00AA00);
		}
		pTxtUI->SetText(ticketModel->GetQtNum());
		pListItem->Add(pTxtUI);
	}

	{///��ע
		if (ticketModel->GetBtnTextInfo() == _T("Ԥ��"))
		{

			CButtonUI *pBtnUI = new CButtonUI();

			pBtnUI->SetName(_T("OrderTicketBtn"));
			pBtnUI->SetManager(m_mainFrame->GetPaintManagerUI(), NULL, false);
			pBtnUI->SetAttribute(_T("style"), _T("btn_style"));
			pBtnUI->SetFixedHeight(30);
			pBtnUI->SetFixedWidth(50);

			pBtnUI->SetUserData(ticketModel->GetStationTrainCode());

			pBtnUI->SetText(_T("Ԥ��"));

			pListItem->Add(pBtnUI);
		}
		else
		{
			CLabelUI *pTxtUI = new CLabelUI();
			pTxtUI->SetManager(m_mainFrame->GetPaintManagerUI(), NULL, false);
			pTxtUI->SetAttribute(_T("style"), _T("listitem_style"));
			pTxtUI->SetFont(2);
			pTxtUI->SetTextColor(0xFFFFFFFF);
			pTxtUI->SetText(ticketModel->GetBtnTextInfo());
			pListItem->Add(pTxtUI);
		}
	}

	return E_OK;
}