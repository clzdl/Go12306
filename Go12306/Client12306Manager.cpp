
#include "stdafx.h"
#include "Client12306Manager.h"
#include <sstream>
#include <fstream>
#include "OthFunc.h"



Client12306Manager *Client12306Manager::m_objInstance = NULL;
SharedPtr<InvalidCertificateHandler> Client12306Manager::m_ptrCert = NULL;
Context::Ptr Client12306Manager::m_ptrContext = NULL;

std::string Client12306Manager::m_strDomain = "kyfw.12306.cn";


char* Client12306Manager::m_strMonAbbreviate[] = {"Jan" , "Feb" , "Mar", "Apr","May","Jun","Jul","Aug","Sep","Oct","Nov","Dec"};
char* Client12306Manager::m_strWeekAbbreviate[] = { "Mon","Tue","Web","Thu","Fri","Sat","Sun"};

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
	m_sessHttpsClient.setKeepAlive(true);

	m_headerDefault["Accept"] = "application/json, text/javascript,*/*; q=0.01";
	m_headerDefault["Accept-Encoding"] = "gzip, deflate, br";
	m_headerDefault["Accept-Language"] = "zh-CN,zh;q=0.8";
	m_headerDefault["Content-Type"] = "application/x-www-form-urlencoded;charset=UTF-8";
	m_headerDefault["Origin"] = "https://kyfw.12306.cn";
	m_headerDefault["X-Requested-With"] = "XMLHttpRequest";
	m_headerDefault["User-Agent"] = "Mozilla/5.0 (Windows NT 6.1; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/53.0.2785.101 Safari/537.36";
	m_headerDefault["Host"] = "kyfw.12306.cn";


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


std::string Client12306Manager::ExecPost(std::string service, std::map<string, string> *param, std::map<string, string> *header)
{
	m_strLastErrInfo.clear();
	HTTPRequest request(HTTPRequest::HTTP_POST, service , Net::HTTPMessage::HTTP_1_1);

	std::stringstream body;
	if (param && !(param->empty()))
	{
		
		for (std::map<string, string>::iterator it = param->begin(); it != param->end(); ++it)
		{

			if (it != param->begin())
				body << "&";

			std::string enStr;
			//URI::encode(it->second, "@,", enStr);

			UriEncode(it->second, enStr);

			body << it->first << "=" << enStr;
		}

		
	}

	std::string utf8Body =  Gbk2Utf8(body.str());

	request.setContentLength(utf8Body.length());
	if (!m_cookieCollection.empty())
		request.setCookies(m_cookieCollection);


	///增加默认全局默认header
	for (std::map<string, string>::iterator it = m_headerDefault.begin(); it != m_headerDefault.end(); ++it)
		request.set(it->first, it->second);
	
	
	if (header)
	{///增加特殊header
		for (std::map<string, string>::iterator it = header->begin(); it != header->end(); ++it)
		{
			request.set(it->first, it->second);
		}
	}

	request.setKeepAlive(true);

	{
		std::stringstream ss;
		request.write(ss);

		///请求头
		DUI__Trace(_T(" %s "), Utf8ToUnicode(ss.str()).c_str());
		///请求体
		DUI__Trace(_T(" %s "), Utf8ToUnicode(utf8Body).c_str());
	}

	std::ostream &os = m_sessHttpsClient.sendRequest(request);

	os << utf8Body;

	HTTPResponse response;

	std::istream& rs = m_sessHttpsClient.receiveResponse(response);

	std::ostringstream ostr;
	StreamCopier::copyStream(rs, ostr);

	{
		std::stringstream ss;
		response.write(ss);

		///输出相应头
		DUI__Trace(_T(" %s "), Utf8ToUnicode(ss.str()).c_str());
		
		///输出相应体
		DUI__Trace(_T(" %s "), Utf8ToUnicode(ostr.str()).c_str());
	}

	return ostr.str();
}


std::string Client12306Manager::ExecPostBySeq(std::string service, std::vector<CParam> *param , std::map<string, string> *header)
{
	m_strLastErrInfo.clear();
	HTTPRequest request(HTTPRequest::HTTP_POST, service, Net::HTTPMessage::HTTP_1_1);

	std::stringstream body;
	if (param && !(param->empty()))
	{

		for (std::vector<CParam>::iterator it = param->begin(); it != param->end(); ++it)
		{

			if (it != param->begin())
				body << "&";

			std::string enStr;

			if (it->GetNeedEncode())
			{
				//URI::encode(it->GetValue(), "@,", enStr);
				UriEncode(it->GetValue(), enStr);
			}
			else
				enStr = it->GetValue();

			body << it->GetKey() << "=" << enStr;
		}


	}

	std::string utf8Body = body.str();

	request.setContentLength(utf8Body.length());
	if (!m_cookieCollection.empty())
		request.setCookies(m_cookieCollection);


	///增加默认全局默认header
	for (std::map<string, string>::iterator it = m_headerDefault.begin(); it != m_headerDefault.end(); ++it)
		request.set(it->first, it->second);


	if (header)
	{///增加特殊header
		for (std::map<string, string>::iterator it = header->begin(); it != header->end(); ++it)
		{
			request.set(it->first, it->second);
		}
	}

	request.setKeepAlive(true);

	{
		std::stringstream ss;
		request.write(ss);

		///请求头
		DUI__Trace(_T(" %s "), Utf8ToUnicode(ss.str()).c_str());
		///请求体
		DUI__Trace(_T(" %s "), Utf8ToUnicode(utf8Body).c_str());
	}

	std::ostream &os = m_sessHttpsClient.sendRequest(request);

	os << utf8Body;

	HTTPResponse response;

	std::istream& rs = m_sessHttpsClient.receiveResponse(response);

	std::ostringstream ostr;
	StreamCopier::copyStream(rs, ostr);

	{
		std::stringstream ss;
		response.write(ss);

		///输出相应头
		DUI__Trace(_T(" %s "), Utf8ToUnicode(ss.str()).c_str());

		///输出相应体
		DUI__Trace(_T(" %s "), Utf8ToUnicode(ostr.str()).c_str());
	}

	return ostr.str();
}

std::string Client12306Manager::ExecGet(std::string service, std::map<string, string> *param,std::map<string, string> *header)
{
	m_strLastErrInfo.clear();
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

	HTTPRequest request(HTTPRequest::HTTP_GET, service, Net::HTTPMessage::HTTP_1_1);

	if (!m_cookieCollection.empty())
		request.setCookies(m_cookieCollection);


	request.setKeepAlive(true);


	///增加默认全局默认header
	for (std::map<string, string>::iterator it = m_headerDefault.begin(); it != m_headerDefault.end(); ++it)
		request.set(it->first, it->second);

	if (header)
	{
		///增加特殊header
		for (std::map<string, string>::iterator it = header->begin(); it != header->end(); ++it)
		{
			request.set(it->first, it->second);
		}
	}
	{
		std::stringstream ss;
		request.write(ss);

		///请求头
		DUI__Trace(_T(" %s "), Utf8ToUnicode(ss.str()).c_str());
	}

	m_sessHttpsClient.sendRequest(request);
	HTTPResponse response;

	std::istream& rs = m_sessHttpsClient.receiveResponse(response);
	
	if (m_cookieCollection.empty())
	{
		std::vector<HTTPCookie> cookies;
		response.getCookies(cookies);

		for (std::vector<HTTPCookie>::iterator it = cookies.begin(); it != cookies.end(); ++it)
		{
			m_cookieCollection.add(it->getName(), it->getValue());
		}

	}

	std::ostringstream ostr;
	StreamCopier::copyStream(rs, ostr);

	{
		std::stringstream ss;
		response.write(ss);

		///输出相应头
		DUI__Trace(_T(" %s "), Utf8ToUnicode(ss.str()).c_str());

		///输出相应体
		DUI__Trace(_T(" %s "), Utf8ToUnicode(ostr.str()).c_str());
	}

	return ostr.str();
	
}

int Client12306Manager::QueryTicketLog(std::string begPlace, std::string endPlace, std::string travelTime, bool &flag, _TICKET_TYPE ticketType)
{
	flag = false;
	try
	{

		string strService = "/otn/leftTicket/log?";

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
		switch (ticketType)
		{
		case _ADULT:
		default:
			strService += "ADULT";
			break;
		}


		std::string strOrgRes = ExecGet(strService);
	
		do
		{
			JSON::Parser parser;
			Dynamic::Var result;

			result = parser.parse(strOrgRes);

			JSON::Object::Ptr pObj = result.extract<JSON::Object::Ptr>();

			Dynamic::Var jStatus = pObj->get("status");

			if (jStatus.toString() != "true" && jStatus.toString() != "TRUE")
				break;

			flag = true;

		} while (false);

	}
	catch (Poco::Exception &e)
	{
		DUI__Trace(_T("%s\n"), Utf8ToUnicode(e.displayText()).c_str());
		return FAIL;
	}

	return SUCCESS;
}


int Client12306Manager::QueryLeftTicket(std::string begPlace, std::string endPlace, std::string travelTime , std::vector<CTicketModel> &vecTicket, _TICKET_TYPE ticketType)
{
	int iRet = SUCCESS;

	/*bool bValid = false;
	QueryTicketLog(begPlace, endPlace, travelTime, bValid, ticketType);


	if (!bValid)
		return FAIL;*/

	try
	{

		string strService = "/otn/leftTicket/queryA?";

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
		case _STUDENT:
			strService += "0X00";
			break;
		case _ADULT:
		default:
			strService += "ADULT";
			break;
		}
		

		std::string strOrgRes = ExecGet(strService);

		if (strOrgRes.empty())
		{
			m_strLastErrInfo = Gbk2Utf8("12306返回失败.");
			return FAIL;
		}

		std::string strGunString;
		Gunzip((byte*)const_cast<char*>(strOrgRes.c_str()), strOrgRes.length() , strGunString);

		iRet = JsonParseTicket(strGunString , travelTime , ticketType , vecTicket);
		

	}
	catch (Poco::Exception &e)
	{
		DUI__Trace(_T("%s\n"), Utf8ToUnicode(e.displayText()).c_str());
		m_strLastErrInfo = Gbk2Utf8("结果返回错误");
		return FAIL;
	}

	return iRet;
}


int Client12306Manager::JsonParseTicket(std::string jsonString , std::string travelTime , _TICKET_TYPE ticketType , std::vector<CTicketModel> &vecTicket)
{
	JSON::Parser parser;
	Dynamic::Var result;

	try
	{

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


		if (0 > pArry->size())
		{
			m_strLastErrInfo = Gbk2Utf8("无余票信息");
			return FAIL;
		}

		JSON::Array::ConstIterator it = pArry->begin();
		//把数组里的所有内容打印出来
		//当然也可以把每个对象拿出来用。
		for (; it != pArry->end(); it++)
		{
			DUI__Trace(Utf8ToUnicode(it->toString()).c_str());

			JSON::Object::Ptr pItem = it->extract<JSON::Object::Ptr>();

			CTicketModel ticketModel;

			ticketModel.SetTrainDate(Utf8ToUnicode(travelTime).c_str());
			ticketModel.SetTicketType(ticketType);

			Dynamic::Var tmpJObj;
			////buttonTextInfo
			tmpJObj = pItem->get("buttonTextInfo");
			ticketModel.SetBtnTextInfo(Utf8ToUnicode(tmpJObj.toString()).c_str());

			/////secretStr
			tmpJObj = pItem->get("secretStr");
			ticketModel.SetSecretStr(Utf8ToUnicode(tmpJObj.toString()).c_str());


			////queryLeftNewDTO
			tmpJObj = pItem->get("queryLeftNewDTO");

			JSON::Object::Ptr pQueryDto = tmpJObj.extract<JSON::Object::Ptr>();


			AssignJson2TicketObj(pQueryDto, ticketModel);

			vecTicket.emplace_back(ticketModel);

		}
	}
	catch (Poco::Exception &e)
	{
		DUI__Trace(_T("%s\n"), Utf8ToUnicode(e.displayText()).c_str());
		m_strLastErrInfo = Gbk2Utf8("结果返回错误");
		return FAIL;
	}

	return SUCCESS;
}


int Client12306Manager::AssignJson2TicketObj(JSON::Object::Ptr queryDto, CTicketModel &objTicket)
{
	Dynamic::Var tmpJObj;
	/////train_no
	tmpJObj = queryDto->get("train_no");
	if (!tmpJObj.isEmpty())
		objTicket.SetTrainNo(Utf8ToUnicode(tmpJObj.toString()).c_str());

	////station_train_code
	tmpJObj = queryDto->get("station_train_code");
	if (!tmpJObj.isEmpty())
		objTicket.SetStationTrainCode(Utf8ToUnicode(tmpJObj.toString()).c_str());

	////start_station_telecode
	tmpJObj = queryDto->get("start_station_telecode");
	if (!tmpJObj.isEmpty())
		objTicket.SetStartStationTelecode(Utf8ToUnicode(tmpJObj.toString()).c_str());


	////start_station_name
	tmpJObj = queryDto->get("start_station_name");
	if (!tmpJObj.isEmpty())
		objTicket.SetStartStationName(Utf8ToUnicode(tmpJObj.toString()).c_str());
	
	////end_station_telecode
	tmpJObj = queryDto->get("end_station_telecode");
	if (!tmpJObj.isEmpty())
		objTicket.SetEndStationTelecode(Utf8ToUnicode(tmpJObj.toString()).c_str());

	////end_station_name	
	tmpJObj = queryDto->get("end_station_name");
	if (!tmpJObj.isEmpty())
		objTicket.SetEndStationName(Utf8ToUnicode(tmpJObj.toString()).c_str());


	////from_station_telecode
	tmpJObj = queryDto->get("from_station_telecode");
	if (!tmpJObj.isEmpty())
		objTicket.SetFromStationTelecode(Utf8ToUnicode(tmpJObj.toString()).c_str());

	////from_station_name
	tmpJObj = queryDto->get("from_station_name");
	if (!tmpJObj.isEmpty())
		objTicket.SetFromStationName(Utf8ToUnicode(tmpJObj.toString()).c_str());

	////to_station_telecode
	tmpJObj = queryDto->get("to_station_telecode");
	if (!tmpJObj.isEmpty())
		objTicket.SetToStationTelecode(Utf8ToUnicode(tmpJObj.toString()).c_str());

	////to_station_name
	tmpJObj = queryDto->get("to_station_name");
	if (!tmpJObj.isEmpty())
		objTicket.SetToStationName(Utf8ToUnicode(tmpJObj.toString()).c_str());

	////start_time
	tmpJObj = queryDto->get("start_time");
	if (!tmpJObj.isEmpty())
		objTicket.SetStartTime(Utf8ToUnicode(tmpJObj.toString()).c_str());

	////arrive_time
	tmpJObj = queryDto->get("arrive_time");
	if (!tmpJObj.isEmpty())
		objTicket.SetArriveTime(Utf8ToUnicode(tmpJObj.toString()).c_str());

	////day_difference
	tmpJObj = queryDto->get("day_difference");
	if (!tmpJObj.isEmpty())
		objTicket.SetDayDifferent(Utf8ToUnicode(tmpJObj.toString()).c_str());

	////train_class_name
	tmpJObj = queryDto->get("train_class_name");
	if (!tmpJObj.isEmpty())
		objTicket.SetTrainClassName(Utf8ToUnicode(tmpJObj.toString()).c_str());

	////lishi
	tmpJObj = queryDto->get("lishi");
	if (!tmpJObj.isEmpty())
		objTicket.SetLiShi(Utf8ToUnicode(tmpJObj.toString()).c_str());

	////canWebBuy
	tmpJObj = queryDto->get("canWebBuy");
	if (!tmpJObj.isEmpty())
		objTicket.SetCanWebBuy(Utf8ToUnicode(tmpJObj.toString()).c_str());

	////lishiValue
	tmpJObj = queryDto->get("lishiValue");
	if (!tmpJObj.isEmpty())
		objTicket.SetLiShiValue(Utf8ToUnicode(tmpJObj.toString()).c_str());

	////yp_info
	tmpJObj = queryDto->get("yp_info");
	if (!tmpJObj.isEmpty())
		objTicket.SetYpInfo(Utf8ToUnicode(tmpJObj.toString()).c_str());

	////control_train_day
	tmpJObj = queryDto->get("control_train_day");
	if (!tmpJObj.isEmpty())
		objTicket.SetControlTrainDay(Utf8ToUnicode(tmpJObj.toString()).c_str());

	////start_train_date
	tmpJObj = queryDto->get("start_train_date");
	if (!tmpJObj.isEmpty())
		objTicket.SetStartTrainDate(Utf8ToUnicode(tmpJObj.toString()).c_str());

	////seat_feature
	tmpJObj = queryDto->get("seat_feature");
	if (!tmpJObj.isEmpty())
		objTicket.SetSeatFeature(Utf8ToUnicode(tmpJObj.toString()).c_str());

	////yp_ex
	tmpJObj = queryDto->get("yp_ex");
	if (!tmpJObj.isEmpty())
		objTicket.SetYpEx(Utf8ToUnicode(tmpJObj.toString()).c_str());

	////train_seat_feature
	tmpJObj = queryDto->get("train_seat_feature");
	if (!tmpJObj.isEmpty())
		objTicket.SetTrainSeatFeatrue(Utf8ToUnicode(tmpJObj.toString()).c_str());

	////seat_types
	tmpJObj = queryDto->get("seat_types");
	if (!tmpJObj.isEmpty())
		objTicket.SetSeatTypes(Utf8ToUnicode(tmpJObj.toString()).c_str());

	////location_code
	tmpJObj = queryDto->get("location_code");
	if (!tmpJObj.isEmpty())
		objTicket.SetLocationCode(Utf8ToUnicode(tmpJObj.toString()).c_str());


	////from_station_no
	tmpJObj = queryDto->get("from_station_no");
	if (!tmpJObj.isEmpty())
		objTicket.SetFromStationNo(Utf8ToUnicode(tmpJObj.toString()).c_str());


	////to_station_no
	tmpJObj = queryDto->get("to_station_no");
	if (!tmpJObj.isEmpty())
		objTicket.SetToStationNo(Utf8ToUnicode(tmpJObj.toString()).c_str());

	////control_day
	tmpJObj = queryDto->get("control_day");
	if (!tmpJObj.isEmpty())
		objTicket.SetControlDay(Utf8ToUnicode(tmpJObj.toString()).c_str());

	////sale_time
	tmpJObj = queryDto->get("sale_time");
	if (!tmpJObj.isEmpty())
		objTicket.SetSaleTime(Utf8ToUnicode(tmpJObj.toString()).c_str());

	////is_support_card
	tmpJObj = queryDto->get("is_support_card");
	if (!tmpJObj.isEmpty())
		objTicket.SetIsSupportCard(Utf8ToUnicode(tmpJObj.toString()).c_str());


	////controlled_train_flag
	tmpJObj = queryDto->get("controlled_train_flag");
	if (!tmpJObj.isEmpty())
		objTicket.SetControlledTrainFlag(Utf8ToUnicode(tmpJObj.toString()).c_str());

	////controlled_train_message
	tmpJObj = queryDto->get("controlled_train_message");
	if (!tmpJObj.isEmpty())
		objTicket.SetControlledTrainMessage(Utf8ToUnicode(tmpJObj.toString()).c_str());

	////gg_num
	tmpJObj = queryDto->get("gg_num");
	if (!tmpJObj.isEmpty())
		objTicket.SetGgNum(Utf8ToUnicode(tmpJObj.toString()).c_str());

	////gr_num
	tmpJObj = queryDto->get("gr_num");
	if (!tmpJObj.isEmpty())
		objTicket.SetGrNum(Utf8ToUnicode(tmpJObj.toString()).c_str());

	////qt_num
	tmpJObj = queryDto->get("qt_num");
	if (!tmpJObj.isEmpty())
		objTicket.SetQtNum(Utf8ToUnicode(tmpJObj.toString()).c_str());

	////rw_num
	tmpJObj = queryDto->get("rw_num");
	if (!tmpJObj.isEmpty())
		objTicket.SetRwNum(Utf8ToUnicode(tmpJObj.toString()).c_str());

	////rz_num
	tmpJObj = queryDto->get("rz_num");
	if (!tmpJObj.isEmpty())
		objTicket.SetRzNum(Utf8ToUnicode(tmpJObj.toString()).c_str());

	////tz_num
	tmpJObj = queryDto->get("tz_num");
	if (!tmpJObj.isEmpty())
		objTicket.SetTzNum(Utf8ToUnicode(tmpJObj.toString()).c_str());

	////wz_num
	tmpJObj = queryDto->get("wz_num");
	if (!tmpJObj.isEmpty())
		objTicket.SetWzNum(Utf8ToUnicode(tmpJObj.toString()).c_str());

	////yb_num
	tmpJObj = queryDto->get("yb_num");
	if (!tmpJObj.isEmpty())
		objTicket.SetYbNum(Utf8ToUnicode(tmpJObj.toString()).c_str());

	////yw_num
	tmpJObj = queryDto->get("yw_num");
	if (!tmpJObj.isEmpty())
		objTicket.SetYwNum(Utf8ToUnicode(tmpJObj.toString()).c_str());


	////yz_num
	tmpJObj = queryDto->get("yz_num");
	if (!tmpJObj.isEmpty())
		objTicket.SetYzNum(Utf8ToUnicode(tmpJObj.toString()).c_str());

	////ze_num
	tmpJObj = queryDto->get("ze_num");
	if (!tmpJObj.isEmpty())
		objTicket.SetZeNum(Utf8ToUnicode(tmpJObj.toString()).c_str());

	////zy_num
	tmpJObj = queryDto->get("zy_num");
	if (!tmpJObj.isEmpty())
		objTicket.SetZyNum(Utf8ToUnicode(tmpJObj.toString()).c_str());

	////swz_num
	tmpJObj = queryDto->get("swz_num");
	if (!tmpJObj.isEmpty())
		objTicket.SetSwzNum(Utf8ToUnicode(tmpJObj.toString()).c_str());

	tmpJObj = queryDto->get("location_code");
	if (!tmpJObj.isEmpty())
		objTicket.SetTrainLocation(Utf8ToUnicode(tmpJObj.toString()).c_str());

	return SUCCESS;
}
int Client12306Manager::LoginInit()
{
	try
	{

		string strService = "/otn/login/init";


		std::map<string, string> header;
		header["Referer"] = "https://kyfw.12306.cn/otn/login/init";

		ExecGet(strService, NULL, &header);


	}
	catch (Poco::Exception &e)
	{
		DUI__Trace(_T("%s\n"), Utf8ToUnicode(e.displayText()).c_str());
		return FAIL;
	}
	return SUCCESS;
}

int Client12306Manager::QueryPassCode(std::string moduleName , std::string &bytes)
{
	try
	{

		string strService = "/otn/passcodeNew/getPassCodeNew?";

		///module
		strService += "module=";
		strService += moduleName;
		strService += "&";

		////rand
		strService += "rand=";
		strService += "sjrand";
		//strService += "&";

		//////value
		//strService += "0.06173759602765039";

		std::map<string, string> header;
		header["Referer"] = "https://kyfw.12306.cn/otn/login/init";
	

		bytes = ExecGet(strService , NULL , &header);

	
	}
	catch (Poco::Exception &e)
	{
		DUI__Trace(_T("%s\n"), Utf8ToUnicode(e.displayText()).c_str());
		return FAIL;
	}
	return SUCCESS;
}


int Client12306Manager::AnsynValidPassCode(std::vector<CDuiPoint> &selPoints , std::string &res)
{
	try
	{

		string strService = "/otn/passcodeNew/checkRandCodeAnsyn";
		std::map<string, string> param;

		param["rand"] = "sjrand";

		char randCode[128] = { 0 };
		for (std::vector<CDuiPoint>::iterator it = selPoints.begin(); it != selPoints.end(); ++it)
		{
			if(it == selPoints.begin())
				sprintf(randCode , "%d,%d" , it->x , it->y);
			else
				sprintf(randCode, "%s,%d,%d", randCode, it->x, it->y);
		}
		param["randCode"] = randCode;

		std::map<string, string> header;
		header["Referer"] = "https://kyfw.12306.cn/otn/login/init";
		

		res = ExecPost(strService , &param, &header);

	}
	catch (Poco::Exception &e)
	{
		DUI__Trace(_T("%s\n"), Utf8ToUnicode(e.displayText()).c_str());
		return FAIL;
	}

	return SUCCESS;
}

int Client12306Manager::AnsynValidPassCode(std::vector<CDuiPoint> &selPoints, bool &flag)
{
	std::string response;
	
	flag = false;

	do
	{
		if (SUCCESS != AnsynValidPassCode(selPoints, response))
			break;


		JSON::Parser parser;
		Dynamic::Var result;

		result = parser.parse(response);

		JSON::Object::Ptr pObj = result.extract<JSON::Object::Ptr>();

		Dynamic::Var jStatus = pObj->get("status");

		if (jStatus.toString() != "true")
			break;
		

		Dynamic::Var jData = pObj->get("data");

		JSON::Object::Ptr pMsg = jData.extract<JSON::Object::Ptr>();

		if (pMsg->get("msg").toString() != "true" && 
					pMsg->get("msg").toString() != "TRUE")
			break;


		flag = true;


	} while (false);

	return SUCCESS;
}


int Client12306Manager::AnsysLoginSugguest(std::string userName, std::string userPass, std::string randCode , std::string &res)
{
	try
	{

		string strService = "/otn/login/loginAysnSuggest";
		std::map<string, string> param;

		param["loginUserDTO.user_name"] = userName;
		param["userDTO.password"] = userPass;
		param["randCode"] = randCode;

		std::map<string, string> header;
		header["Referer"] = "https://kyfw.12306.cn/otn/login/init";
	

		res = ExecPost(strService, &param , &header);

	}
	catch (Poco::Exception &e)
	{
		DUI__Trace(_T("%s\n"), Utf8ToUnicode(e.displayText()).c_str());

		m_strLastErrInfo = e.displayText();

		return FAIL;
	}

	return SUCCESS;
}

int Client12306Manager::AnsysLoginSugguest(std::string userName, std::string userPass, std::string randCode, bool &flag)
{
	std::string response;
	flag = false;
	do
	{
		if (SUCCESS != AnsysLoginSugguest(userName, userPass, randCode, response))
			break;

		//response = Utf8ToGbk(response);

		JSON::Parser parser;
		Dynamic::Var result;

		result = parser.parse(response);

		JSON::Object::Ptr pObj = result.extract<JSON::Object::Ptr>();

		Dynamic::Var jStatus = pObj->get("status");

		if (jStatus.toString() != "true")
			break;


		Dynamic::Var jData = pObj->get("data");

		JSON::Object::Ptr pData = jData.extract<JSON::Object::Ptr>();


		if (!pData->has("loginCheck"))
		{
			///获取message array
			JSON::Array::Ptr jMsg = pObj->getArray("messages");

			Dynamic::Var msg = jMsg->get(0);
			
			m_strLastErrInfo = msg.toString();
		
			break;
		}


		if (pData->get("loginCheck").toString() != "Y" &&
						pData->get("loginCheck").toString() != "y")
		{
			///获取message array
			JSON::Array::Ptr jMsg = pObj->getArray("messages");

			Dynamic::Var msg = jMsg->get(0);

			m_strLastErrInfo = msg.toString();

			break;
		}


		flag = true;

	} while (false);

	return SUCCESS;
}

int Client12306Manager::UserLogin(std::string &res)
{
	try
	{

		string strService = "/otn/login/userLogin";
		std::map<string, string> param;

		param["_json_att"] = "";
		
		std::map<string, string> header;
		header["Referer"] = "https://kyfw.12306.cn/otn/login/init";

		res = ExecPost(strService, &param,&header);

	}
	catch (Poco::Exception &e)
	{
		DUI__Trace(_T("%s\n"), Utf8ToUnicode(e.displayText()).c_str());
		return FAIL;
	}

	return SUCCESS;
}

int Client12306Manager::InitMy12306(std::string &res)
{
	try
	{

		string strService = "/otn/index/initMy12306";
		std::map<string, string> header;
		header["Referer"] = "https://kyfw.12306.cn/otn/login/init";

		std::string response = ExecGet(strService , NULL , &header);

		Gunzip((Byte*)const_cast<char*>(response.c_str()), response.length(), res);

		DUI__Trace(_T("%s"), Utf8ToUnicode(res).c_str());
		
	}
	catch (Poco::Exception &e)
	{
		DUI__Trace(_T("%s"), Utf8ToUnicode(e.displayText()).c_str());
		return FAIL;
	}

	return SUCCESS;
}


int Client12306Manager::QueryMyOrder(std::string startDate, std::string endDate , std::string type, std::string seqTrainName,std::map<string, COrderModel> &mapOrder)
{
	int iRetFlag = SUCCESS;
	try
	{

		string strService = "/otn/queryOrder/queryMyOrder";

		std::map<string, string> header;
		header["Referer"] = "https://kyfw.12306.cn/otn/queryOrder/init";


		std::map<string, string> param;
		/// 1: 订单日期  ， 2： 乘车日期
		param["queryType"] = type;
		param["queryStartDate"] = startDate;
		param["queryEndDate"] = endDate;
		param["come_from_flag"] = "my_order";
		param["pageSize"] = "8888";
		param["pageIndex"] = "0";
		param["query_where"] = "G";
		param["sequeue_train_name"] = seqTrainName;
		
		


		std::string response = ExecPost(strService, &param, &header);
		std::string gunRes;
		Gunzip((Byte*)const_cast<char*>(response.c_str()), response.length(), gunRes);
		
		iRetFlag = JsonParseOrder(gunRes, mapOrder);
	}
	catch (Poco::Exception &e)
	{
		DUI__Trace(_T("%s"), Utf8ToUnicode(e.displayText()).c_str());
		return FAIL;
	}

	return iRetFlag;
}


int Client12306Manager::JsonParseOrder(std::string jsonString, std::map<std::string, COrderModel> &mapOrder)
{
	try
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

		if (pObj->has("url"))
		{
			if (pObj->has("messages"))
			{
				Dynamic::Var jMsg = pObj->get("messages");
				m_strLastErrInfo = jMsg.toString();
			}

			return FAIL;
		}


		////data object
		Dynamic::Var pData = pObj->get("data");

		JSON::Object::Ptr jData = pData.extract<JSON::Object::Ptr>();

		////订单个数
		int cnt = atoi(jData->get("order_total_number").toString().c_str());

		JSON::Array::Ptr jOrderDTODataList = jData->getArray("OrderDTODataList");


		for (int i = 0; i < cnt; ++i)
		{

			Dynamic::Var pOrderDTOData = jOrderDTODataList->get(i);

			DUI__Trace(Utf8ToUnicode(pOrderDTOData.toString()).c_str());

			JSON::Object::Ptr jOrderDTOData = pOrderDTOData.extract<JSON::Object::Ptr>();

			COrderModel objOrder;
			AssignJson2OrderObj(jOrderDTOData, objOrder);

			

			mapOrder.insert(std::pair<std::string, COrderModel>(UnicodeToUtf8(objOrder.GetOrderNo().GetData()), objOrder));

		}
	}
	catch (Exception &e)
	{
		m_strLastErrInfo = e.displayText();
		return FAIL;
	}

	return SUCCESS;
}

int Client12306Manager::AssignJson2OrderObj(JSON::Object::Ptr jOrderDTOData, COrderModel &objOrder)
{
	try
	{
		///订单号
		objOrder.SetOrderNo(Utf8ToUnicode(jOrderDTOData->get("sequence_no").toString()).c_str() );

		///订单日期
		objOrder.SetOrderDate(Utf8ToUnicode(jOrderDTOData->get("order_date").toString()).c_str());

		///旅客名称
		JSON::Array::Ptr jArrayPassengerNames =  jOrderDTOData->getArray("array_passser_name_page");

		objOrder.SetPassengerName(Utf8ToUnicode(jArrayPassengerNames->get(0).toString()).c_str());

		///乘车站
		JSON::Array::Ptr jArrayFromStation = jOrderDTOData->getArray("from_station_name_page");
		objOrder.SetFromStation(Utf8ToUnicode(jArrayFromStation->get(0).toString()).c_str());

		///下车站
		JSON::Array::Ptr jArrayToStation = jOrderDTOData->getArray("to_station_name_page");
		objOrder.SetToStateion(Utf8ToUnicode(jArrayToStation->get(0).toString()).c_str());

		///发车时间
		objOrder.SetTravelDate(Utf8ToUnicode(jOrderDTOData->get("start_train_date_page").toString()).c_str());

		///车次
		objOrder.SetTrainCode(Utf8ToUnicode(jOrderDTOData->get("train_code_page").toString()).c_str());


		///
		JSON::Array::Ptr jOrderTickets = jOrderDTOData->getArray("tickets");
		JSON::Array::ConstIterator it = jOrderTickets->begin();

		for (; it != jOrderTickets->end(); ++it)
		{
			JSON::Object::Ptr jOrderTicket = it->extract<JSON::Object::Ptr>();

			COrderTicketModel orderTicketModel(&objOrder);
			if (SUCCESS != AssignJson2OrderTicketObj(jOrderTicket, orderTicketModel))
				return FAIL;

			objOrder.Add(orderTicketModel);

		}


	}
	catch (Exception &e)
	{
		m_strLastErrInfo = e.displayText();
		return FAIL;
	}
	
	return SUCCESS;
}


int Client12306Manager::AssignJson2OrderTicketObj(JSON::Object::Ptr jOrderTicketDTOData, COrderTicketModel &objOrderTicket)
{
	try
	{
	
		///票号
		objOrderTicket.SetTicketNo(Utf8ToUnicode(jOrderTicketDTOData->get("ticket_no").toString()).c_str());

		///车厢
		objOrderTicket.SetCoachName(Utf8ToUnicode(jOrderTicketDTOData->get("coach_name").toString()).c_str());

		///座位
		objOrderTicket.SetSeatName(Utf8ToUnicode(jOrderTicketDTOData->get("seat_name").toString()).c_str());

		///座位类型
		objOrderTicket.SetSeatTypeName(Utf8ToUnicode(jOrderTicketDTOData->get("seat_type_name").toString()).c_str());

		///票类型
		objOrderTicket.SetTicketTypeName(Utf8ToUnicode(jOrderTicketDTOData->get("ticket_type_name").toString()).c_str());

		///票价格
		objOrderTicket.SetTicketTypeName(Utf8ToUnicode(jOrderTicketDTOData->get("str_ticket_price_page").toString()).c_str());


		///付款状态
		objOrderTicket.SetTicketPayStatus(Utf8ToUnicode(jOrderTicketDTOData->get("ticket_status_name").toString()).c_str());


		////passengerDTO
		Dynamic::Var pPassengerDto = jOrderTicketDTOData->get("passengerDTO");

		JSON::Object::Ptr jPassengerDto = pPassengerDto.extract<JSON::Object::Ptr>();


		////乘车人
		objOrderTicket.SetPassengerPersonName(Utf8ToUnicode(jPassengerDto->get("passenger_name").toString()).c_str());

		////乘车证件
		objOrderTicket.SetPassengerIDTypeName(Utf8ToUnicode(jPassengerDto->get("passenger_id_type_name").toString()).c_str());
	}
	catch (Exception &e)
	{
		m_strLastErrInfo = e.displayText();
		return FAIL;
	}

	return SUCCESS;
}


static bool CmpStationBySN(const CStation *s1, const CStation *s2) 
{
	return s1->GetShortName().compare(0, s2->GetShortName().length(), s2->GetShortName()) < 0;
}

static bool CmpStationByPY(const CStation *s1, const CStation *s2) 
{
	return s1->GetPinYinName().compare(0, s2->GetPinYinName().length(), s2->GetPinYinName()) < 0 ;
}


static bool CmpStationBySEQ(const CStation *s1, const CStation *s2)
{
	return s1->GetSeq() < s2->GetSeq();
}
int Client12306Manager::Query12306StationName()
{
	int iRetFlag = SUCCESS;
	try
	{

		string strService = "/otn/resources/js/framework/station_name.js";

		std::string response = ExecGet(strService);
	

		ParseStationString(response);

		/*for (std::map<std::string, CStation>::iterator it = m_mapStation.begin(); it != m_mapStation.end(); ++it)
		{
			DUI__Trace(_T("%s,%s,%s,%s"), Utf8ToUnicode(it->second.GetStationCode()).c_str(),
											Utf8ToUnicode(it->second.GetShortName()).c_str(),
											Utf8ToUnicode(it->second.GetPinYinName()).c_str(),
											Utf8ToUnicode(it->second.GetChinaName()).c_str());
		}*/


		///
		std::sort(m_vecStationBySNSort.begin(), m_vecStationBySNSort.end(), CmpStationBySN);
		std::sort(m_vecStationByPYSort.begin(), m_vecStationByPYSort.end(), CmpStationByPY);

		/*for (std::vector<CStation*>::iterator it = m_vecStationBySNSort.begin(); it != m_vecStationBySNSort.end(); ++it)
		{
			DUI__Trace(_T("%s,%s,%s,%s"), Utf8ToUnicode((*it)->GetStationCode()).c_str(),
				Utf8ToUnicode((*it)->GetShortName()).c_str(),
				Utf8ToUnicode((*it)->GetPinYinName()).c_str(),
				Utf8ToUnicode((*it)->GetChinaName()).c_str());
		}*/


		for (std::vector<CStation*>::iterator it = m_vecStationByPYSort.begin(); it != m_vecStationByPYSort.end(); ++it)
		{
			DUI__Trace(_T("%s,%s,%s,%s"), Utf8ToUnicode((*it)->GetStationCode()).c_str(),
				Utf8ToUnicode((*it)->GetShortName()).c_str(),
				Utf8ToUnicode((*it)->GetPinYinName()).c_str(),
				Utf8ToUnicode((*it)->GetChinaName()).c_str());
		}

	}
	catch (Poco::Exception &e)
	{
		DUI__Trace(_T("%s"), Utf8ToUnicode(e.displayText()).c_str());
		return FAIL;
	}

	return iRetFlag;
}

int Client12306Manager::ParseStationString(std::string res)
{
	int begPos = res.find("@")+1;

	int endPos = 0;
	while ((endPos = res.find("@", begPos )) != string::npos)
	{
		std::string info = res.substr(begPos, endPos - begPos );
		CStation station;
		////
		int iBeg = 0;
		int iEnd = 0;
		iEnd = info.find("|");


		///汉字
		iBeg = iEnd + 1;
		iEnd = info.find("|" , iBeg);
		station.SetChinaName(info.substr(iBeg,iEnd - iBeg));

		///站点编码
		iBeg = iEnd + 1;
		iEnd = info.find("|", iBeg);
		station.SetStationCode(info.substr(iBeg,iEnd - iBeg));

		///汉字拼音
		iBeg = iEnd + 1;
		iEnd = info.find("|", iBeg);
		station.SetPinYinName(info.substr(iBeg,iEnd - iBeg));


		//////拼音缩写
		iBeg = iEnd + 1;
		iEnd = info.find("|", iBeg);
		station.SetShortName(info.substr(iBeg, iEnd - iBeg));

		///序号
		iBeg = iEnd + 1;
		station.SetSeq(atoi(info.substr(iBeg).c_str()));

		std::pair<std::map<std::string, CStation>::iterator, bool> retPair;
		retPair = m_mapStation.insert(std::pair<std::string, CStation>(station.GetStationCode(), station));

		m_vecStationBySNSort.push_back(&(retPair.first->second));

		m_vecStationByPYSort.push_back(&(retPair.first->second));

		begPos = endPos + 1;
	}

	std::string info = res.substr(begPos);
	CStation station;
	///
	int iBeg = 0;
	int iEnd = 0;
	iEnd = info.find("|");
	

	///汉字
	iBeg = iEnd + 1;
	iEnd = info.find("|", iBeg);
	station.SetChinaName(info.substr(iBeg,iEnd - iBeg ));

	///站点编码
	iBeg = iEnd + 1;
	iEnd = info.find("|", iBeg);
	station.SetStationCode(info.substr(iBeg,iEnd - iBeg ));

	///汉字拼音
	iBeg = iEnd + 1;
	iEnd = info.find("|", iBeg);
	station.SetPinYinName(info.substr(iBeg,iEnd - iBeg ));

	////拼音缩写
	iBeg = iEnd + 1;
	iEnd = info.find("|", iBeg);
	station.SetShortName(info.substr(iBeg, iEnd - iBeg));


	///序号
	iBeg = iEnd + 1;
	station.SetSeq(atoi(info.substr(iBeg).c_str()));

	std::pair<std::map<std::string, CStation>::iterator, bool> retPair;
	retPair = m_mapStation.insert(std::pair<std::string, CStation>(station.GetStationCode(), station));

	m_vecStationBySNSort.push_back(&(retPair.first->second));

	m_vecStationByPYSort.push_back(&(retPair.first->second));
	

	return SUCCESS;
}



static bool CmpStationBySN01(const CStation *s1, const CStation *s2)
{
	return s1->GetShortName().compare(0, s2->GetShortName().length(), s2->GetShortName() , 0 , s2->GetShortName().length()) < 0;
}

static bool CmpStationByPY01(const CStation *s1, const CStation *s2)
{
	return s1->GetPinYinName().compare(0, s2->GetPinYinName().length(), s2->GetPinYinName() , 0 , s2->GetPinYinName().length()) < 0;
}


std::vector<CStation*> Client12306Manager::GetStation(std::string vPrefix)
{
	std::vector<CStation*> vecRes;

	CStation sa;
	sa.SetPinYinName(vPrefix);
	sa.SetShortName(vPrefix);


	std::vector<CStation*>::iterator lowerIt;
	
	std::set<std::string> setString;
	lowerIt = std::lower_bound(m_vecStationBySNSort.begin(), m_vecStationBySNSort.end(), &sa, CmpStationBySN01);

	for(; lowerIt != m_vecStationBySNSort.end(); ++lowerIt)
	{
		CStation *sa = *lowerIt;
		int len = min(vPrefix.length(), sa->GetShortName().length());
		if (vPrefix.compare(0, len,sa->GetShortName() , 0 , len))
			break;
		
		vecRes.push_back(sa);
		setString.insert(sa->GetStationCode());
	}

	lowerIt = std::lower_bound(m_vecStationByPYSort.begin(), m_vecStationByPYSort.end(), &sa, CmpStationByPY01);

	for (; lowerIt != m_vecStationByPYSort.end(); ++lowerIt)
	{
		CStation *sa = *lowerIt;
		int len = min(vPrefix.length(), sa->GetPinYinName().length());
		if (vPrefix.compare(0, len, sa->GetPinYinName(), 0, len))
			break;

		std::set < std::string>::iterator v = setString.find(sa->GetStationCode());

		if (v != setString.end())
			continue;

		vecRes.push_back(sa);
	}

	std::sort(vecRes.begin(), vecRes.end(), CmpStationBySEQ);

	return vecRes;
}


CStation* Client12306Manager::GetStationByCode(std::string code)
{
	CStation* pRet = NULL;
	std::map<std::string, CStation>::iterator it =  m_mapStation.find(code);

	if (it != m_mapStation.end())
	{
		pRet = &(it->second);
	}

	return pRet;
}


int Client12306Manager::QueryPassenger()
{
	int iRetFlag = SUCCESS;
	try
	{

		string strService = "/otn/confirmPassenger/getPassengerDTOs";


		std::string response = ExecGet(strService);
		std::string gunRes;
		Gunzip((Byte*)const_cast<char*>(response.c_str()), response.length(), gunRes);

		DUI__Trace(_T("%s"), Utf8ToUnicode(gunRes).c_str());

		ParsePassengerString(gunRes);
	}
	catch (Poco::Exception &e)
	{
		DUI__Trace(_T("%s"), Utf8ToUnicode(e.displayText()).c_str());
		return FAIL;
	}

	return iRetFlag;
}

int Client12306Manager::ParsePassengerString(std::string res)
{
	try
	{
		JSON::Parser parser;
		Dynamic::Var result;

		result = parser.parse(res);

		JSON::Object::Ptr pObj = result.extract<JSON::Object::Ptr>();

		Dynamic::Var jStatus = pObj->get("status");

		if (jStatus.toString() != "true")
		{
			DUI__Trace(Utf8ToUnicode(jStatus.toString()).c_str());
			return FAIL;
		}

		if (pObj->has("url"))
		{
			if (pObj->has("messages"))
			{
				Dynamic::Var jMsg = pObj->get("messages");
				m_strLastErrInfo = jMsg.toString();
			}

			return FAIL;
		}


		////data object
		Dynamic::Var pData = pObj->get("data");

		JSON::Object::Ptr jData = pData.extract<JSON::Object::Ptr>();




		////订单个数
		

		JSON::Array::Ptr jNorPassengers = jData->getArray("normal_passengers");
		int cnt = jNorPassengers->size();

		for (int i = 0; i < cnt; ++i)
		{

			Dynamic::Var pPassengerObj = jNorPassengers->get(i);
			JSON::Object::Ptr jPassengerObj = pPassengerObj.extract<JSON::Object::Ptr>();

			CPassenger passenger;

			///
			Dynamic::Var pName = jPassengerObj->get("passenger_name");

			passenger.SetName(Utf8ToUnicode( pName.toString() ).c_str() );

			Dynamic::Var pTypeCode = jPassengerObj->get("passenger_id_type_code");
			passenger.SetCardType(Utf8ToUnicode(pTypeCode.toString()).c_str());

			Dynamic::Var pTypeName = jPassengerObj->get("passenger_id_type_name");
			passenger.SetCardTypeName(Utf8ToUnicode(pTypeName.toString()).c_str());

			Dynamic::Var pIdNo = jPassengerObj->get("passenger_id_no");
			
			passenger.SetCardNo(Utf8ToUnicode(pIdNo.toString()).c_str());

			Dynamic::Var pMobileNo = jPassengerObj->get("mobile_no");

			passenger.SetPhoneNo(Utf8ToUnicode(pMobileNo.toString()).c_str());

			Dynamic::Var pType = jPassengerObj->get("passenger_type");
			passenger.SetType( Utf8ToUnicode(pType.toString()).c_str());

			m_mapPassenger.insert(std::pair<std::string, CPassenger>(UnicodeToUtf8(passenger.GetCardNo().GetData()), passenger));
		}
	}
	catch (Exception &e)
	{
		m_strLastErrInfo = e.displayText();
		return FAIL;
	}

	return SUCCESS;
}

CPassenger* Client12306Manager::GetPassengerByCardNo(std::string cardNo)
{
	CPassenger *ret = NULL;
	std::map<std::string, CPassenger>::iterator it = m_mapPassenger.find(cardNo);
	if (it != m_mapPassenger.end())
	{
		ret = &(it->second);
	}
	return ret;
}

int Client12306Manager::CheckUser()
{
	int iRetFlag = SUCCESS;
	try
	{

		string strService = "/otn/login/checkUser";

		std::string response = ExecPost(strService);

		std::string gunString;
		Gunzip((Byte*)response.c_str() , response.length() , gunString);



		JSON::Parser parser;
		Dynamic::Var result;

		result = parser.parse(gunString);

		JSON::Object::Ptr pObj = result.extract<JSON::Object::Ptr>();

		Dynamic::Var jStatus = pObj->get("status");

		if (jStatus.toString() != "true")
		{
			DUI__Trace(Utf8ToUnicode(jStatus.toString()).c_str());
			return FAIL;
		}

		if (pObj->has("url"))
		{
			if (pObj->has("messages"))
			{
				Dynamic::Var jMsg = pObj->get("messages");
				m_strLastErrInfo = jMsg.toString();
			}

			return FAIL;
		}
			


	}
	catch (Poco::Exception &e)
	{
		DUI__Trace(_T("%s"), Utf8ToUnicode(e.displayText()).c_str());
		m_strLastErrInfo = e.displayText();
		return FAIL;
	}

	return iRetFlag;
}


int Client12306Manager::SubmitOrderRequest(CTicketModel *ticket)
{
	int iRetFlag = SUCCESS;
	try
	{
		string strService = "/otn/leftTicket/submitOrderRequest";

		std::vector <CParam> param;

		////加密串 12306 服务器返回时，已为 encode 模式
		param.push_back(CParam("secretStr" , UnicodeToUtf8(ticket->GetSecretStr().GetData())));		
		param.push_back(CParam("train_date", UnicodeToUtf8(ticket->GetTrainDate().GetData())));
		param.push_back(CParam("back_train_date", UnicodeToUtf8(ticket->GetTrainDate().GetData())));
		param.push_back(CParam("tour_flag", "dc"));
		param.push_back(CParam("purpose_codes", ticket->GetTicketType() == _ADULT ? "ADULT" : "0X00" ));
		param.push_back(CParam("query_from_station_name", UnicodeToUtf8(ticket->GetFromStationName().GetData()) ));
		param.push_back(CParam("query_to_station_name", UnicodeToUtf8(ticket->GetToStationName().GetData())  ));


		std::string response = ExecPostBySeq(strService ,  &param , NULL);

		std::string gunString;
		Gunzip((Byte*)response.c_str(), response.length(), gunString);


		JSON::Parser parser;
		Dynamic::Var result;

		result = parser.parse(gunString);

		JSON::Object::Ptr pObj = result.extract<JSON::Object::Ptr>();

		Dynamic::Var jStatus = pObj->get("status");

		if (jStatus.toString() != "true")
		{
	
			if (pObj->has("messages"))
			{
				Dynamic::Var jMsg = pObj->getArray("messages")->get(0);

				m_strLastErrInfo = jMsg.toString();

				DUI__Trace(Utf8ToUnicode(m_strLastErrInfo).c_str());
			}

			return FAIL;
		}

	}
	catch (Poco::Exception &e)
	{
		DUI__Trace(_T("%s"), Utf8ToUnicode(e.displayText()).c_str());
		return FAIL;
	}

	return iRetFlag;
}


int Client12306Manager::InitDc(std::string &token , std::string &leftTicketSgtring , std::string &keyCheckIsChange)
{
	int iRetFlag = SUCCESS;
	try
	{
		string strService = "/otn/confirmPassenger/initDc";
		
		std::string response = ExecPostBySeq(strService, NULL, NULL);

		std::string gunString;
		Gunzip((Byte*)response.c_str(), response.length(), gunString);


		int pos = gunString.find("globalRepeatSubmitToken");

		int beg = gunString.find("'", pos + 23);

		int end = gunString.find("'", beg + 1 );

		token = gunString.substr(beg + 1 , end - beg -1 );




		pos = gunString.find("'key_check_isChange'", end);

		beg = gunString.find("'", pos + 20);

		end = gunString.find("'", beg + 1);

		keyCheckIsChange = gunString.substr(beg + 1, end - beg - 1);


		pos = gunString.find("'leftTicketStr'", end);

		beg = gunString.find("'", pos + 15);

		end = gunString.find("'", beg + 1);

		leftTicketSgtring = gunString.substr(beg + 1, end - beg - 1);


		

	}
	catch (Poco::Exception &e)
	{
		DUI__Trace(_T("%s"), Utf8ToUnicode(e.displayText()).c_str());
		return FAIL;
	}

	return iRetFlag;

}

int Client12306Manager::CheckOrderInfo(std::vector<CPassengerTicket> &vecPT , CCheckOrderInfoResult &resOrderInfo)
{
	int iRetFlag = SUCCESS;
	try
	{
		string strService = "/otn/confirmPassenger/checkOrderInfo";

		std::vector<CParam> param;

		param.push_back(CParam("cancel_flag", "2"));
		param.push_back(CParam("bed_level_order_num", "000000000000000000000000000000"));

		std::string passengerTicketString;
		std::string oldPassengerString;

		for (std::vector<CPassengerTicket>::iterator it = vecPT.begin(); it != vecPT.end(); ++it)
		{
			if (it != vecPT.begin())
			{
				passengerTicketString += "_";
				oldPassengerString += "_";
			}

			passengerTicketString += UnicodeToUtf8(it->GetSeatType().GetData());
			passengerTicketString += ",";
			passengerTicketString += "0";
			passengerTicketString += ",";
			passengerTicketString += UnicodeToUtf8(it->GetTicketType().GetData());
			passengerTicketString += ",";
			passengerTicketString += UnicodeToUtf8(it->GetPassengerName().GetData());
			passengerTicketString += ",";
			passengerTicketString += UnicodeToUtf8(it->GetIdType().GetData());
			passengerTicketString += ",";
			passengerTicketString += UnicodeToUtf8(it->GetIdNo().GetData());
			passengerTicketString += ",";
			passengerTicketString += UnicodeToUtf8(it->GetMobileNo().GetData());
			passengerTicketString += ",";
			passengerTicketString += UnicodeToUtf8(it->GetSaveStatus().GetData());



			oldPassengerString += UnicodeToUtf8(it->GetPassengerName().GetData());
			oldPassengerString += ",";
			oldPassengerString += UnicodeToUtf8(it->GetIdType().GetData());
			oldPassengerString += ",";
			oldPassengerString += UnicodeToUtf8(it->GetIdNo().GetData());
			oldPassengerString += ",";
			oldPassengerString += UnicodeToUtf8(it->GetPassengerType().GetData());

		}


		param.push_back(CParam("passengerTicketStr", passengerTicketString));
		param.push_back(CParam("oldPassengerStr", oldPassengerString));
		param.push_back(CParam("tour_flag", "dc"));
		param.push_back(CParam("randCode", ""));



		
		std::string response = ExecPostBySeq(strService, &param, NULL);

		std::string gunString;
		Gunzip((Byte*)response.c_str(), response.length(), gunString);



		JSON::Parser parser;
		Dynamic::Var result;

		result = parser.parse(gunString);

		JSON::Object::Ptr pObj = result.extract<JSON::Object::Ptr>();

		Dynamic::Var jStatus = pObj->get("status");

		if (jStatus.toString() != "true")
		{
			DUI__Trace(Utf8ToUnicode(jStatus.toString()).c_str());
			return FAIL;
		}

	

		Dynamic::Var pData = pObj->get("data");
		JSON::Object::Ptr jData = pData.extract<JSON::Object::Ptr>();


		if (jData->has("submitStatus") && jData->get("submitStatus").toString() == "false")
		{

			m_strLastErrInfo = jData->get("errMsg").toString();
			return FAIL;

		}

		resOrderInfo.SetIfShowPassCode(jData->get("ifShowPassCode"));
		resOrderInfo.SetCanChooseBeds(jData->get("canChooseBeds"));
		resOrderInfo.SetCanChooseSeats(jData->get("canChooseSeats"));
		resOrderInfo.SetChoose_Seats(jData->get("choose_Seats"));
		resOrderInfo.SetIsCanChooseMid(jData->get("isCanChooseMid"));
		resOrderInfo.SetIfShowPassCodeTime(jData->get("ifShowPassCodeTime"));
		resOrderInfo.SetSubmitStatus(jData->get("submitStatus"));
		resOrderInfo.SetSmokeStr(jData->get("smokeStr"));


	}
	catch (Poco::Exception &e)
	{
		DUI__Trace(_T("%s"), Utf8ToUnicode(e.displayText()).c_str());
		return FAIL;
	}

	return iRetFlag;
}

int Client12306Manager::getQueueCount(CTicketModel *ticket , std::string token, std::string leftTicketString, std::string seatType, CGetQueqeCountResult &res)
{
	int iRetFlag = SUCCESS;
	try
	{
		string strService = "/otn/confirmPassenger/getQueueCount";

		std::vector<CParam> param;

		//param.push_back(CParam("train_date", UnicodeToUtf8( ticket->GetTrainDate().GetData()) ));
		int year, month, day;
		sscanf( UnicodeToUtf8(ticket->GetTrainDate().GetData()).c_str(), "%d-%d-%d", &year, &month, &day);

		Poco::LocalDateTime ldt;
		ldt.assign(year, month, day);

		std::string  trainDate;
		trainDate += m_strWeekAbbreviate[ldt.dayOfWeek()];
		trainDate += " ";
		trainDate += m_strMonAbbreviate[ldt.month()-1];
		trainDate += " ";
		stringstream ssDay , ssYear;
		ssDay << day;
		trainDate += ssDay.str();
		trainDate += " ";

		ssYear << year;
		trainDate += ssYear.str();
		trainDate += " ";

		trainDate += "00:00:00";
		trainDate += " ";

		trainDate += "GMT+0800";
		trainDate += " ";

		trainDate += "(China Standard Time)";

		std::string enTrainDate;
		UriEncode(trainDate,  enTrainDate);

		param.push_back(CParam("train_date", enTrainDate));
		param.push_back(CParam("train_no", UnicodeToUtf8(ticket->GetTrainNo().GetData())  ));
		param.push_back(CParam("stationTrainCode", UnicodeToUtf8(ticket->GetStationTrainCode().GetData())));

		param.push_back(CParam("seatType", seatType));
		param.push_back(CParam("fromStationTelecode", UnicodeToUtf8(ticket->GetFromStationTelecode().GetData())));
		param.push_back(CParam("toStationTelecode", UnicodeToUtf8(ticket->GetToStationTelecode().GetData())));
		param.push_back(CParam("leftTicket", leftTicketString));

		param.push_back(CParam("purpose_codes", "00"));
		param.push_back(CParam("train_location", UnicodeToUtf8( ticket->GetTrainLocation().GetData() )));

		param.push_back(CParam("_json_att", ""));
		
		param.push_back(CParam("REPEAT_SUBMIT_TOKEN", token));


		std::map<string, string> header;
		header["Referer"] = "https://kyfw.12306.cn/otn/confirmPassenger/initDc";

		std::string response = ExecPostBySeq(strService, &param, &header);

		std::string gunString;
		Gunzip((Byte*)response.c_str(), response.length(), gunString);


		JSON::Parser parser;
		Dynamic::Var result;

		result = parser.parse(gunString);

		JSON::Object::Ptr pObj = result.extract<JSON::Object::Ptr>();

		Dynamic::Var jStatus = pObj->get("status");

		if (jStatus.toString() != "true")
		{
			DUI__Trace(Utf8ToUnicode(jStatus.toString()).c_str());
			if (pObj->has("messages"))
			{
				Dynamic::Var jMsg = pObj->get("messages");
				m_strLastErrInfo = jMsg.toString();
			}
			return FAIL;
		}

		Dynamic::Var pData = pObj->get("data");
		JSON::Object::Ptr jData = pData.extract<JSON::Object::Ptr>();

		res.SetTicket(jData->get("ticket").toString());
		res.SetOp2(jData->get("op_2").toString());
		res.SetCountT(jData->get("countT").toString());

	}
	catch (Poco::Exception &e)
	{
		DUI__Trace(_T("%s"), Utf8ToUnicode(e.displayText()).c_str());
		return FAIL;
	}

	return iRetFlag;
}

void Client12306Manager::UriEncode(std::string str, std::string &out)
{
	const std::string ILLEGAL = "%<>{}|\\\"^`:+";

	for (std::string::iterator it = str.begin(); it != str.end(); ++it)
	{
		unsigned char c = *it;
		if ((c >= 'a' && c <= 'z') ||
			(c >= 'A' && c <= 'Z') ||
			(c >= '0' && c <= '9') ||
			c == '-' || c == '_' ||
			c == '.' || c == '~')
		{
			out += c;
		}
		else if (c == 0x20)
			out += "+";
		else if (c < 0x20 || c >= 0x7F || ILLEGAL.find(c) != std::string::npos)
		{
			char sHex[4] = {0};
			char *ptr = sHex;
			char *q = sHex;
			do
			{
				unsigned int tmp = c;
				c /= 0x10;   ///右移4位

				*ptr++ = "FEDCBA9876543210123456789ABCDEF"[15 + (tmp - c * 0x10)];

			} while (c);

			*ptr-- = '\0';
			////reverse
			while (q < ptr)
			{
				char tmp = *ptr;
				*ptr-- = *q;
				*q++ = tmp;
			}

			out += "%";
			out += sHex;

		}
		else out += c;

	}	

}


int Client12306Manager::ConfirmSingleForQueue(std::vector<CPassengerTicket> &vecPT , CTicketModel *ticket, std::string token, std::string leftTicketString , std::string keyCheckIsChg,std::string randCode )
{
	int iRetFlag = SUCCESS;
	try
	{
		string strService = "/otn/confirmPassenger/confirmSingleForQueue";

		std::map<string, string> header;
		header["Referer"] = "https://kyfw.12306.cn/otn/confirmPassenger/initDc";

		std::vector<CParam> param;		



		std::string passengerTicketString;
		std::string oldPassengerString;

		for (std::vector<CPassengerTicket>::iterator it = vecPT.begin(); it != vecPT.end(); ++it)
		{
			if (it != vecPT.begin())
			{
				passengerTicketString += "_";
				oldPassengerString += "_";
			}

			passengerTicketString += UnicodeToUtf8(it->GetSeatType().GetData());
			passengerTicketString += ",";
			passengerTicketString += "0";
			passengerTicketString += ",";
			passengerTicketString += UnicodeToUtf8(it->GetTicketType().GetData());
			passengerTicketString += ",";
			passengerTicketString += UnicodeToUtf8(it->GetPassengerName().GetData());
			passengerTicketString += ",";
			passengerTicketString += UnicodeToUtf8(it->GetIdType().GetData());
			passengerTicketString += ",";
			passengerTicketString += UnicodeToUtf8(it->GetIdNo().GetData());
			passengerTicketString += ",";
			passengerTicketString += UnicodeToUtf8(it->GetMobileNo().GetData());
			passengerTicketString += ",";
			passengerTicketString += UnicodeToUtf8(it->GetSaveStatus().GetData());



			oldPassengerString += UnicodeToUtf8(it->GetPassengerName().GetData());
			oldPassengerString += ",";
			oldPassengerString += UnicodeToUtf8(it->GetIdType().GetData());
			oldPassengerString += ",";
			oldPassengerString += UnicodeToUtf8(it->GetIdNo().GetData());
			oldPassengerString += ",";
			oldPassengerString += UnicodeToUtf8(it->GetPassengerType().GetData());

		}


		param.push_back(CParam("passengerTicketStr", passengerTicketString , true));
		param.push_back(CParam("oldPassengerStr", oldPassengerString, true));

		param.push_back(CParam("randCode", randCode));
		param.push_back(CParam("purpose_codes", "00"));
		param.push_back(CParam("key_check_isChange", keyCheckIsChg));
		param.push_back(CParam("leftTicketStr", leftTicketString));
		param.push_back(CParam("train_location", UnicodeToUtf8( ticket->GetTrainLocation().GetData())));
		param.push_back(CParam("choose_seats",""));
		param.push_back(CParam("seatDetailType", "000"));
		param.push_back(CParam("roomType", "00"));
		param.push_back(CParam("dwAll", "N"));
		param.push_back(CParam("_json_att", ""));
		param.push_back(CParam("REPEAT_SUBMIT_TOKEN", token));
		
		std::string response = ExecPostBySeq(strService, &param, &header);

		std::string gunString;
		Gunzip((Byte*)response.c_str(), response.length(), gunString);
	
		JSON::Parser parser;
		Dynamic::Var result;

		result = parser.parse(gunString);

		JSON::Object::Ptr pObj = result.extract<JSON::Object::Ptr>();

		Dynamic::Var jStatus = pObj->get("status");

		if (jStatus.toString() != "true")
		{
			DUI__Trace(Utf8ToUnicode(jStatus.toString()).c_str());
			Dynamic::Var pData = pObj->get("data");

			m_strLastErrInfo = pData.toString();

			return FAIL;
		}

	}
	catch (Poco::Exception &e)
	{
		DUI__Trace(_T("%s"), Utf8ToUnicode(e.displayText()).c_str());
		return FAIL;
	}

	return iRetFlag;
}


int Client12306Manager::QueryOrderWaitTime(std::string token, CQueryOrderWaitTimeResult &res)
{
	int iRetFlag = SUCCESS;
	try
	{
		string strService = "/otn/confirmPassenger/queryOrderWaitTime";

		std::map<string, string> header;
		header["Referer"] = "https://kyfw.12306.cn/otn/confirmPassenger/initDc";

		std::map<std::string, std::string> param;

		long rand = GetTickCount();

		stringstream  ss;
		ss << rand;


		param["random"] = ss.str();
		param["tourFlag"] = "dc";
		param["_json_att"] = "";
		param["REPEAT_SUBMIT_TOKEN"] = token;
		

		

		std::string response = ExecGet(strService, &param, &header);

		std::string gunString;
		Gunzip((Byte*)response.c_str(), response.length(), gunString);



		JSON::Parser parser;
		Dynamic::Var result;

		result = parser.parse(gunString);

		JSON::Object::Ptr pObj = result.extract<JSON::Object::Ptr>();

		Dynamic::Var jStatus = pObj->get("status");

		if (jStatus.toString() != "true")
		{
			DUI__Trace(Utf8ToUnicode(jStatus.toString()).c_str());
			Dynamic::Var pData = pObj->get("data");

			m_strLastErrInfo = pData.toString();

			return FAIL;
		}

		Dynamic::Var vData = pObj->get("data");

		JSON::Object::Ptr jData = vData.extract<JSON::Object::Ptr>();

		
		res.SetQueryOrderWaitTimeStatus(jData->get("queryOrderWaitTimeStatus").toString());
		res.SetCount(jData->get("count").toString());
		res.SetWaitTime(jData->get("waitTime").convert<int>());
		res.SetRequestId(jData->get("requestId").toString());
		res.SetWaitCount(jData->get("waitCount").convert<int>());
		res.SetTourFlag(jData->get("tourFlag").toString());
		res.SetOrderId(jData->get("orderId").toString());

	

	}
	catch (Poco::Exception &e)
	{
		DUI__Trace(_T("%s"), Utf8ToUnicode(e.displayText()).c_str());
		return FAIL;
	}

	return iRetFlag;

}

int Client12306Manager::ResultOrderForDcQueue(std::string orderNo, std::string token)
{

	int iRetFlag = SUCCESS;
	try
	{
		string strService = "/otn/confirmPassenger/resultOrderForDcQueue";

		std::map<string, string> header;
		header["Referer"] = "https://kyfw.12306.cn/otn/confirmPassenger/initDc";

		std::map<std::string, std::string> param;



		param["orderSequence_no"] = orderNo;
		param["tourFlag"] = "dc";
		param["_json_att"] = "";
		param["REPEAT_SUBMIT_TOKEN"] = token;




		std::string response = ExecPost(strService, &param, &header);

		std::string gunString;
		Gunzip((Byte*)response.c_str(), response.length(), gunString);



	}
	catch (Poco::Exception &e)
	{
		DUI__Trace(_T("%s"), Utf8ToUnicode(e.displayText()).c_str());
		return FAIL;
	}

	return iRetFlag;
	
}
