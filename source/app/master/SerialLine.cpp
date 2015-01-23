#include <windows.h>
#include "Serial.h"

#include "SerialLine.h"

SerialLine::SerialLine()
{
	mSerial = new CSerial;
}

SerialLine::~SerialLine()
{

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