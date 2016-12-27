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
	std::string m_strShortName;   ///ƴ����д
	std::string m_strChinaName;		///����
	std::string m_strPinYinName;   ///����ƴ��
	std::string m_strStationCode;   ///վ�����
	unsigned m_uiSeq;		///���


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

	std::string m_strTicket;     /// ���ŷָ������磺 n1,n2   ;  n1: ָ����λ����Ʊ��  n2: ����Ʊ
	std::string m_strOp2;     ///true: Ŀǰ�Ŷ������Ѿ�������Ʊ����������ѡ������ϯ��򳵴� �� 
	std::string m_strCountT;     //// op_2 = false ,  Ŀǰ�Ŷ�����   




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
	/*@action: ��ѯ��Ʊ
	*@parameter:
	*	begPlace:��Σ� ��վ���룺  SJP  ʯ��ׯ
	*	endPlace:��Σ� ��վ���룺  BJP  ����
	*	travelTime����Σ� �˳�����:  2016-12-12
	*	vecTicket:���Σ�  ��Ʊ��Ϣ
	*	ticketType����Σ� ��Ʊ���� :  ����/ѧ��
	*@return: 0-success;-1-fail
	*/
	int QueryLeftTicket(std::string begPlace, std::string endPlace, std::string travelTime , std::vector<CTicketModel> &vecTicket, _TICKET_TYPE ticketType = _ADULT);
	
	/*@action:��ʼ����¼
	*/
	int LoginInit( );


	/*@action:��ȡ��֤��
	*/
	int QueryPassCode(std::string moduleName, std::string &bytes);

	

	/*@action: �첽У����֤��
	*/
	int AnsynValidPassCode(std::vector<CDuiPoint> &selPoints, bool &flag);



	/*@action: �첽�ĵ�¼
	*/
	int AnsysLoginSugguest(std::string userName, std::string userPass, std::string randCode, bool &flag);


	/*@action:  �û���¼
	*/
	int UserLogin(std::string &res);

	/*@action:  �û���ʼ��
	*/
	int InitMy12306(std::string &res);

	/*@action: ��ѯ�û�����
	*/
	int QueryMyOrder(std::string startDate , std::string endDate , std::string type, std::string seqTrainName , std::map<string, COrderModel> &mapOrder);

	/*@action:  ��ѯվ����
	*/
	int Query12306StationName();

	std::string GetLastErrInfo() { return m_strLastErrInfo; }

	/*@action: ��ȡ����������վ������
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


	/*@action ����֤�������ȡpassenger��Ϣ
	*/
	CPassenger* GetPassengerByCardNo(std::string cardNo);

	/*@action:  ����û�
	*/
	int CheckUser();

	/*@action:  �ύ��������
	*/
	int SubmitOrderRequest(CTicketModel *ticket);


	/*@action:  ��ʼ�����̹�Ʊ����
	*/
	int InitDc(std::string &token , std::string &leftTicketSgtring, std::string &keyCheckIsChange);

	/*@action:  ��鶩����Ϣ
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


	/*@action: ִ�� http ��post����
	*/
	std::string ExecPost(std::string service, std::map<string, string> *param = NULL,std::map<string,string> *header=NULL);

	/*@action: ִ�� http ��post����
	*/
	std::string ExecPostBySeq(std::string service, std::vector<CParam> *param = NULL, std::map<string, string> *header = NULL);

	/*@action: ִ�� http ��get����
	*/
	std::string ExecGet(std::string service, std::map<string, string> *param = NULL, std::map<string, string> *header = NULL);


	

	/*@action: �첽У����֤��
	*/
	int AnsynValidPassCode(std::vector<CDuiPoint> &selPoints, std::string &res);


	/*@action: �첽�ĵ�¼
	*/
	int AnsysLoginSugguest(std::string userName, std::string userPass, std::string randCode, std::string &res);

	/*@action: ���� ���ص�json��
	*@parameter:
	*	jsonString:��Σ���Ҫ������json��
	*	vecTicket:���Σ� ��Ʊ��Ϣ
	*@return: 0-success;-1-fail
	*/
	int JsonParseTicket(std::string jsonString ,std::string travelTime, _TICKET_TYPE ticketType, std::vector<CTicketModel> &vecTicket);

	/*@action: 
	*/
	int AssignJson2TicketObj(JSON::Object::Ptr queryDto, CTicketModel &objTicket);


	/*@action: ��ѯ��Ʊǰ��log����
	*/
	int QueryTicketLog(std::string begPlace, std::string endPlace, std::string travelTime,bool &flag, _TICKET_TYPE ticketType = _ADULT);

	/*@action: ���� ���ص�json��
	*@parameter:
	*	jsonString:��Σ���Ҫ������json��
	*	mapOrder:���Σ�������Ϣ
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

	///վ�㻺��
	std::map<std::string, CStation> m_mapStation;

	///�������Ƽ�д����
	std::vector<CStation*> m_vecStationBySNSort;


	///����ƴ��ȫƴ����
	std::vector<CStation*> m_vecStationByPYSort;

	///�˿���Ϣ
	std::map<std::string, CPassenger> m_mapPassenger;

	std::string m_strCtx12306;    ///12306��Ŀ��Ŀ¼
	std::string m_strLeftTicketUrl;

	static char *m_strMonAbbreviate[];
	static char *m_strWeekAbbreviate[];
};



