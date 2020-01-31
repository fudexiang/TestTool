#include <portability/x3port.h>
#include <nonplugin/scanplugins.h>

#include <thread>


#define SPDLOG_TEST
//#define CRC_TEST1
//#define CRC_TEST2
//#define UART_TEST

#include <stdio.h>
#include "Loglib\ILogLib.h"
#include "Comlib\IComLib.h"
#include "HexStr\IHexStr.h"

#ifdef CRC_TEST
typedef struct
{
	uint16_t result;
	uint16_t count;
}EEPROM_CRC_t;

typedef struct
{
	uint8_t Logical_Addr_Start;
	uint8_t Logical_Addr_End;
	EEPROM_CRC_t CRC_Info;
}EEPROM_BkArea_t;

typedef struct
{
	EEPROM_BkArea_t EEPROM_Bk_area_Info;
}Device_Conf_t;

static const uint16_t CRC16Tab[256] =
{
	0x0000,0x1021,0x2042,0x3063,0x4084,0x50a5,0x60c6,0x70e7,
	0x8108,0x9129,0xa14a,0xb16b,0xc18c,0xd1ad,0xe1ce,0xf1ef,
	0x1231,0x0210,0x3273,0x2252,0x52b5,0x4294,0x72f7,0x62d6,
	0x9339,0x8318,0xb37b,0xa35a,0xd3bd,0xc39c,0xf3ff,0xe3de,
	0x2462,0x3443,0x0420,0x1401,0x64e6,0x74c7,0x44a4,0x5485,
	0xa56a,0xb54b,0x8528,0x9509,0xe5ee,0xf5cf,0xc5ac,0xd58d,
	0x3653,0x2672,0x1611,0x0630,0x76d7,0x66f6,0x5695,0x46b4,
	0xb75b,0xa77a,0x9719,0x8738,0xf7df,0xe7fe,0xd79d,0xc7bc,
	0x48c4,0x58e5,0x6886,0x78a7,0x0840,0x1861,0x2802,0x3823,
	0xc9cc,0xd9ed,0xe98e,0xf9af,0x8948,0x9969,0xa90a,0xb92b,
	0x5af5,0x4ad4,0x7ab7,0x6a96,0x1a71,0x0a50,0x3a33,0x2a12,
	0xdbfd,0xcbdc,0xfbbf,0xeb9e,0x9b79,0x8b58,0xbb3b,0xab1a,
	0x6ca6,0x7c87,0x4ce4,0x5cc5,0x2c22,0x3c03,0x0c60,0x1c41,
	0xedae,0xfd8f,0xcdec,0xddcd,0xad2a,0xbd0b,0x8d68,0x9d49,
	0x7e97,0x6eb6,0x5ed5,0x4ef4,0x3e13,0x2e32,0x1e51,0x0e70,
	0xff9f,0xefbe,0xdfdd,0xcffc,0xbf1b,0xaf3a,0x9f59,0x8f78,
	0x9188,0x81a9,0xb1ca,0xa1eb,0xd10c,0xc12d,0xf14e,0xe16f,
	0x1080,0x00a1,0x30c2,0x20e3,0x5004,0x4025,0x7046,0x6067,
	0x83b9,0x9398,0xa3fb,0xb3da,0xc33d,0xd31c,0xe37f,0xf35e,
	0x02b1,0x1290,0x22f3,0x32d2,0x4235,0x5214,0x6277,0x7256,
	0xb5ea,0xa5cb,0x95a8,0x8589,0xf56e,0xe54f,0xd52c,0xc50d,
	0x34e2,0x24c3,0x14a0,0x0481,0x7466,0x6447,0x5424,0x4405,
	0xa7db,0xb7fa,0x8799,0x97b8,0xe75f,0xf77e,0xc71d,0xd73c,
	0x26d3,0x36f2,0x0691,0x16b0,0x6657,0x7676,0x4615,0x5634,
	0xd94c,0xc96d,0xf90e,0xe92f,0x99c8,0x89e9,0xb98a,0xa9ab,
	0x5844,0x4865,0x7806,0x6827,0x18c0,0x08e1,0x3882,0x28a3,
	0xcb7d,0xdb5c,0xeb3f,0xfb1e,0x8bf9,0x9bd8,0xabbb,0xbb9a,
	0x4a75,0x5a54,0x6a37,0x7a16,0x0af1,0x1ad0,0x2ab3,0x3a92,
	0xfd2e,0xed0f,0xdd6c,0xcd4d,0xbdaa,0xad8b,0x9de8,0x8dc9,
	0x7c26,0x6c07,0x5c64,0x4c45,0x3ca2,0x2c83,0x1ce0,0x0cc1,
	0xef1f,0xff3e,0xcf5d,0xdf7c,0xaf9b,0xbfba,0x8fd9,0x9ff8,
	0x6e17,0x7e36,0x4e55,0x5e74,0x2e93,0x3eb2,0x0ed1,0x1ef0
};

uint16_t CRC16_CalCulate_MultiByte(uint8_t *buf, int len)
{
	int counter;
	uint16_t crc = 0;
	for (counter = 0; counter < len; counter++)
		crc = (crc << 8) ^ CRC16Tab[((crc >> 8) ^ *buf++) & 0x00FF];
	return crc;
}


void CRC16_Init(void *pDeviceConf)
{
	Device_Conf_t *MyEE = (Device_Conf_t *)pDeviceConf;
	EEPROM_CRC_t *Crc_info = &(MyEE->EEPROM_Bk_area_Info.CRC_Info);

	Crc_info->count = 0;
	Crc_info->result = 0;
}
void CRC16_CalCulate_OneByte(uint8_t OneByteData, void *pDeviceConf)
{
	Device_Conf_t *MyEE = (Device_Conf_t *)pDeviceConf;
	EEPROM_CRC_t *Crc_info = &(MyEE->EEPROM_Bk_area_Info.CRC_Info);
	uint16_t crc = Crc_info->result;

	Crc_info->count++;
	Crc_info->result = (crc << 8) ^ CRC16Tab[((crc >> 8) ^ OneByteData) & 0x00FF];
}
uint16_t Get_CRC16_Result(void *pDeviceConf)
{
	Device_Conf_t *MyEE = (Device_Conf_t *)pDeviceConf;
	EEPROM_CRC_t *Crc_info = &(MyEE->EEPROM_Bk_area_Info.CRC_Info);
	return Crc_info->result;
}
#endif

#ifdef CRC_TEST2

/*********************************************************************\
* Function  : ComTp_Calculate16BitCRC
*
* Parameters: 	uint16_t Seed_du16,
uint16_t Polynomial_du16,
uint8_t* Data_du8,
uint16_t NumberOfBytes_du16,
uint16_t StartOffSet_du16
*
* Returns   : None
*
* Description:
*
*
\*********************************************************************/
uint16_t ComTp_Calculate16BitCRC(uint16_t Seed_du16,
	uint16_t Polynomial_du16,
	uint8_t* Data_du8,
	uint16_t NumberOfBytes_du16,
	uint16_t StartOffSet_du16)
{
	uint32_t i;
	uint16_t j;
	uint16_t CalculatedCRC_ldu16 = Seed_du16;

	for (i = StartOffSet_du16; i < (StartOffSet_du16 + NumberOfBytes_du16); i++)
	{
		CalculatedCRC_ldu16 ^= (uint16_t)(((uint16_t)Data_du8[i] & (uint16_t)0x00FF) << 8);

		for (j = 0; j < 8; j++)
		{
			if ((CalculatedCRC_ldu16 & 0x8000) > 0)
			{
				CalculatedCRC_ldu16 <<= 1;
				CalculatedCRC_ldu16 ^= Polynomial_du16;
			}
			else
			{
				CalculatedCRC_ldu16 <<= 1;
			}
		}
	}

	return CalculatedCRC_ldu16;
}

#endif

#ifdef UART_TEST

void Uart_Thread(x3::Object<IComLib> pComm)
{
	
}

#endif
int test()
{
	uint16_t ret1= 0x12, ret2 = 0x21;

#ifdef SPDLOG_TEST
	x3::Object<ILogLib> pLog(clsidLogLib);

	if (pLog)
	{
		pLog->CreateLogFile();
		pLog->info("Welcome to spdlog!");
		pLog->error("Some error message with arg{}..", 1);
		pLog->warn("Easy padding in numbers like {:08%d}", 12);
		pLog->SaveLogFile();
		pLog->CreateLogFile();
		pLog->critical("Support for int: {0:d};  hex: {0:x};  oct: {0:o}; bin: {0:b}", 42);
		pLog->info("Support for floats {:03.2f}", 1.23456);
		pLog->info("Positional args are {%s} {%s}..", "too", "supported");
		pLog->info("{:<30}", "left aligned");
	}
#endif

#ifdef CRC_TEST
	uint8_t Buff[9] = { 0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38,0x39 };
	uint8_t i = 0;
	Device_Conf_t DeviceNVM_Conf;

	ret1 = CRC16_CalCulate_MultiByte(Buff,9);

	CRC16_Init(&DeviceNVM_Conf);
	for (i = 0; i < 9; i++)
	{
		CRC16_CalCulate_OneByte(Buff[i], &DeviceNVM_Conf);
	}
	ret2 = Get_CRC16_Result(&DeviceNVM_Conf);
#endif
	
#ifdef CRC_TEST2
	uint8_t Buff[] = { 0x02, 0x01, 0xA0, 0x01, 0xFF, 0x04, 0x00, 0x1D, 0x00, 0x00, 0x00, 0x10, 0x50, 0x03 };
	
	ret2 = ComTp_Calculate16BitCRC(0xFFFF, 0x1021, Buff, 10,1);
#endif

#ifdef UART_TEST

	TSerialPortID MySerialPort;
	uint8_t SendBuff[] = { 0x02, 0x01, 0xA0, 0x01, 0xFF, 0x04, 0x00, 0x1D, 0x00, 0x00, 0x00, 0x10, 0x50, 0x03 };
	uint8_t ReceBuff[100];

	if (pComm)
	{
		if (0 == pComm->SerialPortOpen((char *)L"COM3", 9600, &MySerialPort))
		{
			pComm->SerialPortWriteBuffer(MySerialPort, SendBuff, 14);
			pComm->SerialPortReadBuffer(MySerialPort, ReceBuff, 16);
			pComm->SerialPortClose(MySerialPort);
		}
		else
		{
			printf("open_error[%x][%x]\n", ret1, ret2);
		}
	}
	

	//std::thread UARTThread(&Uart_Thread, pComm);

	//UARTThread.join();

#endif

	printf("Goodbye[%x][%x]\n",ret1,ret2);
	
	return 0;
}

int main()
{
    x3::loadScanPlugins();

    test();

    x3::unloadScanPlugins();

	while(1)
	{
		Sleep(100);
	}

	return 0;

}


