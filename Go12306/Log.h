#pragma once


class CLog
{
public:
	static CLog* GetInstance();

	void Log(std::string msg);
private:
	CLog();
	~CLog();
	CLog(const CLog& log);
	CLog& operator =(const CLog& log);
	
	static CLog* m_objInstance;

	static std::string m_sConfig;

};
