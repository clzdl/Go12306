

#include "stdafx.h"
#include "OrderModel.h"

COrderTicketModel::COrderTicketModel(COrderModel *orderModel)
	:m_orderModel(orderModel)
{
}
COrderTicketModel::~COrderTicketModel()
{

}

COrderModel::COrderModel()
{
	
}

COrderModel::~COrderModel()
{
}

void COrderModel::Add(COrderTicketModel &otm)
{
	m_vecOrderTicket.push_back(otm);
}