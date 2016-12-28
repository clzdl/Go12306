#include "stdafx.h"
#include "TicketManager.h"

#include "MainFrame.h"
#include "TicketModel.h"

CDuiString CTicketManager::m_transLiShi[] = { _T("当日到达"),_T("次日到达"),_T("两日到达"),_T("三日到达") };

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

	///添加行
	CListContainerElementUI* pListItem = new CListContainerElementUI();
	pListItem->SetChildVAlign(DT_VCENTER);

	pListItem->SetFixedHeight(40);

	pListItem->SetManager(m_mainFrame->GetPaintManagerUI(), NULL, false);

	m_ticketListView->Add(pListItem);

	{
		///车次
		CLabelUI *pTxtUI = new CLabelUI();
		pTxtUI->SetManager(m_mainFrame->GetPaintManagerUI(), NULL, false);
		pTxtUI->SetAttribute(_T("style"), _T("listitem_style"));
		pTxtUI->SetFont(2);
		pTxtUI->SetText(ticketModel->GetStationTrainCode());
		pListItem->Add(pTxtUI);
	}


	{///出发站/到达站
		CVerticalLayoutUI *pStationVLayoutUI = new CVerticalLayoutUI();

		CLabelUI *pTxtUI = new CLabelUI();
		pTxtUI->SetManager(m_mainFrame->GetPaintManagerUI(), NULL, false);
		pTxtUI->SetAttribute(_T("style"), _T("listitem_style"));
		if (ticketModel->GetFromStationTelecode() == ticketModel->GetStartStationTelecode())
			tmpString = _T("(始)");
		else
			tmpString = _T("(过)");

		pTxtUI->SetText(tmpString + ticketModel->GetFromStationName());
		pStationVLayoutUI->Add(pTxtUI);

		///////

		pTxtUI = new CLabelUI();
		pTxtUI->SetManager(m_mainFrame->GetPaintManagerUI(), NULL, false);
		pTxtUI->SetAttribute(_T("style"), _T("listitem_style"));

		if (ticketModel->GetToStationTelecode() == ticketModel->GetEndStationTelecode())
			tmpString = _T("(终)");
		else
			tmpString = _T("(过)");

		pTxtUI->SetText(tmpString + ticketModel->GetToStationName());
		pStationVLayoutUI->Add(pTxtUI);

		pListItem->Add(pStationVLayoutUI);
	}

	{///出发时间  到达时间
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
	{///历时

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

	{///商务座
		CLabelUI *pTxtUI = new CLabelUI();
		pTxtUI->SetManager(m_mainFrame->GetPaintManagerUI(), NULL, false);
		pTxtUI->SetAttribute(_T("style"), _T("listitem_style"));
		if (ticketModel->GetSwzNum().Compare(_T("--")) && ticketModel->GetSwzNum().Compare(_T("无")))
		{
			pTxtUI->SetFont(2);
			pTxtUI->SetTextColor(0xFF00AA00);
		}
		pTxtUI->SetText(ticketModel->GetSwzNum());
		pListItem->Add(pTxtUI);
	}

	{///特等座
		CLabelUI *pTxtUI = new CLabelUI();
		pTxtUI->SetManager(m_mainFrame->GetPaintManagerUI(), NULL, false);
		pTxtUI->SetAttribute(_T("style"), _T("listitem_style"));
		if (ticketModel->GetTzNum().Compare(_T("--")) && ticketModel->GetTzNum().Compare(_T("无")))
		{
			pTxtUI->SetFont(2);

			pTxtUI->SetTextColor(0xFF00AA00);
		}
		pTxtUI->SetText(ticketModel->GetTzNum());
		pListItem->Add(pTxtUI);
	}

	{///一等座
		CLabelUI *pTxtUI = new CLabelUI();
		pTxtUI->SetManager(m_mainFrame->GetPaintManagerUI(), NULL, false);
		pTxtUI->SetAttribute(_T("style"), _T("listitem_style"));
		if (ticketModel->GetZyNum().Compare(_T("--")) && ticketModel->GetZyNum().Compare(_T("无")))
		{
			pTxtUI->SetFont(2);
			pTxtUI->SetTextColor(0xFF00AA00);
		}
		pTxtUI->SetText(ticketModel->GetZyNum());
		pListItem->Add(pTxtUI);
	}

	{///二等座
		CLabelUI *pTxtUI = new CLabelUI();
		pTxtUI->SetManager(m_mainFrame->GetPaintManagerUI(), NULL, false);
		pTxtUI->SetAttribute(_T("style"), _T("listitem_style"));
		if (ticketModel->GetZeNum().Compare(_T("--")) && ticketModel->GetZeNum().Compare(_T("无")))
		{
			pTxtUI->SetFont(2);
			pTxtUI->SetTextColor(0xFF00AA00);
		}
		pTxtUI->SetText(ticketModel->GetZeNum());
		pListItem->Add(pTxtUI);
	}

	{///高级软卧
		CLabelUI *pTxtUI = new CLabelUI();
		pTxtUI->SetManager(m_mainFrame->GetPaintManagerUI(), NULL, false);
		pTxtUI->SetAttribute(_T("style"), _T("listitem_style"));
		if (ticketModel->GetGrNum().Compare(_T("--")) && ticketModel->GetGrNum().Compare(_T("无")))
		{
			pTxtUI->SetFont(2);
			pTxtUI->SetTextColor(0xFF00AA00);
		}
		pTxtUI->SetText(ticketModel->GetGrNum());
		pListItem->Add(pTxtUI);
	}

	{///软卧
		CLabelUI *pTxtUI = new CLabelUI();
		pTxtUI->SetManager(m_mainFrame->GetPaintManagerUI(), NULL, false);
		pTxtUI->SetAttribute(_T("style"), _T("listitem_style"));
		if (ticketModel->GetRwNum().Compare(_T("--")) && ticketModel->GetRwNum().Compare(_T("无")))
		{
			pTxtUI->SetFont(2);
			pTxtUI->SetTextColor(0xFF00AA00);
		}
		pTxtUI->SetText(ticketModel->GetRwNum());
		pListItem->Add(pTxtUI);
	}

	{///硬卧
		CLabelUI *pTxtUI = new CLabelUI();
		pTxtUI->SetManager(m_mainFrame->GetPaintManagerUI(), NULL, false);
		pTxtUI->SetAttribute(_T("style"), _T("listitem_style"));
		if (ticketModel->GetYwNum().Compare(_T("--")) && ticketModel->GetYzNum().Compare(_T("无")))
		{
			pTxtUI->SetFont(2);
			pTxtUI->SetTextColor(0xFF00AA00);
		}
		pTxtUI->SetText(ticketModel->GetYwNum());
		pListItem->Add(pTxtUI);
	}

	{///软座
		CLabelUI *pTxtUI = new CLabelUI();
		pTxtUI->SetManager(m_mainFrame->GetPaintManagerUI(), NULL, false);
		pTxtUI->SetAttribute(_T("style"), _T("listitem_style"));
		if (ticketModel->GetRzNum().Compare(_T("--")) && ticketModel->GetYzNum().Compare(_T("无")))
		{
			pTxtUI->SetFont(2);
			pTxtUI->SetTextColor(0xFF00AA00);
		}
		pTxtUI->SetText(ticketModel->GetRzNum());
		pListItem->Add(pTxtUI);
	}

	{///硬座
		CLabelUI *pTxtUI = new CLabelUI();
		pTxtUI->SetManager(m_mainFrame->GetPaintManagerUI(), NULL, false);
		pTxtUI->SetAttribute(_T("style"), _T("listitem_style"));
		if (ticketModel->GetYzNum().Compare(_T("--")) && ticketModel->GetYzNum().Compare(_T("无")))
		{
			pTxtUI->SetFont(2);
			pTxtUI->SetTextColor(0xFF00AA00);
		}
		pTxtUI->SetText(ticketModel->GetYzNum());
		pListItem->Add(pTxtUI);
	}

	{///无座
		CLabelUI *pTxtUI = new CLabelUI();
		pTxtUI->SetManager(m_mainFrame->GetPaintManagerUI(), NULL, false);
		pTxtUI->SetAttribute(_T("style"), _T("listitem_style"));
		if (ticketModel->GetWzNum().Compare(_T("--")) && ticketModel->GetWzNum().Compare(_T("无")))
		{
			pTxtUI->SetFont(2);
			pTxtUI->SetTextColor(0xFF00AA00);
		}
		pTxtUI->SetText(ticketModel->GetWzNum());
		pListItem->Add(pTxtUI);
	}

	{///其它
		CLabelUI *pTxtUI = new CLabelUI();
		pTxtUI->SetManager(m_mainFrame->GetPaintManagerUI(), NULL, false);
		pTxtUI->SetAttribute(_T("style"), _T("listitem_style"));
		if (ticketModel->GetQtNum().Compare(_T("--")) && ticketModel->GetQtNum().Compare(_T("无")))
		{
			pTxtUI->SetFont(2);
			pTxtUI->SetTextColor(0xFF00AA00);
		}
		pTxtUI->SetText(ticketModel->GetQtNum());
		pListItem->Add(pTxtUI);
	}

	{///备注
		if (ticketModel->GetBtnTextInfo() == _T("预订"))
		{

			CButtonUI *pBtnUI = new CButtonUI();

			pBtnUI->SetName(_T("OrderTicketBtn"));
			pBtnUI->SetManager(m_mainFrame->GetPaintManagerUI(), NULL, false);
			pBtnUI->SetAttribute(_T("style"), _T("btn_style"));
			pBtnUI->SetFixedHeight(30);
			pBtnUI->SetFixedWidth(50);

			pBtnUI->SetUserData(ticketModel->GetStationTrainCode());

			pBtnUI->SetText(_T("预订"));

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