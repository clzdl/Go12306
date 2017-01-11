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
	void SetPhoneNo(CDuiString v) { m_sPhoneNo = v; }
	void SetType(CDuiString v) { m_sType = v; }

	CDuiString GetName() const { return m_sName; }
	CDuiString GetCardType() const { return m_sCardType; }
	CDuiString GetCardTypeName() const { return m_sCardTypeName; }
	CDuiString GetCardNo() const { return  m_sCardNo; }
	CDuiString GetPhoneNo() const { return m_sPhoneNo; }
	CDuiString GetType() const { return m_sType; }

	bool operator <(const CPassenger &passenger) const
	{
		return m_sCardNo < passenger.GetCardNo();
	}

private:

	CDuiString m_sName;
	CDuiString m_sCardType;
	CDuiString m_sCardTypeName;
	CDuiString m_sCardNo;
	CDuiString m_sPhoneNo;
	CDuiString m_sType;   
};