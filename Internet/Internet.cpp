#include "Internet.h"

namespace Internet
{

CInternet::CInternet()
{
	m_iSocket = INVALID_SOCKET;
}

CInternet::~CInternet()
{
	
}

bool CInternet::VisitUrl(string strURL, int iPort, string &strResponseData)
{
	string strServerName;
	string strObjName;
	SOCKET sSocket = INVALID_SOCKET;
	string strHeader;
	CInternet cInternet;

	try
	{
		if (ParseUrl(strURL, strServerName, strObjName) == false)
		{
			RETURN(false);
		}

		if (cInternet.InternetConnect(strURL, iPort) == false)
		{
			RETURN(false);
		}

		strHeader = "GET /";
		strHeader.append(strObjName);
		strHeader.append(" HTTP/1.1\r\n");
		strHeader.append("Host: ");
		strHeader.append(strServerName);
		strHeader.append("\r\n");
		strHeader.append("Connection: close\r\n\r\n");

		if (cInternet.SendRequest(strHeader, "", strResponseData) == false)
		{
			RETURN(false);
		}

		RETURN(true);
	}
	catch (CReleaseEvent e)
	{
		cInternet.InternetClose();

		return e.GetReturn();
	}
}

bool CInternet::ParseUrl(string strURL, string &strServerName, string &strObjectName)
{
	int iDoubleSlash	= 0;
	int iSecondSlash	= 0;
	int iServerNameLength = 0;
	int iObjectNameLength = 0;

	if (strURL.empty() == true)
	{
		return false;
	}

	if ((strURL.find("http://") == string::npos) && (strURL.find("https://") == string::npos))
	{
		return false;
	}

	strServerName.clear();
	strObjectName.clear();

	//find // in http://
	iDoubleSlash = strURL.find("//");
	if (iDoubleSlash ==  string::npos)
	{
		return false;
	}

	iSecondSlash = strURL.find("/", iDoubleSlash + 2);
	if (iSecondSlash == string::npos)
	{
		strServerName = strURL.substr(iDoubleSlash + 2, strURL.length() - iDoubleSlash - 2);
	}
	else
	{
		iServerNameLength = iSecondSlash - (iDoubleSlash + 2);
		iObjectNameLength = strURL.length() - (iSecondSlash + 1);

		strServerName = strURL.substr(iDoubleSlash + 2, iServerNameLength);
		strObjectName = strURL.substr(iSecondSlash + 1, iObjectNameLength);
	}

	return true;
}

SOCKET CInternet::ConnectToServer(string strServerName, int iPort)
{
	WSADATA wsaData;
	SOCKET sSocket			= INVALID_SOCKET;
	struct hostent *host	= NULL;
	SOCKADDR_IN SockAddr;
	unsigned int uiAddr;

	if (strServerName.empty() == true)
	{
		return INVALID_SOCKET;
	}

    if (WSAStartup(MAKEWORD(2,2), &wsaData) != 0)
	{
		return INVALID_SOCKET;
    }

	//create socket
	sSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (sSocket == INVALID_SOCKET)
	{
		return INVALID_SOCKET;
	}

	//Phan giai ten mien
	uiAddr = inet_addr(strServerName.data());
	if(uiAddr == INADDR_NONE)
    {
        host = gethostbyname(strServerName.data());
    }
    else
    {
        host = gethostbyaddr((char*)&uiAddr, sizeof(uiAddr),AF_INET);
    }

	if (host == NULL)
	{
		return INVALID_SOCKET;
	}

	SockAddr.sin_port			=	htons(iPort);
    SockAddr.sin_family			=	AF_INET;
    SockAddr.sin_addr.s_addr	= *((unsigned long*)host->h_addr);

	if(connect(sSocket, (SOCKADDR*)(&SockAddr), sizeof(SockAddr)) != 0)
	{
		return INVALID_SOCKET;
    }

	return sSocket;
}

bool   CInternet::SendRequest(SOCKET sSocket, string strHeader, string strData)
{
	string strMsg;

	if (sSocket == INVALID_SOCKET)
	{
		return false;
	}

	if (m_iSocket == INVALID_SOCKET)
	{
		return false;
	}

	strMsg.clear();
	strMsg.append(strHeader);
	strMsg.append("\r\n\r\n");
	strMsg.append(strData);
	if (send(m_iSocket, strMsg.data(), strMsg.length(), 0) == SOCKET_ERROR)
	{
		return false;
	}

	return true;
}

bool   CInternet::RecvData(SOCKET sSocket, string &strResponse)
{
	struct timeval timeout;
	fd_set set;
	int iResult;
	char szBuffer[2048] = {0};
	int iDataLength;

	if (sSocket == INVALID_SOCKET)
	{
		return false;
	}

	strResponse.clear();

	FD_ZERO(&set); /* clear the set */
	FD_SET(sSocket, &set); /* add our file descriptor to the set */
	timeout.tv_sec = TIME_OUT;
	timeout.tv_usec = 0;

	iResult = select(sSocket, &set, NULL, NULL, &timeout);
	if (iResult == SOCKET_ERROR)
	{
	
	}
	else if (iResult == 0)
	{
		
	}
	else
	{
		while ((iDataLength = recv(sSocket, szBuffer, 2040, 0)) > 0)
		{        
			szBuffer[iDataLength] = 0;
			strResponse.append(szBuffer);
		}
    }

	return true;
}

bool CInternet::InternetConnect(string strURL, int iPort)
{
	string strServerName;
	string strObjectName;

	if (ParseUrl(strURL, strServerName, strObjectName) == false)
	{
		return false;
	}

	m_iSocket = INVALID_SOCKET;
	m_iSocket = ConnectToServer(strServerName, iPort);
	if (m_iSocket == INVALID_SOCKET)
	{
		return false;
	}

	return true;
}

bool CInternet::InternetClose()
{
	if (m_iSocket != INVALID_SOCKET)
	{
		closesocket(m_iSocket);
	}

	return true;
}

bool CInternet::SendRequest(string strHeader, string strData, string &strResponse)
{
	if (m_iSocket == INVALID_SOCKET)
	{
		return false;
	}

	if (SendRequest(m_iSocket, strHeader, strData) == false)
	{
		return false;
	}

	if (RecvData(m_iSocket, strResponse) == false)
	{
		return false;
	}

	return true;
}

}