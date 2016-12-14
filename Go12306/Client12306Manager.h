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

#include "TicketModel.h"
#include "OrderModel.h"

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



using namespace Poco::Dynamic;
using namespace Poco;




class Client12306Manager
{
public:
	static Client12306Manager *Instance();

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
	int QueryMyOrder(std::string startDate , std::string endDate , std::string seqTrainName , std::map<string, COrderModel> &mapOrder);



	std::string GetLastErrInfo() { return m_strLastErrInfo; }
private:


	/*@action: 执行 http 的get请求
	*/
	std::string ExecPost(std::string service, std::map<string, string> *param = NULL,std::map<string,string> *header=NULL);

	/*@action: 执行 http 的post请求
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
	int JsonParseTicket(std::string jsonString , std::vector<CTicketModel> &vecTicket);

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
};