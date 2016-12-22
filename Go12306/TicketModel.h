#pragma once


class CTicketModel
{
public:
	CTicketModel() {};
	~CTicketModel() {};


	void SetTrainNo(CDuiString v) { m_strTrainNo = v; }
	void SetStationTrainCode(CDuiString v) { m_strStationTrainCode = v; }
	void SetStartStationTelecode(CDuiString v) { m_strStartStationTelecode = v; }
	void SetStartStationName(CDuiString v) { m_strStartStationName = v; }
	void SetEndStationTelecode(CDuiString v) { m_strEndStationTelecode = v; }
	void SetEndStationName(CDuiString v) { m_strEndStationName = v; }
	void SetFromStationTelecode(CDuiString v) { m_strFromStationTelecode = v; }
	void SetFromStationName(CDuiString v) { m_strFromStationName = v; }
	void SetToStationTelecode(CDuiString v) { m_strToStationTelecode = v; }
	void SetToStationName(CDuiString v) { m_strToStationName = v; }

	void SetStartTime(CDuiString v) { m_strStartTime = v; }
	void SetArriveTime(CDuiString v) { m_strArriveTime = v; }
	void SetDayDifferent(CDuiString v) { m_strDayDifferent = v; }
	void SetTrainClassName(CDuiString v) { m_strTrainClassName = v; }
	void SetLiShi(CDuiString v) { m_strLiShi = v; }
	void SetCanWebBuy(CDuiString v) { m_strCanWebBuy = v; }
	void SetLiShiValue(CDuiString v) { m_strLiShiValue = v; }
	void SetYpInfo(CDuiString v) { m_strYpInfo = v; }
	void SetControlTrainDay(CDuiString v) { m_strControlTrainDay = v; }
	void SetStartTrainDate(CDuiString v) { m_strStartTrainDate = v; }
	void SetSeatFeature(CDuiString v) { m_strSeatFeature = v; }
	void SetYpEx(CDuiString v) { m_strYpEx = v; }
	void SetTrainSeatFeatrue(CDuiString v) { m_strTrainSeatFeatrue = v; }
	void SetSeatTypes(CDuiString v) { m_strSeatTypes = v; }
	void SetLocationCode(CDuiString v) { m_strLocationCode = v; }
	void SetFromStationNo(CDuiString v) { m_strFromStationNo = v; }
	void SetToStationNo(CDuiString v) { m_strToStationNo = v; }
	void SetControlDay(CDuiString v) { m_strControlDay = v; }
	void SetSaleTime(CDuiString v) { m_strSaleTime = v; }
	void SetIsSupportCard(CDuiString v) { m_strIsSupportCard = v; }
	void SetControlledTrainFlag(CDuiString v) { m_strControlledTrainFlag = v; }
	void SetControlledTrainMessage(CDuiString v) { m_strControlledTrainMessage = v; }
	void SetGgNum(CDuiString v) { m_strGgNum = v; }
	void SetGrNum(CDuiString v) { m_strGrNum = v; }
	void SetQtNum(CDuiString v) { m_strQtNum = v; }
	void SetRwNum(CDuiString v) { m_strRwNum = v; }
	void SetRzNum(CDuiString v) { m_strRzNum = v; }
	void SetTzNum(CDuiString v) { m_strTzNum = v; }
	void SetWzNum(CDuiString v) { m_strWzNum = v; }
	void SetYbNum(CDuiString v) { m_strYbNum = v; }
	void SetYwNum(CDuiString v) { m_strYwNum = v; }
	void SetYzNum(CDuiString v) { m_strYzNum = v; }
	void SetZeNum(CDuiString v) { m_strZeNum = v; }
	void SetZyNum(CDuiString v) { m_strZyNum = v; }
	void SetSwzNum(CDuiString v) { m_strSwzNum = v; }
	void SetSecretStr(CDuiString v) { m_strSecretStr = v; }
	void SetBtnTextInfo(CDuiString v) { m_strBtnTextInfo = v; }
	void SetTrainDate(CDuiString v) { m_strTrainDate = v; }
	void SetTicketType(_TICKET_TYPE v) { m_ticketType = v; }


	CDuiString GetTrainNo() { return m_strTrainNo; }
	CDuiString GetStationTrainCode() { return m_strStationTrainCode; }
	CDuiString GetStartStationTelecode() { return m_strStartStationTelecode; }
	CDuiString GetStartStationName() { return m_strStartStationName; }
	CDuiString GetEndStationTelecode() { return m_strEndStationTelecode; }
	CDuiString GetEndStationName() { return m_strEndStationName; }
	CDuiString GetFromStationTelecode() {return  m_strFromStationTelecode; }
	CDuiString GetFromStationName() { return m_strFromStationName; }
	CDuiString GetToStationTelecode() { return m_strToStationTelecode; }
	CDuiString GetToStationName() { return m_strToStationName; }

	CDuiString GetStartTime() { return m_strStartTime; }
	CDuiString GetArriveTime() { return m_strArriveTime; }
	CDuiString GetDayDifferent() { return m_strDayDifferent; }
	CDuiString GetTrainClassName() { return m_strTrainClassName; }
	CDuiString GetLiShi() {return m_strLiShi; }
	CDuiString GetCanWebBuy() { return m_strCanWebBuy; }
	CDuiString GetLiShiValue() { return m_strLiShiValue; }
	CDuiString GetYpInfo() { return m_strYpInfo; }
	CDuiString GetControlTrainDay() { return m_strControlTrainDay; }
	CDuiString GetStartTrainDate() { return m_strStartTrainDate; }
	CDuiString GetYpEx() { return m_strYpEx; }
	CDuiString GetTrainSeatFeatrue() { return m_strTrainSeatFeatrue; }
	CDuiString GetSeatTypes() { return m_strSeatTypes; }
	CDuiString GetLocationCode() { return m_strLocationCode; }
	CDuiString GetFromStationNo() { return m_strFromStationNo; }
	CDuiString GetToStationNo() { return m_strToStationNo; }
	CDuiString GetControlDay() { return  m_strControlDay; }
	CDuiString GetSaleTime() { return  m_strSaleTime; }
	CDuiString GetIsSupportCard() { return m_strIsSupportCard; }
	CDuiString GetControlledTrainFlag() { return m_strControlledTrainFlag; }
	CDuiString GetControlledTrainMessage() { return m_strControlledTrainMessage; }
	CDuiString GetGgNum() { return m_strGgNum; }
	CDuiString GetGrNum() { return m_strGrNum; }
	CDuiString GetQtNum() { return m_strQtNum; }
	CDuiString GetRwNum() { return m_strRwNum; }
	CDuiString GetRzNum() { return m_strRzNum; }
	CDuiString GetTzNum() { return m_strTzNum; }
	CDuiString GetWzNum() { return m_strWzNum; }
	CDuiString GetYbNum() { return m_strYbNum; }
	CDuiString GetYwNum() { return m_strYwNum; }
	CDuiString GetYzNum() { return m_strYzNum; }
	CDuiString GetZeNum() { return m_strZeNum; }
	CDuiString GetZyNum() { return m_strZyNum; }
	CDuiString GetSwzNum() { return m_strSwzNum; }
	CDuiString GetSecretStr() { return m_strSecretStr; }
	CDuiString GetBtnTextInfo() { return m_strBtnTextInfo; }
	CDuiString GetTrainDate() { return m_strTrainDate; }

	_TICKET_TYPE GetTicketType() { return m_ticketType; }

private:

	CDuiString m_strTrainNo;						////train_no
	CDuiString m_strStationTrainCode;				////station_train_code
	CDuiString m_strStartStationTelecode;			////start_station_telecode
	CDuiString m_strStartStationName;				////start_station_name
	CDuiString m_strEndStationTelecode;				////end_station_telecode
	CDuiString m_strEndStationName;					////end_station_name
	CDuiString m_strFromStationTelecode;			////from_station_telecode
	CDuiString m_strFromStationName;				////from_station_name
	CDuiString m_strToStationTelecode;				////to_station_telecode
	CDuiString m_strToStationName;					////to_station_name
	CDuiString m_strStartTime;						////start_time
	CDuiString m_strArriveTime;						////arrive_time
	CDuiString m_strDayDifferent;					////day_difference
	CDuiString m_strTrainClassName;					////train_class_name
	CDuiString m_strLiShi;							////lishi
	CDuiString m_strCanWebBuy;						////canWebBuy

	CDuiString m_strLiShiValue;						////lishiValue
	CDuiString m_strYpInfo;							////yp_info
	CDuiString m_strControlTrainDay;				////control_train_day
	CDuiString m_strStartTrainDate;					////start_train_date
	CDuiString m_strSeatFeature;					////seat_feature  yp_ex
	CDuiString m_strYpEx;							////yp_ex
	CDuiString m_strTrainSeatFeatrue;				////train_seat_feature
	CDuiString m_strSeatTypes;						////seat_types
	CDuiString m_strLocationCode;					////location_code
	CDuiString m_strFromStationNo;					////from_station_no
	CDuiString m_strToStationNo;					////to_station_no
	CDuiString m_strControlDay;						////control_day
	CDuiString m_strSaleTime;						////sale_time
	CDuiString m_strIsSupportCard;					////is_support_card
	CDuiString m_strControlledTrainFlag;			////controlled_train_flag
	CDuiString m_strControlledTrainMessage;			////controlled_train_message
	CDuiString m_strGgNum;							////gg_num
	CDuiString m_strGrNum;							////gr_num		高级软卧
	CDuiString m_strQtNum;							////qt_num		其它
	CDuiString m_strRwNum;							////rw_num		软卧
	CDuiString m_strRzNum;							////rz_num		软座
	CDuiString m_strTzNum;							////tz_num		特等座
	CDuiString m_strWzNum;							////wz_num		无座
	CDuiString m_strYbNum;							////yb_num
	CDuiString m_strYwNum;							////yw_num		硬卧
	CDuiString m_strYzNum;							////yz_num		硬座
	CDuiString m_strZeNum;							////ze_num		二等座
	CDuiString m_strZyNum;							////zy_num		一等座  
	CDuiString m_strSwzNum;							////swz_num  商务座


	CDuiString m_strTrainDate;				///乘车日期

	CDuiString m_strSecretStr;						////secretStr
	CDuiString m_strBtnTextInfo;					///buttonTextInfo
	
	_TICKET_TYPE m_ticketType;

};
