#ifndef __Modbus_Master_h
#define __Modbus_Master_h

/**
 * RTU Modus Protocol on serial line for M-7000 DIO Module
 * The communication Baud Rates range from 1200bps to 115200bps. The
 * parity, data bits and stop bits are fixed as no parity, 8 data bits and 1
 * stop bit
 */

#include <QObject>
 
class QThread;
class CSerial;

class ModbusMaster : public QObject
{
	Q_OBJECT

	enum {IDLE, WAITING_DELAY, WAITING_REPLY, PROCESSING_REPLY, PROCESSING_ERROR};

public:
	ModbusMaster(unsigned char address=1);
	~ModbusMaster();

	bool init();
	
	bool read(unsigned char& v);
	bool write(unsigned char v);

	void startPolling();
	void stopPolling();

signals:
	void risingEdge(unsigned char v);
	void fallingEdge();

public slots:
	void doPolling();

protected:
	void frame(unsigned char address, unsigned char func, unsigned char *data,int n);	
	bool readData(unsigned char* data, int& len);
	bool writeData(unsigned char* data, int len);

private:
	unsigned char mDefaultAddress;

	CSerial *mSerial;

	QThread* mWorkThread;
	int mPolling;
};

#endif