#ifndef __Modbus_Slave_h
#define __Modbus_Slave_h

/**
 * RTU Modus Protocol on serial line for M-7000 DIO Module
 * The communication Baud Rates range from 1200bps to 115200bps. The
 * parity, data bits and stop bits are fixed as no parity, 8 data bits and 1
 * stop bit
 */

#include <QObject>
 
class SerialLine;
class M7000DIO;

class ModbusSlave : public QObject
{
	Q_OBJECT

	enum {IDLE, WAITING_DELAY, WAITING_REPLY, PROCESSING_REPLY, PROCESSING_ERROR};

public:
	ModbusSlave(M7000DIO *dio, unsigned char address=1);
	~ModbusSlave();

	void start();
	void stop();

public slots:
	void receive(unsigned char* data, int len);

signals:
	void update();
	void error();

protected:
	bool checkRequest(unsigned char* req, int len);
	void processRequest(unsigned char* req, int len);
	void reply(unsigned char* data, int len);
	void frame(unsigned char address, unsigned char func, unsigned char n, unsigned char *data);	
	void frameError(unsigned char address, unsigned char func, unsigned char exception);

private:
	unsigned char mAddress;
	unsigned char mBuffer[256];
	int mDataLength;
	int mState;

	SerialLine *mSerial;
	M7000DIO *mDevice;
};

#endif