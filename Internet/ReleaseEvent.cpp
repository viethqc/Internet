#include "stdafx.h"
#include "ReleaseEvent.h"

CReleaseEvent:: CReleaseEvent(bool bReturnValue)
{
	m_bReturnValue = bReturnValue;
}

CReleaseEvent::~ CReleaseEvent()
{

}

bool CReleaseEvent::GetReturn()
{
	return m_bReturnValue;
}