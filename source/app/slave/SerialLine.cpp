#include <windows.h>
#include "Serial.h"

#include "SerialLine.h"

#define COM_CONF_FILENAME	"com.txt"
#define TAG_NAME 		"name"
#define TAG_BAUD		"baud"
#define TAG_PARITY		"parity"
#define TAG_STOP_BITS	"stop_bits"
#define TAG_DATA_BITS	"data_bits"

//////////////////////////////////////////////////////////////////////////////////
SerialLine::SerialLine()
{
	mSerial = new CSerial;
}

SerialLine::~SerialLine()
{

}

bool SerialLine::init()
{
	mSerial = new CSerial;

	std::string tag, name;
	int baud, parity, stop_bits, data_bits;
	bool ret;

	std::ifstream file;
	file.open(COM_CONF_FILENAME, std::ios::in|std::ios::nocreate);
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

bool SerialLine::send(unsigned char* data, int len)
{	
	return write(data, len);
}

bool SerialLine::write(unsigned char* data, int len)
{
	DWORD dwWrite;
	LONG lLastError = mSerial->Write(data, len, &dwWrite, 0, 100);
	if ( len == (int)dwWrite ) {
		return true;
	}
	return false;
}

bool SerialLine::read() 
{
	const int MAX_READ_NUM = 512;
	LONG lLastError;
	DWORD dwRead;

	unsigned char *buf = mInBuffer;
	mInLength = 0;
	while (1) {
		dwRead = 0;
		lLastError = mSerial->Read(buf, MAX_READ_NUM, &dwRead, 0, 100);
		if ( dwRead == MAX_READ_NUM ) {
			continue;
		}
		if ( dwRead == 0 ) {
			return false;
		}
		else {
			mInLength = dwRead;
			return true;
		}
	}
	
}

void SerialLine::run()
{
	while ( mRunning ) {
		if ( read() ) {
			emit receive(mInBuffer, mInLength);
		}
		::Sleep(100);
	}
}

void SerialLine::start()
{
	mRunning = true;

	QThread::start();
}

void SerialLine::stop()
{
	mRunning = false;
}