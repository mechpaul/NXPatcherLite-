#include <iostream>
#include <Windows.h>
#include <fstream>
#include <vector>
#include <sstream>

#include "Error.h"
#include "Checksum.h"
#include "PatchFile.h"

void WritePatch(char* patchFile)
{
	std::string notice("NXPatcher Lite v2.0 - Written by Fiel - http://www.southperry.net/");
	std::string outputFile;
	int patchCRC = 0xF2F7FBF3;
	std::ofstream o;
	std::vector<unsigned char> v;
	std::ifstream currentEXE;
	std::ifstream fPatch;
	int lengthBase = 0;
	std::vector<unsigned char> base;
	unsigned int zlibCRC;
	Checksum crc;
	int pos;
	int lengthPatch;
	int blockSize = 5000000;
	std::ostringstream cmdline;
	int lengthCurrentEXE;
	char fileName[MAX_PATH] = {0};
	int lengthZlibBlock;

	//Read in the base file stored in the current executable file
	GetModuleFileNameA(NULL, fileName, MAX_PATH);
	currentEXE.open(fileName, std::ios::binary | std::ios::in);
	currentEXE.seekg(0, currentEXE.end);
	lengthCurrentEXE = (int) currentEXE.tellg();
	currentEXE.seekg(lengthCurrentEXE - sizeof(lengthBase), currentEXE.beg);
	currentEXE.read(reinterpret_cast<char*>(&lengthBase), sizeof(lengthBase));
	base.resize(lengthBase);
	currentEXE.seekg(lengthCurrentEXE - sizeof(lengthBase) - lengthBase, currentEXE.beg);
	currentEXE.read(reinterpret_cast<char*>(&base[0]), lengthBase);
	currentEXE.close();

	//Begin the process for creating the patch file.
	outputFile = patchFile;
	outputFile.replace(outputFile.find(".patch"), strlen(".patch"), ".exe");
	o.open(outputFile, std::ios::binary | std::ios::out);
	o.write(reinterpret_cast<char*>(&base[0]), base.size());

	//Begin processing the patch file
	v.resize(blockSize);
	fPatch.open(patchFile, std::ios::binary | std::ios::in);
	fPatch.read(reinterpret_cast<char*>(&v[0]), 12);
	fPatch.read(reinterpret_cast<char*>(&zlibCRC), sizeof(zlibCRC));
	o.write(reinterpret_cast<char*>(&v[0]), 12);
	o.write(reinterpret_cast<char*>(&zlibCRC), sizeof(zlibCRC));

	//Calculate the CRC of the zlib block while writing to the new patch file
	pos = 0;
	fPatch.seekg(0, fPatch.end);
	lengthPatch = (int) fPatch.tellg();
	lengthZlibBlock = lengthPatch - 16;
	fPatch.seekg(16, fPatch.beg);
	while(pos < lengthZlibBlock)
	{
		if(pos + blockSize > lengthZlibBlock)
		{
			blockSize = lengthZlibBlock - pos;
		}
		fPatch.read(reinterpret_cast<char*>(&v[0]), blockSize);
		o.write(reinterpret_cast<char*>(&v[0]), blockSize);
		crc.Update(v, blockSize);
		pos += blockSize;
	}

	//Output the tail of the patch file
	int noticeLength = notice.length();
	o.write(reinterpret_cast<char*>(&notice[0]), notice.length());
	o.write(reinterpret_cast<char*>(&lengthPatch), sizeof(lengthPatch));
	o.write(reinterpret_cast<char*>(&noticeLength), sizeof(noticeLength));
	o.write(reinterpret_cast<char*>(&patchCRC), sizeof(patchCRC));
	o.close();

	if(zlibCRC != crc.GetResult())
	{
		ErrorHandler(NXError::BAD_CRC, MB_OK | MB_ICONEXCLAMATION, IDOK);
		DeleteFile(outputFile.c_str());
		ExitProcess(EXIT_FAILURE);
	}

	int result = MessageBox(NULL, "The pre-patcher is now created.\r\nDo you want to run the pre-patcher now?", "NXPatcher Lite", MB_YESNO | MB_ICONINFORMATION);
	if(result == IDYES)
	{
		ShellExecute(NULL, "open", outputFile.c_str(), NULL, NULL, SW_SHOW);
	}
}



void NoArgHandler(void)
{
	OPENFILENAME ofn;
	std::string szFile;
	std::string currentDirectory;
	std::string szFileTitle;
	int result;

	szFileTitle.resize(MAX_PATH);
	szFile.resize(MAX_PATH);
	currentDirectory.resize(MAX_PATH);

	GetCurrentDirectory(currentDirectory.length(), &currentDirectory[0]);

	result = 1;
	do {
		ZeroMemory(&ofn , sizeof(ofn));
		ofn.lStructSize = sizeof(ofn);
		ofn.hwndOwner = NULL;
		ofn.lpstrFile = &szFile[0];
		ofn.lpstrFile[0] = '\0';
		ofn.nMaxFile = szFile.length();
		ofn.lpstrFilter = "Nexon Patch (*.patch)\0*.patch";
		ofn.nFilterIndex = 1;
		ofn.lpstrFileTitle = &szFileTitle[0];
		ofn.nMaxFileTitle = szFileTitle.length();
		ofn.lpstrInitialDir = currentDirectory.c_str();
		ofn.Flags = OFN_PATHMUSTEXIST|OFN_FILEMUSTEXIST;
		
		result = GetOpenFileName(&ofn);

		if(result)
		{
			if(CheckHeader(ofn.lpstrFile) == 0)
			{
				ErrorHandler(NXError::BAD_HEADER, MB_OKCANCEL | MB_ICONEXCLAMATION);
				continue;
			}			
			WritePatch(ofn.lpstrFile);
			ExitProcess(EXIT_SUCCESS);
		}
	}while(result);
}

void PreArgHandler(char* fileName)
{
	if(CheckHeader(fileName) == 0)
	{
		ErrorHandler(NXError::BAD_HEADER, MB_OKCANCEL | MB_ICONEXCLAMATION);
		NoArgHandler();
	}
	else
	{
		WritePatch(fileName);
	}
	
}


int __stdcall WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	if(__argc == 2)
	{
		PreArgHandler(__argv[1]);
	}
	else
	{
		NoArgHandler();
	}
}