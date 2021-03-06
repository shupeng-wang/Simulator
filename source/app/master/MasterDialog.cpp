#include <QtGui>
#include "MasterDialog.h"
#include "ModbusMaster.h"

MasterDialog::MasterDialog(int inNum, int outNum)
{
	mMaster = 0;

	createWidget();
	createConnects();
}

MasterDialog::~MasterDialog()
{
	stopPolling();
}

bool MasterDialog::init()
{
	mMaster = new ModbusMaster;
	if ( mMaster && !mMaster->init() ) {
		delete mMaster;
		mMaster = 0;
		return false;
	}
	startPolling();
	return true;
}

void MasterDialog::startPolling()
{
	if ( mMaster ) {
		connect(mMaster, SIGNAL(risingEdge(unsigned char)), this, SLOT(startDetect(unsigned char)));
		connect(mMaster, SIGNAL(fallingEdge()), this, SLOT(stopDetect()));
	}

	mMaster->startPolling();
}

void MasterDialog::stopPolling()
{
	if ( mMaster ) {
		mMaster->stopPolling();
		disconnect(mMaster, 0, 0, 0);
		delete mMaster;
		mMaster = 0;
	}
}

void MasterDialog::createWidget()
{
	
	QHBoxLayout *readLayout = new QHBoxLayout;
	mReadEdit = new QLineEdit;
	mReadButton = new QPushButton(tr("Read"));
	mReadEdit->setMaxLength(1);
	readLayout->addWidget(mReadEdit);
	readLayout->addWidget(mReadButton);

	QHBoxLayout *writeLayout = new QHBoxLayout;
	mWriteEdit = new QLineEdit;
	mWriteButton = new QPushButton(tr("Write"));
	mWriteEdit->setMaxLength(1);
	writeLayout->addWidget(mWriteEdit);
	writeLayout->addWidget(mWriteButton);

	mStatusBar = new QStatusBar(this);

	QVBoxLayout *mainLayout = new QVBoxLayout;
	mainLayout->addLayout(readLayout);
	mainLayout->addLayout(writeLayout);
	mainLayout->addWidget(mStatusBar);
	setLayout(mainLayout);
}

void MasterDialog::createConnects()
{
	connect(mReadButton, SIGNAL(pressed()), this, SLOT(read()));
	connect(mWriteButton, SIGNAL(pressed()), this, SLOT(write()));
}

void MasterDialog::read() 
{
	QString text;
	unsigned char v;
	
	if ( !mMaster ) {
		mStatusBar->showMessage(tr("fail to open com"));
		return;
	}

	if ( !mMaster->read(v) ) {
		mStatusBar->showMessage(tr("error on read"));
		return;
	}
	mStatusBar->showMessage(tr("success in reading"));
	text.setNum(v);
	mReadEdit->setText(text);
}

void MasterDialog::write()
{
	QString text;
	unsigned char v;
	bool ok;

	if ( !mMaster ) {
		mStatusBar->showMessage(tr("fail to open com"));
		return;
	}

	v = mWriteEdit->text().toInt(&ok);
	if ( !ok ) {
		return;
	}

	if ( !mMaster->write(v) ) {
		mStatusBar->showMessage(tr("error on write"));
	}
	else {
		mStatusBar->showMessage(tr("success in writing"));
	}
}

void MasterDialog::startDetect(unsigned char v )
{
	QString msg;
	mStatusBar->showMessage(tr("start"));
	mReadEdit->setText(msg.setNum(v));
}

void MasterDialog::stopDetect()
{
	mStatusBar->showMessage(tr("stop"));
}