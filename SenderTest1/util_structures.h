#pragma once

/*
 * Module constants
 */

 /* DICOM VR Lengths */
#define AE_LENGTH 16
#define UI_LENGTH 64
#define SVC_LENGTH 130
#define STR_LENGTH 100
#define WORK_SIZE (64*1024)

#define TIME_OUT 30

#if defined(_WIN32)
#define BINARY_READ "rb"
#define BINARY_WRITE "wb"
#define BINARY_APPEND "rb+"
#define BINARY_READ_APPEND "a+b"
#define BINARY_CREATE "w+b"
#define TEXT_READ "r"
#define TEXT_WRITE "w"
#else
#define BINARY_READ "r"
#define BINARY_WRITE "w"
#define BINARY_APPEND "r+"
#define BINARY_READ_APPEND "a+"
#define BINARY_CREATE "w+"
#define TEXT_READ "r"
#define TEXT_WRITE "w"
#endif

 /*
  * Merge DICOM Toolkit Includes
  */
#include "../mc3inc/mc3media.h"
#include "../mc3inc/mc3msg.h"
#include "../mc3inc/mergecom.h"
#include "../mc3inc/diction.h"
#include "../mc3inc/mc3services.h"
#include "../mc3inc/mc3items.h"
//#include "../mc3inc/mcstatus.h"
/*
 * Used to identify the format of an object
 */
typedef enum
{
	UNKNOWN_FORMAT = 0,
	MEDIA_FORMAT = 1,
	IMPLICIT_LITTLE_ENDIAN_FORMAT,
	IMPLICIT_BIG_ENDIAN_FORMAT,
	EXPLICIT_LITTLE_ENDIAN_FORMAT,
	EXPLICIT_BIG_ENDIAN_FORMAT
} FORMAT_ENUM;


typedef enum
{
	SAMP_TRUE = 1,
	SAMP_FALSE = 0
} SAMP_BOOLEAN;

typedef enum
{
	SAMP_SUCCESS = 1,
	SAMP_FAILURE = 0
} SAMP_STATUS;

/*
 * Structure to store local application information
 */
typedef struct stor_scu_options
{
	int     StartImage;
	int     StopImage;
	int     ListenPort; /* for StorageCommit */
	int     RemotePort;

	char    RemoteAE[AE_LENGTH + 2];
	char    LocalAE[AE_LENGTH + 2];
	char    RemoteHostname[STR_LENGTH];
	char    ServiceList[SVC_LENGTH + 2];
	char    FileList[1024];
	char    Username[STR_LENGTH];
	char    Password[STR_LENGTH];

	SAMP_BOOLEAN UseFileList;
	SAMP_BOOLEAN Verbose;
	SAMP_BOOLEAN StorageCommit;
	SAMP_BOOLEAN ResponseRequested;
	SAMP_BOOLEAN StreamMode;

	AssocInfo       asscInfo;
} STORAGE_OPTIONS;

/*
 * Structure to maintain list of instances sent & to be sent.
 * The structure keeps track of all instances and is used
 * in a linked list.
 */
typedef struct instance_node
{
	int    msgID;                       /* messageID of for this node */
	char   fname[1024];                 /* Name of file */
	TRANSFER_SYNTAX transferSyntax;     /* Transfer syntax of file */

	char   SOPClassUID[UI_LENGTH + 2];    /* SOP Class UID of the file */
	char   serviceName[48];             /* Merge DICOM Toolkit service name for SOP Class */
	char   SOPInstanceUID[UI_LENGTH + 2]; /* SOP Instance UID of the file */

	size_t       imageBytes;            /* size in bytes of the file */

	unsigned int dicomMsgID;            /* DICOM Message ID in group 0x0000 elements */
	unsigned int status;                /* DICOM status value returned for this file. */
	char   statusMeaning[STR_LENGTH];   /* Textual meaning of "status" */
	SAMP_BOOLEAN responseReceived;      /* Bool indicating we've received a response for a sent file */
	SAMP_BOOLEAN failedResponse;        /* Bool saying if a failure response message was received */
	SAMP_BOOLEAN imageSent;             /* Bool saying if the image has been sent over the association yet */
	SAMP_BOOLEAN mediaFormat;           /* Bool saying if the image was originally in media format (Part 10) */

	char	patientName[50];			/*Set the patient name*/
	int		patientID;					/*Set the patient ID*/

	struct instance_node* Next;         /* Pointer to next node in list */

} InstanceNode;

typedef struct CALLBACKINFO
{
	FILE*   fp;
	char    fileName[512];
	/*
	 * Note! The size of this buffer impacts toolkit performance.
	 *       Higher values in general should result in increased performance of reading files
	 */
	size_t  bytesRead;
	size_t  bufferLength;

	char*   buffer;
} CBinfo;
