
#include "stdafx.h"
#include "Client12306Manager.h"
#include <sstream>
#include "OthFunc.h"



Client12306Manager *Client12306Manager::m_objInstance = NULL;
SharedPtr<InvalidCertificateHandler> Client12306Manager::m_ptrCert = NULL;
Context::Ptr Client12306Manager::m_ptrContext = NULL;

std::string Client12306Manager::m_strDomain = "kyfw.12306.cn";

Client12306Manager *Client12306Manager::Instance()
{
	if (m_objInstance == NULL)
	{
		m_ptrCert = new ConsoleCertificateHandler(false);
		m_ptrContext = new Context(Context::CLIENT_USE, "", "", "", Context::VERIFY_NONE, 9, true, "ALL:!ADH:!LOW:!EXP:!MD5:@STRENGTH");
		SSLManager::instance().initializeClient(0, m_ptrCert, m_ptrContext);

		m_objInstance = new Client12306Manager();

	}

	return m_objInstance;
}


Client12306Manager::Client12306Manager()
{
	m_sessHttpsClient.setHost(m_strDomain);
}
Client12306Manager::~Client12306Manager()
{

}
Client12306Manager::Client12306Manager(const Client12306Manager &hcm)
{

}
Client12306Manager& Client12306Manager::operator =(const Client12306Manager &hcm)
{
	return *this;
}


std::string Client12306Manager::ExecPost(std::string service, std::map<string, string> *param)
{
	if (param && !param->empty())
	{
		service += "?";
		int i = 0;
		for (std::map<string, string>::iterator it = param->begin(); it != param->end(); ++it, ++i)
		{
			if (i > 0)
				service += "&";


			service += it->first;
			service += "=";
			service += it->second;
		}

	}

	HTTPRequest request(HTTPRequest::HTTP_POST, service);

	m_sessHttpsClient.sendRequest(request);
	HTTPResponse response;

	std::istream& rs = m_sessHttpsClient.receiveResponse(response);

	std::ostringstream ostr;
	StreamCopier::copyStream(rs, ostr);

	return ostr.str();
}

std::string Client12306Manager::ExecGet(std::string service, std::map<string, string> *param)
{
	if (param && !param->empty())
	{
		service += "?";
		int i = 0;
		for (std::map<string, string>::iterator it = param->begin(); it != param->end(); ++it,++i)
		{
			if (i > 0)
				service += "&";


			service += it->first;
			service += "=";
			service += it->second;
		}

	}
	HTTPRequest request(HTTPRequest::HTTP_GET, service);


	m_sessHttpsClient.sendRequest(request);
	HTTPResponse response;

	std::istream& rs = m_sessHttpsClient.receiveResponse(response);

	std::ostringstream ostr;
	StreamCopier::copyStream(rs, ostr);

	return ostr.str();
	
}


int Client12306Manager::QueryLeftTicket(std::string begPlace, std::string endPlace, std::string travelTime , std::vector<CTicketModel> &vecTicket, _TICKET_TYPE ticketType)
{
	try
	{

		string strService = "/otn/leftTicket/queryX?";

		///train_date   YYYY-mm-dd
		strService += "leftTicketDTO.train_date=";
		strService += travelTime;
		strService += "&";

		////leftTicketDTO.from_station
		strService += "leftTicketDTO.from_station=";
		strService += begPlace;
		strService += "&";

		////leftTicketDTO.to_station
		strService += "leftTicketDTO.to_station=";
		strService += endPlace;
		strService += "&";

		////purpose_codes
		strService += "purpose_codes=";
		switch(ticketType)
		{
		case _ADULT:
		default:
			strService += "ADULT";
			break;
		}
		

		std::string strRes = ExecGet(strService);

		JsonParseTicket(strRes, vecTicket);
		

	}
	catch (Poco::Exception &e)
	{
		DUI__Trace(_T("%s\n"), Utf8ToUnicode(e.displayText()).c_str());
		return FAIL;
	}

	return SUCCESS;
}


int Client12306Manager::JsonParseTicket(std::string jsonString, std::vector<CTicketModel> &vecTicket)
{
	JSON::Parser parser;
	Dynamic::Var result;

	result = parser.parse(jsonString);

	JSON::Object::Ptr pObj = result.extract<JSON::Object::Ptr>();

	Dynamic::Var jStatus = pObj->get("status");

	if (jStatus.toString() != "true")
	{
		DUI__Trace(Utf8ToUnicode(jStatus.toString()).c_str());
		return FAIL;
	}

	////array
	JSON::Array::Ptr pArry = pObj->getArray("data");


	JSON::Array::ConstIterator it = pArry->begin();
	//把数组里的所有内容打印出来
	//当然也可以把每个对象拿出来用。
	for (; it != pArry->end(); it++)
	{
		DUI__Trace(Utf8ToUnicode(it->toString()).c_str());

		JSON::Object::Ptr pItem = it->extract<JSON::Object::Ptr>();

		Dynamic::Var tmpJObj;
		////buttonTextInfo
		tmpJObj = pItem->get("buttonTextInfo");
		DUI__Trace(Utf8ToUnicode(tmpJObj.toString()).c_str());

		/////secretStr
		tmpJObj = pItem->get("secretStr");
		DUI__Trace(Utf8ToUnicode(tmpJObj.toString()).c_str());


		////queryLeftNewDTO
		tmpJObj = pItem->get("queryLeftNewDTO");

		JSON::Object::Ptr pQueryDto = tmpJObj.extract<JSON::Object::Ptr>();

		CTicketModel ticketModel;
		AssignJson2TicketObj(pQueryDto, ticketModel);
		
		vecTicket.emplace_back(ticketModel);

	}

	return SUCCESS;
}


int Client12306Manager::AssignJson2TicketObj(JSON::Object::Ptr queryDto, CTicketModel &objTicket)
{
	Dynamic::Var tmpJObj;
	/////train_no
	tmpJObj = queryDto->get("train_no");
	objTicket.SetTrainNo(Utf8ToUnicode(tmpJObj.toString()).c_str());

	////station_train_code
	tmpJObj = queryDto->get("station_train_code");
	objTicket.SetStationTrainCode(Utf8ToUnicode(tmpJObj.toString()).c_str());

	////start_station_telecode
	tmpJObj = queryDto->get("start_station_telecode");
	objTicket.SetStartStationTelecode(Utf8ToUnicode(tmpJObj.toString()).c_str());


	////start_station_name
	tmpJObj = queryDto->get("start_station_name");
	objTicket.SetStartStationName(Utf8ToUnicode(tmpJObj.toString()).c_str());
	
	////end_station_telecode
	tmpJObj = queryDto->get("end_station_telecode");
	objTicket.SetEndStationTelecode(Utf8ToUnicode(tmpJObj.toString()).c_str());

	////end_station_name
	tmpJObj = queryDto->get("end_station_name");
	objTicket.SetEndStationName(Utf8ToUnicode(tmpJObj.toString()).c_str());


	////from_station_telecode
	tmpJObj = queryDto->get("from_station_telecode");
	objTicket.SetFromStationTelecode(Utf8ToUnicode(tmpJObj.toString()).c_str());

	////from_station_name
	tmpJObj = queryDto->get("from_station_name");
	objTicket.SetFromStationName(Utf8ToUnicode(tmpJObj.toString()).c_str());

	////to_station_telecode
	tmpJObj = queryDto->get("to_station_telecode");
	objTicket.SetToStationTelecode(Utf8ToUnicode(tmpJObj.toString()).c_str());

	////to_station_name
	tmpJObj = queryDto->get("to_station_name");
	objTicket.SetToStationName(Utf8ToUnicode(tmpJObj.toString()).c_str());

	////start_time
	tmpJObj = queryDto->get("start_time");
	objTicket.SetStartTime(Utf8ToUnicode(tmpJObj.toString()).c_str());

	////arrive_time
	tmpJObj = queryDto->get("arrive_time");
	objTicket.SetArriveTime(Utf8ToUnicode(tmpJObj.toString()).c_str());

	////day_difference
	tmpJObj = queryDto->get("day_difference");
	objTicket.SetDayDifferent(Utf8ToUnicode(tmpJObj.toString()).c_str());

	////train_class_name
	tmpJObj = queryDto->get("train_class_name");
	objTicket.SetTrainClassName(Utf8ToUnicode(tmpJObj.toString()).c_str());

	////lishi
	tmpJObj = queryDto->get("lishi");
	objTicket.SetLiShi(Utf8ToUnicode(tmpJObj.toString()).c_str());

	////canWebBuy
	tmpJObj = queryDto->get("canWebBuy");
	objTicket.SetCanWebBuy(Utf8ToUnicode(tmpJObj.toString()).c_str());

	////lishiValue
	tmpJObj = queryDto->get("lishiValue");
	objTicket.SetLiShiValue(Utf8ToUnicode(tmpJObj.toString()).c_str());

	////yp_info
	tmpJObj = queryDto->get("yp_info");
	objTicket.SetYpInfo(Utf8ToUnicode(tmpJObj.toString()).c_str());

	////control_train_day
	tmpJObj = queryDto->get("control_train_day");
	objTicket.SetControlTrainDay(Utf8ToUnicode(tmpJObj.toString()).c_str());

	////start_train_date
	tmpJObj = queryDto->get("start_train_date");
	objTicket.SetStartTrainDate(Utf8ToUnicode(tmpJObj.toString()).c_str());

	////seat_feature
	tmpJObj = queryDto->get("seat_feature");
	objTicket.SetSeatFeature(Utf8ToUnicode(tmpJObj.toString()).c_str());

	////yp_ex
	tmpJObj = queryDto->get("yp_ex");
	objTicket.SetYpEx(Utf8ToUnicode(tmpJObj.toString()).c_str());

	////train_seat_feature
	tmpJObj = queryDto->get("train_seat_feature");
	objTicket.SetTrainSeatFeatrue(Utf8ToUnicode(tmpJObj.toString()).c_str());

	////seat_types
	tmpJObj = queryDto->get("seat_types");
	objTicket.SetSeatTypes(Utf8ToUnicode(tmpJObj.toString()).c_str());

	////location_code
	tmpJObj = queryDto->get("location_code");
	objTicket.SetLocationCode(Utf8ToUnicode(tmpJObj.toString()).c_str());


	////from_station_no
	tmpJObj = queryDto->get("from_station_no");
	objTicket.SetFromStationNo(Utf8ToUnicode(tmpJObj.toString()).c_str());


	////to_station_no
	tmpJObj = queryDto->get("to_station_no");
	objTicket.SetToStationNo(Utf8ToUnicode(tmpJObj.toString()).c_str());

	////control_day
	tmpJObj = queryDto->get("control_day");
	objTicket.SetControlDay(Utf8ToUnicode(tmpJObj.toString()).c_str());

	////sale_time
	tmpJObj = queryDto->get("sale_time");
	objTicket.SetSaleTime(Utf8ToUnicode(tmpJObj.toString()).c_str());

	////is_support_card
	tmpJObj = queryDto->get("is_support_card");
	objTicket.SetIsSupportCard(Utf8ToUnicode(tmpJObj.toString()).c_str());


	////controlled_train_flag
	tmpJObj = queryDto->get("controlled_train_flag");
	objTicket.SetControlledTrainFlag(Utf8ToUnicode(tmpJObj.toString()).c_str());

	////controlled_train_message
	tmpJObj = queryDto->get("controlled_train_message");
	objTicket.SetControlledTrainMessage(Utf8ToUnicode(tmpJObj.toString()).c_str());

	////gg_num
	tmpJObj = queryDto->get("gg_num");
	objTicket.SetGgNum(Utf8ToUnicode(tmpJObj.toString()).c_str());

	////gr_num
	tmpJObj = queryDto->get("gr_num");
	objTicket.SetGrNum(Utf8ToUnicode(tmpJObj.toString()).c_str());

	////qt_num
	tmpJObj = queryDto->get("qt_num");
	objTicket.SetQtNum(Utf8ToUnicode(tmpJObj.toString()).c_str());

	////rw_num
	tmpJObj = queryDto->get("rw_num");
	objTicket.SetRwNum(Utf8ToUnicode(tmpJObj.toString()).c_str());

	////rz_num
	tmpJObj = queryDto->get("rz_num");
	objTicket.SetRzNum(Utf8ToUnicode(tmpJObj.toString()).c_str());

	////tz_num
	tmpJObj = queryDto->get("tz_num");
	objTicket.SetTzNum(Utf8ToUnicode(tmpJObj.toString()).c_str());

	////wz_num
	tmpJObj = queryDto->get("wz_num");
	objTicket.SetWzNum(Utf8ToUnicode(tmpJObj.toString()).c_str());

	////yb_num
	tmpJObj = queryDto->get("yb_num");
	objTicket.SetYbNum(Utf8ToUnicode(tmpJObj.toString()).c_str());

	////yw_num
	tmpJObj = queryDto->get("yw_num");
	objTicket.SetYwNum(Utf8ToUnicode(tmpJObj.toString()).c_str());


	////yz_num
	tmpJObj = queryDto->get("yz_num");
	objTicket.SetYzNum(Utf8ToUnicode(tmpJObj.toString()).c_str());

	////ze_num
	tmpJObj = queryDto->get("ze_num");
	objTicket.SetZeNum(Utf8ToUnicode(tmpJObj.toString()).c_str());

	////zy_num
	tmpJObj = queryDto->get("zy_num");
	objTicket.SetZyNum(Utf8ToUnicode(tmpJObj.toString()).c_str());

	////swz_num
	tmpJObj = queryDto->get("swz_num");
	objTicket.SetSwzNum(Utf8ToUnicode(tmpJObj.toString()).c_str());

	return SUCCESS;
}