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

	CDuiString m_sSeatType;    ///ϯ��
	CDuiString m_sTicketType;    ///Ʊ��
	CDuiString m_sPassengerName;   ///�˿�����
	CDuiString m_sIdType;   ///�˿�֤������
	CDuiString m_sIdNo;   ///�˿�֤��h����
	CDuiString m_sMobileNo;  ///�˿͵绰
	CDuiString m_sSaveStatus;   ///״̬���棺  N/Y
	CDuiString m_sPassengerType;   

};