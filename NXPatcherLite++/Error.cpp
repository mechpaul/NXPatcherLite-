#include <string>
#include <sstream>
#include <Windows.h>

#include "Error.h"

int ErrorHandler(int errorNumber, int MessageBoxOptions, int exitIfReturn)
{
	std::string sError;
	std::ostringstream sStream;
	int result;

	sStream << "ERROR ";
	sStream << errorNumber;
	sStream << ": ";

	switch(errorNumber)
	{
	case NXError::DRAG_DROP_BAD_NAME:
		sStream << "The file you dragged and dropped onto NXPatcher is not a patch file. Please select a new file.";
		break;
	case NXError::DIALOG_BAD_NAME:
		sStream << "The file you selected is not a patch file. Please select a new file.";
		break;
	case NXError::BAD_CRC:
		sStream << "The patch file has a bad checksum. Please try downloading the patch file again.";
		break;
	case NXError::BAD_HEADER:
		sStream << "The file you selected is not a patch file. Please select a new file.";
		break;
	default:
		sStream << "NXPatcher Lite has encountered an unknown error. Please contact Fiel on Southperry.net. http://www.southperry.net/showthread.php?t=13610";
		break;

	}

	sError = sStream.str();

	result = MessageBox(NULL, sError.c_str(), "NXPatcher Lite", MessageBoxOptions);

	if(result == exitIfReturn || result == 0)
	{
		ExitProcess(EXIT_FAILURE);
	}

	return result;
}