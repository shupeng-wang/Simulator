#include <Windows.h>
#include <fstream>
#include <qthread.h>
#include "crc.h"
#include "Serial.h"
#include "ModbusMaster.h"

#define COM_CONF_FILENAME "com.txt"
#define TAG_NAME 		"name"
#define TAG_BAUD		"baud"
#define TAG_PARITY		"parity"
#define TAG_STOP_BITS	"stop_bits"
#define TAG_DATA_BITS	"data_bits"

////////////////////////////////////////////////////////////////////////////////
ModbusMaster::ModbusMaster(unsigned char address)
{
	mDefaultAddress = address;
	mWorkThread = 0;
}

ModbusMaster::~ModbusMaster()
{
	mSerial->Close();
	delete mSerial;
}

bool ModbusMaster::init()
{
	mSerial = new CSerial;

	std::string tag, name;
	int baud, parity, stop_bits, data_bits;
	bool ret;

	std::ifstream file;
	file.open(COM_CONF_FILENAME, std::ios::in);
	if ( !file ) {
		return false;
	}

	ret = true;
	while ( file >> tag ) {
		if ( !tag.compare(TAG_NAME) ) {
			file >> name;
		}
		else if ( !tag.compare(TAG_BAUD) ) {
			file >> baud;
		}
		else if ( !tag.compare(TAG_PARITY) ) {
			file >> parity;
		}
		else if ( !tag.compare(TAG_STOP_BITS) ) {
			file >> stop_bits;
		}
		else if ( !tag.compare(TAG_DATA_BITS) ) {
			file >> data_bits;
		}
		else {
			ret = false;
		}
	}
	
	file.close();
	if ( !ret ) {
		return false;
	}

	if ( mSerial->Open(name.c_str()) != ERROR_SUCCESS ) {
		return false;
	}

	if ( mSerial->Setup() != ERROR_SUCCESS ) {
		return false;
	}

	return true;
}


void ModbusMaster::frame(unsigned char address, unsigned char func, 
	                     unsigned char *data, int n)
{
	unsigned char *paddress = data - 2;
	*paddress = address;
	*(paddress+1) = func;
	
	// CRC check 
	unsigned char v1, v2;
	uint16 crc_val = crc16(paddress, n+2);
	v1 = crc_val >> 8;
	v2 = crc_val & 0xff;
	*(data+n) = v1;
	*(data+n+1)= v2;
}	                   

bool ModbusMaster::write(unsigned char v)
{
	unsigned char buf[512];
	unsigned char *data = buf+2;
	int len = 0;
	unsigned char func_code = 0x0f;

	data[0] = data[1] = 0;	// for DO output
	data[2] = 0; data[3] = 8;	// number of output channels
	data[4] = 1;				// byte count
	data[5] = v;
	frame(mDefaultAddress, func_code, data, 6);

	QMutexLocker locker(&mMutex);

	if ( !writeData(buf, 6+4) ) {
		return false;
	}

	wait(100);				// sleep a while for response

	if ( !readData(buf, len) || len < 6) {
		return false;
	}

	if ( (buf[0] != mDefaultAddress) || (buf[1] != func_code) ) {
		return false;
	}

	return true;
}

bool ModbusMaster::read(unsigned char& v)
{
	unsigned char buf[512];
	unsigned char *data = buf+2;
	int len = 0;
	unsigned char func_code = 0x01;

	data[0] = 0x00; data[1] = 0x20;	// read coil for DI channel
	data[2] = 0x00; data[3] = 0x08;	// number of output channels
	frame(mDefaultAddress, func_code, data, 4);

	QMutexLocker locker(&mMutex);

	if ( !writeData(buf, 4+4) ) {
		return false;
	}

	wait(100);				// sleep a while for respose

	if ( !readData(buf, len) || len < 4 ) {
		return false;
	}

	if ( (buf[0] != mDefaultAddress) || (buf[1] != func_code) ||
		 (buf[2] != 1) ) {
		return false;
	}

	v = buf[3];
	return true;
}

bool ModbusMaster::readData(unsigned char* pdata, int& len)
{
	const int MAX_READ_NUM = 512;
	LONG lLastError;
	DWORD dwRead;

	while (1) {
		dwRead = 0;
		lLastError = mSerial->Read(pdata, MAX_READ_NUM, &dwRead, 0, 100);
		if ( dwRead == MAX_READ_NUM ) {
			continue;
		}
		if ( dwRead == 0 ) {
			return false;
		}
		else {
			len = dwRead;
			break;
		}
	}

	if ( len <= 2 ) {
		return false;
	}

	// CRC check 
	unsigned char v1, v2;
	uint16 crc_val = crc16(pdata, len-2);
	v1 = crc_val >> 8;
	v2 = crc_val & 0xff;
	if ( (pdata[len-2] != v1) || (pdata[len-1] != v2)) {
		return false;
	}
	return true;
}

bool ModbusMaster::writeData(unsigned char* pdata, int len)
{
	DWORD dwWrite;

	LONG lLastError = mSerial->Write(pdata, len, &dwWrite, 0, 100);
	if ( len == (int)dwWrite ) {
		return true;
	}
	return false;
}


void ModbusMaster::doPolling()
{
	static unsigned char state = 0;
	unsigned char v;
	while ( mPolling ) {
		if ( !read(v) ) {
			continue;
		}
		if ( (state == 0) && (v > 0) ) {		// rising edge
			state = v;
			emit risingEdge(v);
		}
		else if ( (state > 0) && ( v == 0) ) {	// falling edge
			state = v;
			emit fallingEdge();
		}
	}
}

void ModbusMaster::startPolling()
{
	if ( mWorkThread ) {
		return;
	}

	mWorkThread = new QThread;
	connect(mWorkThread, SIGNAL(started()), this, SLOT(doPolling()));
	this->moveToThread(mWorkThread);

	mPolling = 1;
	mWorkThread->start();
}

void ModbusMaster::stopPolling()
{
	mPolling = 0;

	mWorkThread->quit();
	mWorkThread->wait();
	delete mWorkThread;

	mWorkThread = 0;
}

void ModbusMaster::wait(int ms)
{
	::Sleep(ms);
}