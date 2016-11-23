#include <string>
#include <vector>
class PatchWriter
{
public:
	PatchWriter(std::string &outputFile, std::string &patchFile);
	void operator<<(std::vector<unsigned char> &data);
	void operator<<(int data);
	void close(void);
	~PatchWriter(void);
protected:
	std::string fileName;
	std::string BuiltBy;
};