enum NXError {
	DRAG_DROP_BAD_NAME = 1001,
	DIALOG_BAD_NAME,
	BAD_CRC,
	BAD_HEADER,
};

int ErrorHandler(int errorNumber, int MessageBoxOptions, int exitIfReturn = IDCANCEL);