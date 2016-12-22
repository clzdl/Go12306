#pragma once


class CPassengerTicket
{
public:
	CPassengerTicket();
	~CPassengerTicket();

	void SetSeatType(CDuiString v) { m_sSeatType = v; }
	void SetTicketType(CDuiString v) { m_sTicketType = v; }
	void SetPassengerName(CDuiString v) { m_sPassengerName = v; }
	void SetIdType(CDuiString v) { m_sIdType = v; }
	void SetIdNo(CDuiString v) { m_sIdNo = v; }
	void SetMobileNo(CDuiString v) { m_sMobileNo = v; }
	void SetSaveStatus(CDuiString v) { m_sSaveStatus = v; }
	void SetPassengerType(CDuiString v) { m_sPassengerType = v; }



	CDuiString GetSeatType() { return m_sSeatType; }
	CDuiString GetTicketType() { return m_sTicketType; }
	CDuiString GetPassengerName() { return  m_sPassengerName; }
	CDuiString GetIdType() { return  m_sIdType ; }
	CDuiString GetIdNo() { return  m_sIdNo; }
	CDuiString GetMobileNo() { return m_sMobileNo ; }
	CDuiString GetSaveStatus() { return  m_sSaveStatus; }
	CDuiString GetPassengerType() { return  m_sPassengerType; }

private:

	CDuiString m_sSeatType;    ///席别
	CDuiString m_sTicketType;    ///票种
	CDuiString m_sPassengerName;   ///乘客姓名
	CDuiString m_sIdType;   ///乘客证件类型
	CDuiString m_sIdNo;   ///乘客证件h号码
	CDuiString m_sMobileNo;  ///乘客电话
	CDuiString m_sSaveStatus;   ///状态保存：  N/Y
	CDuiString m_sPassengerType;   

};