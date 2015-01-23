#include "crc.h"
#include "m7000dio.h"
#include "SerialLine.h"
#include "ModbusSlave.h"

////////////////////////////////////////////////////////////////////////////////
ModbusSlave::ModbusSlave(M7000DIO* dio, unsigned char address)
{
	mDevice = dio;
	mSerial = new SerialLine;

	connect(mSerial, SIGNAL(receive(unsigned char*, int)), 
		    this, SLOT(receive(unsigned char*, int)));	
	//connect(this, SIGNAL(send(unsigned char*, int)), mSerial, SLOT(send(unsigned char*, int)));
}

ModbusSlave::~ModbusSlave()
{
	delete mSerial;
}

void ModbusSlave::start()
{
	// Set up serial port
	mSerial->start();
}

void ModbusSlave::stop()
{
	mSerial->stop();
}

void ModbusSlave::receive(unsigned char *data, int len)
{
	if (!checkRequest(data, len)) {
		emit error();
		return;
	}

	processRequest(data, len-2);	// remove crc field
}

bool ModbusSlave::checkRequest(unsigned char *data, int len)
{
	// Check address
	if ( (data[0] != mDevice->getAddress()) && (data[0] != 0)) {
		return false;
	}

	// CRC check 
	unsigned char v1, v2;
	uint16 crc_val = crc16(data, len);
	v1 = crc_val >> 8;
	v2 = crc_val & 0xff;
	if ( (v1 != data[len-2]) || (v2 != data[len-1]) ) {
		return false;
	}
}

void ModbusSlave::processRequest(unsigned char* request, int len) 
{
	unsigned char buf[2];
	unsigned char *pAddress = request;
	unsigned char *pFunc = pAddress + 1;
	unsigned char *pData = pFunc + 1;

	short start, num;
	switch( *pFunc ) 
	{
	case FUNC_READ_COILS:
		start = *((short*)pData);
		num = *((short*)pData+2);
		if (mDevice->read(start, num, buf)) {
			frame(mDevice->getAddress(), 0x01, 1, buf);
		}
		else {
			frameError(mDevice->getAddress(), 0x81, 0);
		}
		reply(mBuffer, mDataLength);
		break;
	case FUNC_WRITE_COILS:
		start = *((short*)pData);
		num = *((short*)pData+2);
		buf[0] = *(pData+4);
		if (mDevice->write(start, num, &buf[0])) {
			frame(mDevice->getAddress(), FUNC_WRITE_COILS, 4, pData);
		}
		else {
			frameError(mDevice->getAddress(), 0x8F, 0);
		}
		reply(mBuffer, mDataLength);
		break;
	default:
		;
	}
}

void ModbusSlave::frame(unsigned char address, unsigned char func, 
	                    unsigned char n, unsigned char *data)
{
	mBuffer[0] = address;
	mBuffer[1] = func;
	mBuffer[2] = n;
	unsigned char *ptr = mBuffer+3;
	for ( unsigned char i = 0; i < n; i++ ) {
		ptr[i] = data[i];
	}

	// CRC check 
	unsigned char v1, v2;
	uint16 crc_val = crc16(mBuffer, n+3);
	v1 = crc_val >> 8;
	v2 = crc_val & 0xff;
	mBuffer[n+4] = v1;
	mBuffer[n+5] = v2;

	mDataLength = n+6;
}	                   

void ModbusSlave::frameError(unsigned char address, unsigned char func, 
	                         unsigned char exception)
{
	mBuffer[0] = address;
	mBuffer[1] = func;
	mBuffer[2] = exception;

	// CRC check 
	unsigned char v1, v2;
	uint16 crc_val = crc16(mBuffer, 3);
	v1 = crc_val >> 8;
	v2 = crc_val & 0xff;
	mBuffer[3] = v1;
	mBuffer[4] = v2;

	mDataLength = 5;

}

void ModbusSlave::reply(unsigned char* data, int len)
{
	mSerial->send(data, len);
}