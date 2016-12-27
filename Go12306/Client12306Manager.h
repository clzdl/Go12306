#pragma once



#include "Poco/Net/HTTPSClientSession.h"
#include "Poco/Net/HTTPRequest.h"
#include "Poco/Net/HTTPResponse.h"
#include "Poco/Net/HTTPCookie.h"
#include "Poco/Net/NameValueCollection.h"
#include "Poco/Net/SecureStreamSocket.h"
#include "Poco/Net/Context.h"
#include "Poco/Net/Session.h"
#include "Poco/Net/SSLManager.h"
#include "Poco/Net/SSLException.h"
#include "Poco/SharedPtr.h"



#include "Poco/Net/SSLManager.h"
#include "Poco/Net/KeyConsoleHandler.h"
#include "Poco/Net/ConsoleCertificateHandler.h"
#include "Poco/Net/InvalidCertificateHandler.h"
#include "Poco/Net/AcceptCertificateHandler.h"


#include "Poco/StreamCopier.h"
#include "Poco/Exception.h"

#include "Poco/JSON/Parser.h"
#include "Poco/JSON/ParseHandler.h"
#include "Poco/JSON/JSONException.h"
#include "Poco/Dynamic/Var.h"
#include "Poco/JSON/Query.h"
#include "Poco/JSON/PrintHandler.h"
#include "Poco/URI.h"
#include "Poco/Util/LoggingConfigurator.h"
#include "Poco/Util/PropertyFileConfiguration.h"
#include "Poco/Logger.h"



#include "TicketModel.h"
#include "OrderModel.h"
#include "Passenger.h"
#include "PassengerTicket.h"

using Poco::StreamCopier;
using Poco::Net::Context;
using Poco::Net::HTTPSClientSession;
using Poco::Net::HTTPRequest;
using Poco::Net::HTTPResponse;
using Poco::Net::HTTPCookie;
using Poco::Net::NameValueCollection;
using Poco::SharedPtr;
using Poco::Net::InvalidCertificateHandler;
using Poco::Net::SSLManager;
using Poco::Net::KeyConsoleHandler;
using Poco::Net::ConsoleCertificateHandler;
using Poco::Net::ConsoleCertificateHandler;
using Poco::URI;
using Poco::Util::LoggingConfigurator;
using Poco::Util::PropertyFileConfiguration;
using Poco::Logger;


using namespace Poco::Dynamic;
using namespace Poco;


class CParam
{
public:
	CParam(std::string key, std::string value , bool encode = false):m_strKey(key),m_strValue(value), m_bNeedEndcode(encode){}
	~CParam() {}

	std::string GetKey() { return m_strKey; }
	std::string GetValue() { return m_strValue; }
	bool GetNeedEncode() { return m_bNeedEndcode; }

private:
	std::string m_strKey;
	std::string m_strValue;
	bool m_bNeedEndcode;
};

class CStation
{
public:
	CStation() {}
	~CStation() {}

	void SetShortName(std::string v) { m_strShortName = v; }
	void SetChinaName(std::string v) { m_strChinaName = v; }
	void SetPinYinName(std::string v) { m_strPinYinName = v; }
	void SetStationCode(std::string v) { m_strStationCode = v; }
	void SetSeq(unsigned int v) { m_uiSeq = v; }

	std::string GetShortName() const { return  m_strShortName ; }
	std::string GetChinaName() const{ return m_strChinaName; }
	std::string GetPinYinName() const { return m_strPinYinName; }
	std::string GetStationCode() const { return m_strStationCode; }
	unsigned int GetSeq() const {return m_uiSeq;}

private:
	std::string m_strShortName;   ///拼音缩写
	std::string m_strChinaName;		///汉字
	std::string m_strPinYinName;   ///汉字拼音
	std::string m_strStationCode;   ///站点编码
	unsigned m_uiSeq;		///序号


};

class CCheckOrderInfoResult
{
public:
	CCheckOrderInfoResult() {}
	~CCheckOrderInfoResult() {}


	void SetIfShowPassCode(std::string v) { m_strIfShowPassCode = v; }
	void SetCanChooseBeds(std::string v) { m_strCanChooseBeds = v; }
	void SetCanChooseSeats(std::string v) { m_strCanChooseSeats = v; }
	void SetChoose_Seats(std::string v) { m_strChoose_Seats = v; }
	void SetIsCanChooseMid(std::string v) { m_strIsCanChooseMid = v; }
	void SetIfShowPassCodeTime(std::string v) { m_strIfShowPassCodeTime = v; }
	void SetSubmitStatus(std::string v) { m_strSubmitStatus = v; }
	void SetSmokeStr(std::string v) { m_strSmokeStr = v; }


	std::string GetIfShowPassCode() { return m_strIfShowPassCode; }
	std::string GetCanChooseBeds() { return m_strCanChooseBeds; }
	std::string GetCanChooseSeats() { return m_strCanChooseSeats; }
	std::string GetChoose_Seats() { return m_strChoose_Seats; }
	std::string GetIsCanChooseMid() {  return m_strIsCanChooseMid; }
	std::string GetIfShowPassCodeTime() { return m_strIfShowPassCodeTime; }
	std::string GetSubmitStatus() { return  m_strSubmitStatus ; }
	std::string GetSmokeStr() { return  m_strSmokeStr; }

private:

	std::string  m_strIfShowPassCode;		////"ifShowPassCode" : "N",
	std::string m_strCanChooseBeds;			////"canChooseBeds" : "N",
	std::string m_strCanChooseSeats;		////"canChooseSeats" : "N",
	std::string m_strChoose_Seats;			////"choose_Seats" : "MOP9",
	std::string m_strIsCanChooseMid;		////	"isCanChooseMid" : "N",
	std::string m_strIfShowPassCodeTime;	////"ifShowPassCodeTime" : "1",
	std::string m_strSubmitStatus;			////"submitStatus" : true,
	std::string m_strSmokeStr;				////"smokeStr" : ""


};

class CGetQueqeCountResult
{
public:
	CGetQueqeCountResult() {}
	~CGetQueqeCountResult() {}

	void SetTicket(std:: string v) { m_strTicket = v; }
	void SetOp2(std:: string v) { m_strOp2 = v; }
	void SetCountT(std:: string v) { m_strCountT = v; }

	std::string  GetTicket() { return m_strTicket; }
	std::string  GetOp2() { return  m_strOp2; }
	std::string  GetCountT() { return  m_strCountT ; }

private:

	std::string m_strTicket;     /// 逗号分隔串；如： n1,n2   ;  n1: 指定座位的余票，  n2: 无座票
	std::string m_strOp2;     ///true: 目前排队人数已经超过余票张数，请您选择其他席别或车次 ； 
	std::string m_strCountT;     //// op_2 = false ,  目前排队人数   




};


class CQueryOrderWaitTimeResult
{
public:
	CQueryOrderWaitTimeResult() :m_iWaitTime(0), m_iWaitCount(0){}
	~CQueryOrderWaitTimeResult() {}

	void SetQueryOrderWaitTimeStatus(std::string v) { m_strQueryOrderWaitTimeStatus = v; }
	void SetCount(std::string v) { m_strCount = v; }
	void SetWaitTime(int v) { m_iWaitTime = v; }
	void SetRequestId(std::string v) { m_strRequestId = v; }
	void SetWaitCount(int v) { m_iWaitCount = v; }
	void SetTourFlag(std::string v) { m_strTourFlag = v; }
	void SetOrderId(std::string v) { m_strOrderId = v; }

	std::string GetQueryOrderWaitTimeStatus() { return m_strQueryOrderWaitTimeStatus ; }
	std::string GetCount() { return m_strCount; }
	int GetWaitTime() { return m_iWaitTime; }
	std::string GetRequestId() { return m_strRequestId; }
	int GetWaitCount() { return m_iWaitCount; }
	std::string GetTourFlag() { return m_strTourFlag; }
	std::string GetOrderId() { return m_strOrderId; }

	void Init()
	{
		m_strQueryOrderWaitTimeStatus.clear();
		m_strCount.clear();
		m_iWaitTime = 0;
		m_strRequestId.clear();
		m_iWaitCount = 0;
		m_strTourFlag.clear();
		m_strOrderId.clear();
	}

private:
	std::string m_strQueryOrderWaitTimeStatus;
	std::string m_strCount;
	int m_iWaitTime;
	std::string m_strRequestId;
	int m_iWaitCount;
	std::string m_strTourFlag;
	std::string m_strOrderId;

};

class Client12306Manager
{
public:
	static Client12306Manager *Instance();


	/*@action:
	*/
	int LeftTicketInit();
	/*@action: 查询余票
	*@parameter:
	*	begPlace:入参， 车站代码：  SJP  石家庄
	*	endPlace:入参， 车站代码：  BJP  北京
	*	travelTime：入参， 乘车日期:  2016-12-12
	*	vecTicket:出参，  余票信息
	*	ticketType：入参， 购票类型 :  成人/学生
	*@return: 0-success;-1-fail
	*/
	int QueryLeftTicket(std::string begPlace, std::string endPlace, std::string travelTime , std::vector<CTicketModel> &vecTicket, _TICKET_TYPE ticketType = _ADULT);
	
	/*@action:初始化登录
	*/
	int LoginInit( );


	/*@action:获取验证码
	*/
	int QueryPassCode(std::string moduleName, std::string &bytes);

	

	/*@action: 异步校验验证码
	*/
	int AnsynValidPassCode(std::vector<CDuiPoint> &selPoints, bool &flag);



	/*@action: 异步的登录
	*/
	int AnsysLoginSugguest(std::string userName, std::string userPass, std::string randCode, bool &flag);


	/*@action:  用户登录
	*/
	int UserLogin(std::string &res);

	/*@action:  用户初始化
	*/
	int InitMy12306(std::string &res);

	/*@action: 查询用户订单
	*/
	int QueryMyOrder(std::string startDate , std::string endDate , std::string type, std::string seqTrainName , std::map<string, COrderModel> &mapOrder);

	/*@action:  查询站点新
	*/
	int Query12306StationName();

	std::string GetLastErrInfo() { return m_strLastErrInfo; }

	/*@action: 获取符合条件的站点数据
	*/
	std::vector<CStation*> GetStation(std::string vPrefix);

	/*@action: 
	*/
	std::vector<CStation*>& Get12306Station() { return m_vecStationByPYSort; }

	/*@action:
	*/
	CStation* GetStationByCode(std::string code);

	/*@action:
	*/

	int QueryPassenger();

	/*@action:
	*/
	std::map<std::string, CPassenger>& GetPassenger() { return m_mapPassenger; }


	/*@action 根据证件号码获取passenger信息
	*/
	CPassenger* GetPassengerByCardNo(std::string cardNo);

	/*@action:  检查用户
	*/
	int CheckUser();

	/*@action:  提交订单请求
	*/
	int SubmitOrderRequest(CTicketModel *ticket);


	/*@action:  初始化单程购票界面
	*/
	int InitDc(std::string &token , std::string &leftTicketSgtring, std::string &keyCheckIsChange);

	/*@action:  检查订单信息
	*/
	int CheckOrderInfo(std::vector<CPassengerTicket> &vecPT , CCheckOrderInfoResult &resOrderInfo);


	/*@action:
	*/
	int getQueueCount(CTicketModel *ticket ,std::string token , std::string leftTicketString, std::string seatType , CGetQueqeCountResult &res);

	/*@action:
	*/
	int ConfirmSingleForQueue(std::vector<CPassengerTicket> &vecPT , CTicketModel *ticket, std::string token, std::string leftTicketString, std::string keyCheckIsChg,std::string randCode="");
	
	/*@acition:
	*/
	int QueryOrderWaitTime(std::string token , CQueryOrderWaitTimeResult &res);

	/*@action:
	*/
	int ResultOrderForDcQueue(std::string orderNo , std::string token);

private:


	/*@action: 执行 http 的post请求
	*/
	std::string ExecPost(std::string service, std::map<string, string> *param = NULL,std::map<string,string> *header=NULL);

	/*@action: 执行 http 的post请求
	*/
	std::string ExecPostBySeq(std::string service, std::vector<CParam> *param = NULL, std::map<string, string> *header = NULL);

	/*@action: 执行 http 的get请求
	*/
	std::string ExecGet(std::string service, std::map<string, string> *param = NULL, std::map<string, string> *header = NULL);


	

	/*@action: 异步校验验证码
	*/
	int AnsynValidPassCode(std::vector<CDuiPoint> &selPoints, std::string &res);


	/*@action: 异步的登录
	*/
	int AnsysLoginSugguest(std::string userName, std::string userPass, std::string randCode, std::string &res);

	/*@action: 解析 返回的json串
	*@parameter:
	*	jsonString:入参，需要解析的json串
	*	vecTicket:出参， 余票信息
	*@return: 0-success;-1-fail
	*/
	int JsonParseTicket(std::string jsonString ,std::string travelTime, _TICKET_TYPE ticketType, std::vector<CTicketModel> &vecTicket);

	/*@action: 
	*/
	int AssignJson2TicketObj(JSON::Object::Ptr queryDto, CTicketModel &objTicket);


	/*@action: 查询余票前的log请求
	*/
	int QueryTicketLog(std::string begPlace, std::string endPlace, std::string travelTime,bool &flag, _TICKET_TYPE ticketType = _ADULT);

	/*@action: 解析 返回的json串
	*@parameter:
	*	jsonString:入参，需要解析的json串
	*	mapOrder:出参，订单信息
	*@return: 0-success;-1-fail
	*/
	int JsonParseOrder(std::string jsonString, std::map<std::string, COrderModel> &mapOrder);


	/*@action:
	*/
	int AssignJson2OrderObj(JSON::Object::Ptr jOrderDTOData, COrderModel &objOrder);

	/*@action:
	*/
	int AssignJson2OrderTicketObj(JSON::Object::Ptr jOrderTicketDTOData, COrderTicketModel &objOrderTicket);


	/*@action: 
	*/
	int ParseStationString(std::string res);


	/*@action:
	*/
	int ParsePassengerString(std::string res );

	/*@action:
	*/
	void UriEncode(std::string str, std::string &out);

	Client12306Manager();

	~Client12306Manager();

	Client12306Manager(const Client12306Manager &hcm);
	Client12306Manager& operator =(const Client12306Manager &hcm);

	static std::string m_strDomain;

	static SharedPtr<InvalidCertificateHandler> m_ptrCert;
	static Context::Ptr m_ptrContext;

	static Client12306Manager *m_objInstance;

	
	NameValueCollection m_cookieCollection;
	HTTPSClientSession m_sessHttpsClient;

	std::map<string, string> m_headerDefault;

	std::string m_strLastErrInfo;

	///站点缓存
	std::map<std::string, CStation> m_mapStation;

	///根据名称简写排序
	std::vector<CStation*> m_vecStationBySNSort;


	///根据拼音全拼排序
	std::vector<CStation*> m_vecStationByPYSort;

	///乘客信息
	std::map<std::string, CPassenger> m_mapPassenger;

	std::string m_strCtx12306;    ///12306项目根目录
	std::string m_strLeftTicketUrl;

	static char *m_strMonAbbreviate[];
	static char *m_strWeekAbbreviate[];
};



