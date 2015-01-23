#ifndef __Serial_Line_h
#define __Serial_Line_h

#include <QThread>

class CSerial;

class SerialLine : public QThread
{
	Q_OBJECT
public:
	SerialLine();
	~SerialLine();

	bool init();
	
	void stop();

	bool send(unsigned char* data, int len);
	
public slots:
	void start();
		
signals:
	void receive(unsigned char* data, int len);

protected:
	void run();
	bool read();
	bool write(unsigned char* data, int len);

private:
	bool mRunning;
	unsigned char mOutBuffer[512];
	unsigned char mInBuffer[512];
	int mInLength, mOutLength;

	CSerial *mSerial;

	int mRuning;
};

#endif