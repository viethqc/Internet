#ifndef _INTERNET_H_
#define _INTERNET_H_
#include <string>

#include <winsock2.h>
#include "ReleaseEvent.h"
#pragma comment(lib,"ws2_32.lib")

using namespace std;

namespace Internet
{

#define TIME_OUT	60

class CInternet
{

public:
	static bool VisitUrl(string strURL, int iPort, string &strResponseData);
	static bool ParseUrl(string strURL, string &strServerName, string &strObjectName);
	bool InternetConnect(string strURL, int iPort);
	bool SendRequest(string strHeader, string strData, string &strResponse);
	bool InternetClose();
	CInternet();
	~CInternet();
private:
	SOCKET ConnectToServer(string strServerName, int iPort);
	bool   RecvData(SOCKET sSocket, string &strResponse);
	bool   SendRequest(SOCKET sSocket, string strHeader, string strData);

private:
	SOCKET m_iSocket;
};

}

#endif