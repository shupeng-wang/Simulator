#include "m7000dio.h"

M7000DIO::M7000DIO(unsigned char address)
{
	mAddress = address;

	mInNumber = 8;
	mOutNumber = 8;
	
	mDI[0] = mDI[1] = 0;
	mDO[0] = mDO[2] = 0;
}

bool M7000DIO::read(int start, int num, unsigned char *data)
{
	if ( start <= 0x1f ) {
		read(start, data, DO);
	} 
	else if ( start >= 0x20 && start <= 0x3f) {
		read(start-0x20, data, DI);
	}

	return true;
}

void M7000DIO::read(int start, unsigned char* data, int type)
{
	unsigned char *ptr;
	if ( type == M7000DIO::DI ) {
		ptr = &mDI[0];
	}
	else {
		ptr = &mDO[0];
	}

	data[0] = ptr[0];
	data[1] = ptr[1];
}

bool M7000DIO::write(int start, int num, unsigned char *data)
{
	if ( start <= 0x1f ) {
		write(start, data, DO);
	} 
	else if ( start >= 0x20 && start <= 0x3f) {
		write(start-0x20, data, DI);
	}

	return true;
}

void M7000DIO::write(int start, unsigned char* data, int type)
{
	unsigned char *ptr;
	if ( type == M7000DIO::DI ) {
		ptr = &mDI[0];
	}
	else {
		ptr = &mDO[0];
	}

	ptr[0] = data[0];
	ptr[1] = data[0];
}