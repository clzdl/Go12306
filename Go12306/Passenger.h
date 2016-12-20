#pragma once


class CPassenger
{
public:
	CPassenger();
	~CPassenger();

	void SetName(CDuiString v) { m_sName = v; }
	void SetCardType(CDuiString v) { m_sCardType = v; }
	void SetCardTypeName(CDuiString v) { m_sCardTypeName = v; }
	void SetCardNo(CDuiString v) { m_sCardNo = v; }


	CDuiString GetName() { return m_sName; }
	CDuiString GetCardType() { return m_sCardType; }
	CDuiString GetCardTypeName() { return m_sCardTypeName; }
	CDuiString GetCardNo() { return  m_sCardNo; }

private:

	CDuiString m_sName;
	CDuiString m_sCardType;
	CDuiString m_sCardTypeName;
	CDuiString m_sCardNo;
};