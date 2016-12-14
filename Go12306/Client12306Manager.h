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
	int QueryMyOrder(std::string startDate , std::string endDate , std::string seqTrainName , std::map<string, COrderModel> &mapOrder);



	std::string GetLastErrInfo() { return m_strLastErrInfo; }
private:


	/*@action: ִ�� http ��get����
	*/
	std::string ExecPost(std::string service, std::map<string, string> *param = NULL,std::map<string,string> *header=NULL);

	/*@action: ִ�� http ��post����
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
	int JsonParseTicket(std::string jsonString , std::vector<CTicketModel> &vecTicket);

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