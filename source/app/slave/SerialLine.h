#ifndef __Serial_Line_h
#define __Serial_Line_h

#include <QObject>

class QThread;
class CSerial;

class SerialLine : public QObject
{
	Q_OBJECT
public:
	SerialLine();
	~SerialLine();

	bool init();
	
	void stop();
	
	void start();

public slots:
	bool send(unsigned char* data, int len);

signals:
	void receiveEvent(unsigned char* data, int len);

protected slots:
		void run();

protected:
	bool read();
	bool write(unsigned char* data, int len);

private:
	bool mRunning;
	unsigned char mOutBuffer[512];
	unsigned char mInBuffer[512];
	int mInLength, mOutLength;

	CSerial *mSerial;

	int mRuning;

	QThread *mWorkerThread;
};

#endif