#ifndef ERROREXCEPTION_H_
#define ERROREXCEPTION_H_

#include <exception>

using namespace std;

#define RETURN(x)	(throw CReleaseEvent(x))

class  CReleaseEvent : public exception
{
public:
	 CReleaseEvent(bool bReturnValue);
	~ CReleaseEvent();
	bool GetReturn();

private :
	bool m_bReturnValue;
};

#endif