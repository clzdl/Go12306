#pragma once



#include "Poco/Net/HTTPSClientSession.h"
#include "Poco/Net/HTTPRequest.h"
#include "Poco/Net/HTTPResponse.h"
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


#include "TicketModel.h"

using Poco::StreamCopier;
using Poco::Net::Context;
using Poco::Net::HTTPSClientSession;
using Poco::Net::HTTPRequest;
using Poco::Net::HTTPResponse;
using Poco::SharedPtr;
using Poco::Net::InvalidCertificateHandler;
using Poco::Net::SSLManager;
using Poco::Net::KeyConsoleHandler;
using Poco::Net::ConsoleCertificateHandler;
using Poco::Net::ConsoleCertificateHandler;




using namespace Poco::Dynamic;
using namespace Poco;


typedef enum
{
	_ADULT,
	_STUDENT,
} _TICKET_TYPE;

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
	
private:
	/*@action: ִ�� http ��get����
	*/
	std::string ExecPost(std::string service, std::map<string, string> *param = NULL);

	/*@action: ִ�� http ��post����
	*/
	std::string ExecGet(std::string service, std::map<string, string> *param = NULL);

	/*@action: ���� ���ص�json��
	*@parameter:
	*	jsonString:��Σ���Ҫ������json��
	*	vecTicket:���Σ� ��Ʊ��Ϣ
	*@return: 0-success;-1-fail
	*/
	int JsonParseTicket(std::string jsonString , std::vector<CTicketModel> &vecTicket);

	/*@
	*/
	int AssignJson2TicketObj(JSON::Object::Ptr queryDto, CTicketModel &objTicket);

	Client12306Manager();

	~Client12306Manager();

	Client12306Manager(const Client12306Manager &hcm);
	Client12306Manager& operator =(const Client12306Manager &hcm);

	static std::string m_strDomain;

	static SharedPtr<InvalidCertificateHandler> m_ptrCert;
	static Context::Ptr m_ptrContext;

	static Client12306Manager *m_objInstance;


	HTTPSClientSession m_sessHttpsClient;
};