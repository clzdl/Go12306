#include "stdafx.h"
#include "Log.h"
#include "Client12306Manager.h"

CLog* CLog::m_objInstance = NULL;

std::string CLog::m_sConfig = "logging.loggers.root.channel = c2\n"
"logging.loggers.root.level = information\n"
"logging.channels.c2.class = FileChannel\n"
"logging.channels.c2.path = GO12306.log\n"
"logging.channels.c2.formatter = f1\n"
"logging.channels.c2.rotation = daily\n"
"logging.channels.c2.archive = timestamp\n"
"logging.formatters.f1.class = PatternFormatter\n"
"logging.formatters.f1.pattern =  %Y-%m-%d %H:%M:%S.%i [%P:%I]:%q:%t\n"
"logging.formatters.f1.times = local\n";



CLog* CLog::GetInstance()
{
	if (m_objInstance == NULL)
	{
		m_objInstance = new CLog();
	}

	return m_objInstance;
}

CLog::CLog()
{
	////日志初始化
	std::istringstream istr(m_sConfig);

	AutoPtr<PropertyFileConfiguration> pConfig = new PropertyFileConfiguration(istr);

	LoggingConfigurator configurator;

	configurator.configure(pConfig);

}

CLog::~CLog()
{
}

CLog::CLog(const CLog& log)
{

}

CLog& CLog::operator =(const CLog& log)
{
	return *this;
}


void CLog::Log(std::string msg)
{
	static Logger& root = Logger::get("");

	root.information(msg);

}