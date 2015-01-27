#ifndef __Relay_Dialog_h
#define __Relay_Dialog_h

#include <QDialog>

class ModbusMaster;
class QPushButton;
class QLineEdit;
class QStatusBar;

class MasterDialog : public QDialog
{
	Q_OBJECT
public:
	MasterDialog(int inNum=8, int outNum=8);
	~MasterDialog();

	bool init();

protected:
	void createWidget();
	void createConnects();

protected slots:
	void read();
	void write();
	void startDetect(unsigned char v);
	void stopDetect();
	void startPolling();
	void stopPolling();

private:
	QLineEdit *mReadEdit;
	QLineEdit *mWriteEdit;
	QPushButton *mReadButton;
	QPushButton *mWriteButton;
	QStatusBar *mStatusBar;
	
	ModbusMaster *mMaster;
};

#endif