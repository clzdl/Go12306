#pragma once


class COrderModel;

class COrderTicketModel
{
public:

	COrderTicketModel(COrderModel *orderModel);
	~COrderTicketModel();

	void SetCoachName(CDuiString v) { m_strCoachName = v; }
	void SetSeatName(CDuiString v) { m_strSeatName = v; }
	void SetSeatTypeName(CDuiString v) { m_strSeatTypeName = v; }
	void SetPassengerPersonName(CDuiString v) { m_strPassengerPersonName = v; }
	void SetPassengerIDTypeName(CDuiString v) { m_strPassengerIDTypeName = v; }
	void SetTicketTypeName(CDuiString v) { m_strTicketTypeName = v; }
	void SetTicketPrice(CDuiString v) { m_strTicketPrice = v; }
	void SetTicketPayStatus(CDuiString v) { m_strTicketPayStatus = v; }
	void SetTicketNo(CDuiString v) { m_strTicketNo = v; }


	CDuiString GetCoachName() { return m_strCoachName; }
	CDuiString GetSeatName() { return m_strSeatName; }
	CDuiString GetSeatTypeName() { return m_strSeatTypeName; }
	CDuiString GetPassengerPersonName() { return m_strPassengerPersonName; }
	CDuiString GetPassengerIDTypeName() { return m_strPassengerIDTypeName; }
	CDuiString GetTicketTypeName() { return m_strTicketTypeName; }
	CDuiString GetTicketPrice() { return m_strTicketPrice; }
	CDuiString GetTicketPayStatus() { return m_strTicketPayStatus; }
	CDuiString GetTicketNo() { return m_strTicketNo; }

private:
	CDuiString m_strCoachName;		////coach_no   车厢名称
	CDuiString m_strSeatName;		////seat_name   座位号码
	CDuiString m_strSeatTypeName;		////seat_type_name  座位类型名称
	CDuiString m_strPassengerPersonName;		///passenger_name
	CDuiString m_strPassengerIDTypeName;		///passenger_id_type_name

	CDuiString m_strTicketTypeName; ////ticket_type_name
	CDuiString m_strTicketPrice;		///str_ticket_price_page
	CDuiString m_strTicketPayStatus;		////ticket_status_name
	CDuiString m_strTicketNo;			///ticket_no

	COrderModel *m_orderModel;
};

class COrderModel
{
	

public:
	COrderModel();
	~COrderModel();

	void SetOrderNo(CDuiString v) { m_strOrderNo = v; }
	void SetOrderDate(CDuiString v) { m_strOrderDate = v; }
	void SetPassengerName(CDuiString v) { m_strPassengerName = v; }
	void SetFromStation(CDuiString v) { m_strFromStation = v; }
	void SetToStateion(CDuiString v) { m_strToStateion = v; }
	void SetTravelDate(CDuiString v) { m_strTravelDate = v; }
	void SetTrainCode(CDuiString v) { m_strTrainCode = v; }

	CDuiString GetOrderNo() { return m_strOrderNo; }
	CDuiString GetOrderDate() { return m_strOrderDate; }
	CDuiString GetPassengerName() { return m_strPassengerName; }
	CDuiString GetFromStation() { return m_strFromStation; }
	CDuiString GetToStateion() { return m_strToStateion; }
	CDuiString GetTravelDate() { return m_strTravelDate; }
	CDuiString GetTrainCode() { return  m_strTrainCode; }

	void Add(COrderTicketModel &otm);

	std::vector<COrderTicketModel>& GetOrderTicket() {return m_vecOrderTicket;}
private:

	CDuiString m_strOrderNo;   ////sequence_no
	CDuiString m_strOrderDate;   ////order_date
	CDuiString m_strPassengerName;  ////array_passser_name_page
	CDuiString m_strFromStation;   ////from_station_name_page
	CDuiString m_strToStateion;		/////to_station_name_page
	CDuiString m_strTravelDate;		////start_train_date_page
	CDuiString m_strTrainCode;		////train_code_page  
	
		 
	std::vector<COrderTicketModel> m_vecOrderTicket;

};
