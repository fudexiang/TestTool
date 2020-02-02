#include <module/plugininc.h>		/* important */
#include "ComLib.h"

CComLib::CComLib()
{

}

CComLib::~CComLib()
{
	
}


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

//-------------------------------------------------------------------------------------------------
// Public functions
//-------------------------------------------------------------------------------------------------
int CComLib::SerialPortOpen(char *String_Device_File_Name, unsigned int Baud_Rate, TSerialPortID *Pointer_Serial_Port_ID)
{
	HANDLE COM_Handle;
	DCB COM_Parameters;
	COMMTIMEOUTS Timing_Parameters;
	char String_Full_Device_Name[128];

	// Access to the raw COM device
	//sprintf(String_Full_Device_Name, "\\\\.\\%s", String_Device_File_Name);

	// Open the serial port and set all access rights
	//COM_Handle = CreateFile((LPCSTR) String_Device_File_Name, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	COM_Handle = CreateFile(L"COM1", GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	if (COM_Handle == INVALID_HANDLE_VALUE) return -1; // Error : can't access to the serial port

													   // Configure port
	COM_Parameters.DCBlength = sizeof(DCB);
	COM_Parameters.fBinary = 1; // Must be set to 1 or Windows becomes angry
	COM_Parameters.fParity = 0; // No parity
								// Ignore modem signals
	COM_Parameters.fOutxCtsFlow = 0;
	COM_Parameters.fOutxDsrFlow = 0;
	COM_Parameters.fDtrControl = DTR_CONTROL_DISABLE;
	COM_Parameters.fDsrSensitivity = 0;
	COM_Parameters.fTXContinueOnXoff = 0;
	COM_Parameters.fOutX = 0;
	COM_Parameters.fInX = 0;
	COM_Parameters.fErrorChar = 0;
	COM_Parameters.fNull = 0;
	COM_Parameters.fRtsControl = RTS_CONTROL_DISABLE;
	COM_Parameters.fAbortOnError = 0;
	COM_Parameters.fDummy2 = 0;
	COM_Parameters.wReserved = 0;
	COM_Parameters.XonLim = 0;
	COM_Parameters.XoffLim = 0;
	COM_Parameters.ByteSize = 8; // 8 bits of data
	COM_Parameters.Parity = NOPARITY; // Parity check disabled
	COM_Parameters.StopBits = ONESTOPBIT;
	COM_Parameters.XonChar = 0;
	COM_Parameters.XoffChar = 0;
	COM_Parameters.ErrorChar = 0;
	COM_Parameters.EofChar = 0;
	COM_Parameters.EvtChar = 0;
	COM_Parameters.wReserved1 = 0;

	// Set transmit and receive speed
	COM_Parameters.BaudRate = Baud_Rate;

	// Set new parameters
	SetCommState(COM_Handle, &COM_Parameters);

	// Make reads non blocking
	Timing_Parameters.ReadIntervalTimeout = MAXDWORD; // According to MSDN, make the ReadFile() function returns immediately
	Timing_Parameters.ReadTotalTimeoutMultiplier = 0;
	Timing_Parameters.ReadTotalTimeoutConstant = 0;
	Timing_Parameters.WriteTotalTimeoutMultiplier = 0;
	Timing_Parameters.WriteTotalTimeoutConstant = 0;
	SetCommTimeouts(COM_Handle, &Timing_Parameters);

	// No error
	*Pointer_Serial_Port_ID = COM_Handle;
	return 0;
}

unsigned char CComLib::SerialPortReadByte(TSerialPortID Serial_Port_ID)
{
	unsigned char Byte;
	DWORD Number_Bytes_Read;

	do
	{
		ReadFile(Serial_Port_ID, &Byte, 1, &Number_Bytes_Read, NULL);
	} while (Number_Bytes_Read == 0);
	return Byte;
}

void CComLib::SerialPortReadBuffer(TSerialPortID Serial_Port_ID, void *Pointer_Buffer, unsigned int Bytes_Count)
{
	unsigned char Byte, *Pointer_Buffer_Byte = (unsigned char  *)Pointer_Buffer;
	DWORD Number_Bytes_Read;

	while (Bytes_Count > 0)
	{
		// Try to get a byte
		ReadFile(Serial_Port_ID, &Byte, 1, &Number_Bytes_Read, NULL);

		if (Number_Bytes_Read > 0)
		{
			*Pointer_Buffer_Byte = Byte;
			Pointer_Buffer_Byte++;
			Bytes_Count--;
		}
	}
}

void CComLib::SerialPortWriteByte(TSerialPortID Serial_Port_ID, unsigned char Byte)
{
	DWORD Number_Bytes_Written;

	WriteFile(Serial_Port_ID, &Byte, 1, &Number_Bytes_Written, NULL);
}

void CComLib::SerialPortWriteBuffer(TSerialPortID Serial_Port_ID, void *Pointer_Buffer, unsigned int Bytes_Count)
{
	DWORD Number_Bytes_Written;

	WriteFile(Serial_Port_ID, Pointer_Buffer, Bytes_Count, &Number_Bytes_Written, NULL);
}

int CComLib::SerialPortIsByteAvailable(TSerialPortID Serial_Port_ID, unsigned char *Pointer_Available_Byte)
{
	DWORD Number_Bytes_Read;

	ReadFile(Serial_Port_ID, Pointer_Available_Byte, 1, &Number_Bytes_Read, NULL);
	if (Number_Bytes_Read == 0) return 0;
	return 1;
}

void CComLib::SerialPortClose(TSerialPortID Serial_Port_ID)
{
	CloseHandle(Serial_Port_ID);
}
