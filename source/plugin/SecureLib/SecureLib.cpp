#include <module/plugininc.h>		/* important */
#include "SecureLib.h"
#include <iphlpapi.h>

#pragma comment(lib,"Iphlpapi.lib") 

CSecureLib::CSecureLib()
{

}

CSecureLib::~CSecureLib()
{

}

CodeRet_t CSecureLib::GetPCPrivateInfo(uint8_t* pBuffer,uint8_t MAX_Len)
{
	int i = 0,j = 0;
	int ret = 0;
	unsigned long size = 0;

	PIP_ADAPTER_INFO pIpAdapterInfo = new IP_ADAPTER_INFO();
	ret = GetAdaptersInfo(pIpAdapterInfo, &size);

	if (ERROR_BUFFER_OVERFLOW == ret)
	{
		delete pIpAdapterInfo;
		pIpAdapterInfo = (PIP_ADAPTER_INFO)new BYTE[size];
		ret = GetAdaptersInfo(pIpAdapterInfo, &size);
	}

	if (ERROR_SUCCESS == ret)
	{
		j = 0;
		while (pIpAdapterInfo)
		{
			for (i=0; i < pIpAdapterInfo->AddressLength; i++)
			{
				pBuffer[j++] = pIpAdapterInfo->Address[i];

				if (j == (MAX_Len-1))
				{
					pBuffer[j] = NUM_ARRAY_STOP_FLAG;
					break;
				}
			}

			pIpAdapterInfo = pIpAdapterInfo->Next;
		}

		if (pIpAdapterInfo)
		{
			delete pIpAdapterInfo;
			pIpAdapterInfo = NULL;
		}
	}
	else
	{
		return RET_ERROR;
	}

	if (pIpAdapterInfo)
	{
		delete pIpAdapterInfo;
		pIpAdapterInfo = NULL;
	}
	return RET_OK;
}