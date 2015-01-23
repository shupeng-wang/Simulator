#include <QtGui>
#include "RelayDialog.h"
#include "ModbusSlave.h"
#include "m7000dio.h"

RelayDialog::RelayDialog(int inNum, int outNum)
{
	mDevice = new M7000DIO;
	mSlave = new ModbusSlave(mDevice, 1);
	mInputNumber = mDevice->getInNumber();
	mOutputNumber = mDevice->getOutNumber();

	mSlave->start();

	createWidget();
	createConnects();
}

RelayDialog::~RelayDialog()
{
	mSlave->stop();
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
		mInput[i]->setEnabled(false);
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
		mOutput[i]->setEnabled(true);
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
	for ( i = 0; i < mOutputNumber; i++ ) {
		connect(mOutput[i], SIGNAL(toggled(bool)), mapper, SLOT(map()));
		mapper->setMapping(mOutput[i], i);
	}
	connect((mapper), SIGNAL(mapped(int)), this, SLOT(outputChanged(int)));
	mSignalMapper = mapper;

	connect(mSlave, SIGNAL(update()), this, SLOT(inputChanged()));
}

void RelayDialog::outputChanged(int id) 
{
	if ( mOutput[id]->isChecked() ) {
		mDevice->setOutChannel(id, 1);
	}
	else {
		mDevice->setOutChannel(id, 0);
	}
}

void RelayDialog::inputChanged()
{
	for ( int i = 0; i < mInputNumber; i++ ) {
		mInput[i]->setChecked(mDevice->getInChannel(i));
	}
}