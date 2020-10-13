#pragma once

#include "util_structures.h"



class ReadMessageFromFile {
public:
	static SAMP_BOOLEAN readFromFile(STORAGE_OPTIONS*  A_options,
		char*             A_filename,
		FORMAT_ENUM       A_format,
		int*              A_msgID,
		TRANSFER_SYNTAX*  A_syntax,
		size_t*           A_bytesRead);
};