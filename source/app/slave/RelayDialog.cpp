#include <QtGui>
#include "RelayDialog.h"
#include "ModbusSlave.h"
#include "m7000dio.h"

RelayDialog::RelayDialog(int inNum, int outNum)
{
	mDevice = new M7000DIO;
	mSlave = new ModbusSlave(mDevice);
	if ( mSlave->init() ) {
		mSlave->start();
	}
	else {
		delete mSlave;
		mSlave = 0;
	}

	mInputNumber = mDevice->getInNumber();
	mOutputNumber = mDevice->getOutNumber();
	createWidget();
	createConnects();
}

RelayDialog::~RelayDialog()
{
	delete mDevice;
}

void RelayDialog::closeEvent(QCloseEvent * event)
{
	if ( mSlave ) {
		mSlave->stop();
		delete mSlave;
	}
	delete mDevice;
	event->accept();
}

void RelayDialog::createWidget()
{
	int i;
	char label[80];
	
	QVBoxLayout *inputLayout = new QVBoxLayout;
	mInput = new QCheckBox*[mInputNumber];
	for ( i = 0; i < mInputNumber; i++ ) {
		sprintf(label, "in %d", i);
		mInput[i] = new QCheckBox(label);
		mInput[i]->setChecked(mDevice->getInChannel(i));
		mInput[i]->setEnabled(true);
		inputLayout->addWidget(mInput[i]);
	}
	QGroupBox *inputGroup = new QGroupBox(tr("Input State"));
	inputGroup->setLayout(inputLayout);

	QVBoxLayout *outputLayout = new QVBoxLayout;
	mOutput = new QCheckBox*[mOutputNumber];
	for ( i = 0; i < mOutputNumber; i++ ) {
		sprintf(label, "out %d", i);
		mOutput[i] = new QCheckBox(label);
		mOutput[i]->setChecked(mDevice->getOutChannel(i));
		mOutput[i]->setEnabled(false);
		outputLayout->addWidget(mOutput[i]);
	}
	QGroupBox *outputGroup = new QGroupBox(tr("Output State"));
	outputGroup->setLayout(outputLayout);

	QHBoxLayout *mainLayout = new QHBoxLayout;
	mainLayout->addWidget(inputGroup);
	mainLayout->addWidget(outputGroup);
	setLayout(mainLayout);
}

void RelayDialog::createConnects()
{
	int i;

	QSignalMapper *mapper = new QSignalMapper(this);
	for ( i = 0; i < mInputNumber; i++ ) {
		connect(mInput[i], SIGNAL(toggled(bool)), mapper, SLOT(map()));
		mapper->setMapping(mInput[i], i);
	}
	connect((mapper), SIGNAL(mapped(int)), this, SLOT(inputChanged(int)));
	mSignalMapper = mapper;

	connect(mSlave, SIGNAL(update()), this, SLOT(outputChanged()));
}

void RelayDialog::inputChanged(int id) 
{
	if ( mInput[id]->isChecked() ) {
		mDevice->setInChannel(id, 1);
	}
	else {
		mDevice->setInChannel(id, 0);
	}
}

void RelayDialog::outputChanged()
{
	for ( int i = 0; i < mOutputNumber; i++ ) {
		mOutput[i]->setChecked(mDevice->getOutChannel(i));
	}
}