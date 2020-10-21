#pragma warning(disable:4996)

#include "ReadMessageFromFile.h"
#include "general_util.h"


//****************************************************************************************************************************
static MC_STATUS NOEXP_FUNC StreamToMsgObj(int A_msgID,
	void*      A_CBinformation,
	int        A_isFirst,
	int*       A_dataSize,
	void**     A_dataBuffer,
	int*       A_isLast);
SAMP_BOOLEAN isFirstFile(int isFirstFlag, CBinfo* A_cbinfo);
SAMP_BOOLEAN isLastFile(int eof, int* isLast);

void openEmptyMessage(int* A_msgID);
void openFile(CBinfo* A_cbinfo, char* A_filename, int* A_msgID);
void setIOBuffer(CBinfo* A_cbinfo);
void setBufferLength(CBinfo* A_cbinfo);
void allocateMemoryToBuffer(CBinfo* A_cbinfo);
void openEmptyMessage(int* A_msgID);
void closeFile(CBinfo* A_cbinfo);
void freeBuffer(CBinfo* A_cbinfo);
//****************************************************************************************************************************

SAMP_BOOLEAN ReadMessageFromFile::readFromFile(STORAGE_OPTIONS*  A_options,
	char*             A_filename,
	FORMAT_ENUM       A_format,
	int*              A_msgID,
	TRANSFER_SYNTAX*  A_syntax,
	size_t*           A_bytesRead)
{
	MC_STATUS       mcStatus;
	unsigned long   errorTag = 0;
	CBinfo*         callbackInfo = new CBinfo;
	*callbackInfo = { 0 };
	int             retStatus = 0;
	returnFlag = SAMP_FALSE;

	/*
	 * Set the format
	 */
	*A_syntax = IMPLICIT_LITTLE_ENDIAN;

	/*
	 * Open an empty message object to load the image into
	 */
	openEmptyMessage(A_msgID);

	openFile(callbackInfo,A_filename,A_msgID);

	setIOBuffer(callbackInfo);

	setBufferLength(callbackInfo);

	allocateMemoryToBuffer(callbackInfo);

	if (!returnFlag)
	{
		mcStatus = MC_Stream_To_Message(*A_msgID, 0x00080000, 0xFFFFFFFF, *A_syntax, &errorTag, (void*)callbackInfo, StreamToMsgObj);

		closeFile(callbackInfo);
		freeBuffer(callbackInfo);


		*A_bytesRead = callbackInfo->bytesRead;
		fflush(stdout);
		returnValue = SAMP_TRUE;
		delete callbackInfo;
	}
	return returnValue;

}

static MC_STATUS NOEXP_FUNC StreamToMsgObj(int A_msgID,
	void*      A_CBinformation,
	int        A_isFirst,
	int*       A_dataSize,
	void**     A_dataBuffer,
	int*       A_isLast)
{
	size_t          bytesRead;
	CBinfo*         callbackInfo = (CBinfo*)A_CBinformation;

	isFirstFile(A_isFirst, callbackInfo);

	bytesRead = fread(callbackInfo->buffer, 1, callbackInfo->bufferLength, callbackInfo->fp);
	if (ferror(callbackInfo->fp))
	{
		perror("\tRead error when streaming message from file.\n");
		return MC_CANNOT_COMPLY;
	}

	isLastFile(feof(callbackInfo->fp), A_isLast);

	*A_dataBuffer = callbackInfo->buffer;
	*A_dataSize = (int)bytesRead;
	callbackInfo->bytesRead += bytesRead;

	return MC_NORMAL_COMPLETION;
}

SAMP_BOOLEAN isFirstFile(int isFirstFlag, CBinfo* A_cbinfo)
{
	if (isFirstFlag)
	{
		A_cbinfo->bytesRead = 0L;
		return SAMP_TRUE;
	}
	else
		return SAMP_FALSE;
}

SAMP_BOOLEAN isLastFile(int eof, int* isLast)
{
	if (eof)
	{
		*isLast = 1;
		return SAMP_TRUE;
	}
	else
	{
		*isLast = 0;
		return SAMP_FALSE;
	}
}


void openEmptyMessage(int* A_msgID)
{
	MC_STATUS mcStatus;
	mcStatus = MC_Open_Empty_Message(A_msgID);
	if (mcStatus != MC_NORMAL_COMPLETION)
	{
		PrintError("Unable to open empty message", mcStatus);
		fflush(stdout);
		returnFlag = SAMP_TRUE;
		returnValue = SAMP_FALSE;
	}
}

void openFile(CBinfo* A_cbinfo, char* A_filename, int* A_msgID)
{
	if (!returnFlag)
	{	
		A_cbinfo->fp = fopen(A_filename, BINARY_READ);
		if (!A_cbinfo->fp)
		{
			printf("ERROR: Unable to open %s.\n", A_filename);
			MC_Free_Message(A_msgID);
			fflush(stdout);
			returnFlag =  SAMP_TRUE;
			returnValue = SAMP_FALSE;
		}
	}
}

void setIOBuffer(CBinfo* A_cinfo)
{
	if (!returnFlag)
	{
		int retStatus = setvbuf(A_cinfo->fp, (char*)NULL, _IOFBF, 32868);
		PrintStatus("WARNING: Unable to set ID buffering on input file.", retStatus != 0);
	}
}

void setBufferLength(CBinfo* A_cbinfo)
{
	MC_STATUS mcStatus;
	if (A_cbinfo->bufferLength == 0)
	{
		int length = 0;

		mcStatus = MC_Get_Int_Config_Value(WORK_BUFFER_SIZE, &length);
		if (mcStatus != MC_NORMAL_COMPLETION)
		{
			length = WORK_SIZE;
		}
		A_cbinfo->bufferLength = length;
	}
}

void allocateMemoryToBuffer(CBinfo* A_cbinfo)
{
	if (!returnFlag)
	{
		A_cbinfo->buffer = (char*)malloc(A_cbinfo->bufferLength);
		if (A_cbinfo->buffer == NULL)
		{
			printf("ERROR: failed to allocate file read buffer [%d] kb", (int)A_cbinfo->bufferLength);
			fflush(stdout);
			returnFlag = SAMP_TRUE;
			returnValue = SAMP_FALSE;
		}
	}
}

void closeFile(CBinfo* A_cbinfo)
{
	if (A_cbinfo->fp)
		fclose(A_cbinfo->fp);
}

void freeBuffer(CBinfo* A_cbinfo)
{
	if (A_cbinfo->buffer)
		free(A_cbinfo->buffer);
}