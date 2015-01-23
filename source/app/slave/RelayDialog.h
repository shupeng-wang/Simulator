#ifndef __Relay_Dialog_h
#define __Relay_Dialog_h

#include <QDialog>

class CSerial;
class ModbusSlave;
class M7000DIO;
class QCheckBox;
class QSignalMapper;

class RelayDialog : public QDialog
{
	Q_OBJECT
public:
	RelayDialog(int inNum=8, int outNum=8);
	~RelayDialog();

public:

protected:
	void createWidget();
	void createConnects();

protected slots:
	void outputChanged(int id);
	void inputChanged();

private:
	QCheckBox **mInput;
	QCheckBox **mOutput;
	int mInputNumber;
	int mOutputNumber;
	
	QSignalMapper *mSignalMapper;

	M7000DIO *mDevice;
	ModbusSlave *mSlave;
};

#endif