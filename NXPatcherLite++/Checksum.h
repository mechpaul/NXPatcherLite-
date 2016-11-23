#include <vector>

class Checksum
{
protected:
	const static int polynomial = 0x04C11DB7;
	const static int size = 256;
	unsigned int result;
	std::vector<unsigned int> sbox;
	void InitializeSbox(void);
public:
	Checksum(void);
	void Update(std::vector<unsigned char> &data, int length);
	unsigned int GetResult(void);
};