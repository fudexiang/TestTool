#pragma once
#ifndef _LOGLIB_H_
#define _LOGLIB_H_

#include <module/classmacro.h>
#include <ComLib/IComLib.h>

class CComLib: public IComLib
{
	X3BEGIN_CLASS_DECLARE(CComLib, clsidComLib)
		X3DEFINE_INTERFACE_ENTRY(IComLib)
		X3END_CLASS_DECLARE()

protected:
	CComLib();
	virtual ~CComLib();
public:

	/** Initialize the specified serial port at the specified baud rate, 8 data bits, no parity, 1 stop bit.
	* @param String_Device_File_Name The name of the serial device, like "/dev/ttyS0" or "COM1".
	* @param Baud_Rate The desired baud rate. The function may fail if this is not a standard value.
	* @param Pointer_Serial_Port_ID On output, contain the opened serial port identifier.
	* @return 0 if the serial port was correctly initialized,
	* @return -1 if an error happened. Use errno to find the error.
	*/
	virtual int SerialPortOpen(char *String_Device_File_Name, unsigned int Baud_Rate, TSerialPortID *Pointer_Serial_Port_ID);

	/** Read a byte from the specified serial port.
	* @param Serial_Port_ID The serial port to read from.
	* @return The read byte.
	* @note This is a blocking function.
	*/
	virtual unsigned char SerialPortReadByte(TSerialPortID Serial_Port_ID);

	/** Read several bytes from the specified serial port.
	* @param Serial_Port_ID The serial port to read from.
	* @param Pointer_Buffer On output, contain the read bytes.
	* @param Bytes_Count How many bytes to read.
	* @note The function will block until all requested bytes are read.
	*/
	virtual void SerialPortReadBuffer(TSerialPortID Serial_Port_ID, void *Pointer_Buffer, unsigned int Bytes_Count);

	/** Write a byte to the specified serial port.
	* @param Serial_Port_ID The serial port to write to.
	* @param Byte The byte to send.
	*/
	virtual void SerialPortWriteByte(TSerialPortID Serial_Port_ID, unsigned char Byte) ;

	/** Write several bytes to the specified serial port.
	* @param Serial_Port_ID The serial port to write to.
	* @param Pointer_Buffer The data to write.
	* @param Bytes_Count How many bytes to write.
	*/
	virtual void SerialPortWriteBuffer(TSerialPortID Serial_Port_ID, void *Pointer_Buffer, unsigned int Bytes_Count);

	/** Check whether a byte was received by the serial port.
	* @param Serial_Port_ID The serial port to read from.
	* @param Pointer_Available_Byte Store On output, contain the received byte if there was one available.
	* @return 0 if no byte was received (and Pointer_Available_Byte has unknown value),
	* @return 1 if a byte is available (in this case the byte is stored into Pointer_Available_Byte).
	*/
	virtual int SerialPortIsByteAvailable(TSerialPortID Serial_Port_ID, unsigned char *Pointer_Available_Byte);

	/** Release the specified serial port.
	* @param Serial_Port_ID The serial port to close.
	* @warning The function does not restore the serial port previous parameters.
	*/
	virtual void SerialPortClose(TSerialPortID Serial_Port_ID) ;
};

#endif