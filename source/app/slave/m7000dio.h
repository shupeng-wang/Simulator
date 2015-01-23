#ifndef __M7000_Dio_h
#define __M7000_Dio_h

#define FUNC_READ_COILS				0x01
#define FUNC_READ_INPUTS			0x02
#define FUNC_READ_REGISTERS			0x03
#define FUNC_READ_INPUT_REGISTERS	0x04
#define FUNC_WRITE_SINGLE_COIL		0x05	
#define FUNC_WRITE_COILS			0x0f
#define FUNC_RW_SETTINGS			0x46

class M7000DIO 
{
	enum {DI=0, DO=1};
public:
	M7000DIO(unsigned char address=1);

	bool read(int start, int num, unsigned char *data);

	bool write(int start, int num, unsigned char *data);

	unsigned char getAddress() {
		return mAddress;
	}

	int getInChannel(int c) {
		return (mDI[0] >> c) & 0x01;
	}
	void setInChannel(int c, int v) {
		unsigned char mask, t;
		t = (v == 0 ? 0 : 1) << c;
		mask = 1 << c;
		mDI[0] = (mDI[0] & ~mask) + t;
	}
	int getOutChannel(int c) {
		return (mDO[0] >> c) & 0x01;
	}
	void setOutChannel(int c, int v) {
		unsigned char mask, t;
		t = (v == 0 ? 0 : 1) << c;
		mask = 1 << c;
		mDO[0] = (mDO[0] & ~mask) + t;
	}



	int getInNumber() {
		return mInNumber;
	}
	int getOutNumber() {
		return mOutNumber;
	}

	unsigned char mDO[2];
	unsigned char mDI[2];
	int mInNumber, mOutNumber;
	unsigned char mAddress;

protected:
	void read(int start, unsigned char *data, int type);
	void write(int start, unsigned char *data, int type);
};


#endif