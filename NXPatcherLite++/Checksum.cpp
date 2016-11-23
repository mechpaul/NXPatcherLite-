#include <vector>
#include "Checksum.h"

Checksum::Checksum(void)
{
	this->result = 0;
	this->InitializeSbox();
}

void Checksum::InitializeSbox(void)
{
	int remain;
	int dividend;
	int bit;

	this->sbox.resize(this->size);

	for(dividend = 0; dividend < this->size; dividend++)
	{
		remain = dividend << 24;
		for(bit = 0; bit < 8; bit++)
		{
			if(remain & 0x80000000)
			{
				remain = (remain << 1) ^ this->polynomial;
			}
			else
			{
				remain = (remain << 1);
			}
		}
		this->sbox[dividend] = remain;
	}
}

void Checksum::Update(std::vector<unsigned char> &block, int length)
{
	int index;
	int pos;

	for(pos = 0; pos < length; pos++)
	{
		index = (this->result >> 0x18) ^ block[pos];
		this->result = (this->result << 0x08) ^ this->sbox[index];
	}
}

unsigned int Checksum::GetResult(void)
{
	return this->result;
}