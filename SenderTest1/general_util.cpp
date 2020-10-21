

#include "general_util.h"
#include <iostream>



//********************************************************************************************************************************
//********************************************************************************************************************************

std::map<RESP_STATUS, std::pair<std::string, SAMP_BOOLEAN>> respStatusMeaningMap = {
	{C_STORE_SUCCESS, std::make_pair("C-STORE Success.", SAMP_TRUE)},
	{C_STORE_WARNING_ELEMENT_COERCION, std::make_pair("Warning: Element Coersion... Continuing.", SAMP_TRUE)},
	{C_STORE_WARNING_INVALID_DATASET, std::make_pair("Warning: Invalid Dataset... Continuing.", SAMP_TRUE) },
	{C_STORE_WARNING_ELEMENTS_DISCARDED, std::make_pair("Warning: Elements Discarded... Continuing.", SAMP_TRUE)},
	{C_STORE_FAILURE_REFUSED_NO_RESOURCES, std::make_pair("ERROR: REFUSED, NO RESOURCES.  ASSOCIATION ABORTING.", SAMP_FALSE)},
	{C_STORE_FAILURE_INVALID_DATASET, std::make_pair("ERROR: INVALID_DATASET.  ASSOCIATION ABORTING.", SAMP_FALSE)},
	{C_STORE_FAILURE_CANNOT_UNDERSTAND, std::make_pair("ERROR: CANNOT UNDERSTAND.  ASSOCIATION ABORTING.", SAMP_FALSE)},
	{C_STORE_FAILURE_PROCESSING_FAILURE, std::make_pair("ERROR: PROCESSING FAILURE.  ASSOCIATION ABORTING.", SAMP_FALSE)}
};
void PrintError(std::string A_string, MC_STATUS A_status)
{
	std::string prefix = "                              ";
	/*
	 *  Need process ID number for messages
	 */
#ifdef UNIX
	sprintf(prefix, "PID %d", getpid());
#endif
	if (A_status == -1)
	{
		std::cout << prefix << "\t" << A_string << std::endl;
	}
	else
	{
		std::cout << prefix << "\t" << A_string << ":\n";
		std::cout << prefix << "\t" << MC_Error_Message(A_status) << std::endl;
	}
}

void PrintStatus(std::string A_string, MC_STATUS A_status)
{
	if (A_status != MC_NORMAL_COMPLETION)
	{
		std::cout << A_string << std::endl;
	}
}
void PrintStatus(std::string A_string, bool statusFlag)
{
	if (statusFlag)
	{
		std::cout << A_string << std::endl;
	}
}

SAMP_BOOLEAN ReadResponseMessages(STORAGE_OPTIONS*  A_options, int A_associationID, int A_timeout, InstanceNode** A_list, InstanceNode* A_node)
{
	SAMP_BOOLEAN    sampBool;
	int             responseMessageID;
	char*           responseService = (char*)"";
	MC_COMMAND      responseCommand;
	static char     affectedSOPinstance[UI_LENGTH + 2];
	unsigned int    dicomMsgID;
	InstanceNode*   node = (InstanceNode*)A_node;
	returnValue = SAMP_TRUE;
	returnFlag = SAMP_FALSE;
	
	/*
	 *  Wait for response
	 */
	waitForResponse(A_associationID, A_timeout, &responseMessageID, responseService, &responseCommand);
	//

	getDicomMsgID(responseMessageID, &dicomMsgID);

	getAffectedSopInstance(&responseMessageID, affectedSOPinstance, sizeof(affectedSOPinstance));
	
	node = *A_list;
	while (isNodeWithAffectedSopInstanceID(&node, dicomMsgID, affectedSOPinstance));

	freeMessageIfNodeIsNull(node, &responseMessageID, &dicomMsgID);


	sampBool = CheckResponseMessage(responseMessageID, &node->status, node->statusMeaning, sizeof(node->statusMeaning), node);
	

	freeMessage(responseMessageID);

	fflush(stdout);
	return returnValue;
}

SAMP_BOOLEAN CheckResponseMessage(int A_responseMsgID, unsigned int* A_status, char* A_statusMeaning, size_t A_statusMeaningLength, InstanceNode* node)
{
	SAMP_BOOLEAN returnBool = SAMP_TRUE;
	if (!returnFlag)
	{

		returnBool = getResponseMessageStatus(A_responseMsgID, A_status, A_statusMeaning, A_statusMeaningLength);

		/* MC_Get_Value_To_UInt worked.  Check the response status */
		mapResponseStatusToGetMeaning(A_statusMeaning, A_statusMeaningLength, A_status, &returnBool);

		fflush(stdout);
		if (!returnBool)
		{
			node->failedResponse = SAMP_TRUE;
		}
		node->failedResponse = SAMP_FALSE;
	}
	
	return returnBool;
}

SAMP_BOOLEAN TestCmdLine(int A_argc, char* A_argv[], STORAGE_OPTIONS* A_options)
{
	int       i = 0, argCount = 0;
	SAMP_BOOLEAN returnBool = SAMP_TRUE;

	if (A_argc < 3)
	{
		return SAMP_FALSE;
	}

	/*
	 * Set default values
	 */
	A_options->StartImage = 0;
	A_options->StopImage = 0;

	strcpy(A_options->LocalAE, "MERGE_STORE_SCU");
	strcpy(A_options->RemoteAE, "MERGE_STORE_SCP");
	strcpy(A_options->ServiceList, "Storage_SCU_Service_List");

	A_options->RemoteHostname[0] = '\0';
	A_options->RemotePort = -1;

	A_options->Verbose = SAMP_FALSE;
	A_options->StorageCommit = SAMP_FALSE;
	A_options->ListenPort = 1115;
	A_options->ResponseRequested = SAMP_FALSE;
	A_options->StreamMode = SAMP_FALSE;
	A_options->Username[0] = '\0';
	A_options->Password[0] = '\0';

	A_options->UseFileList = SAMP_FALSE;
	A_options->FileList[0] = '\0';

	/*
	 * Loop through each arguement
	 */
	//First argument always Options->RemoteAE
	setRemoteAE(&A_argc, A_argv, A_options);
	//std::cout << ":RemoteAE set \t\t" << A_options->RemoteAE << std::endl;
	//Second argument always Start image number 
	setStartImageNumber(&A_argc, A_argv, A_options);
	//std::cout << ":StartImageNumber set \t\t" << A_options->StartImage << std::endl;
	//Third argument always Stop image number
	setStopImageNumber(&A_argc, A_argv, A_options, &returnBool);
	//std::cout << ":StopImageNumber set \t\t" << A_options->StopImage << std::endl;

	for (i = 1; i < A_argc; i++)
	{
		setLocalPortNumber(A_argv, i, A_options);
		setRemoteHostName(A_argv, i, A_options);
		setRemotePortNumber(A_argv, i, A_options);
	}

	return returnBool;

}


//****************************************************************************************************************************

//****************************************************************************************************************************
//****************************************************************************************************************************
void setLocalPortNumber(char* argv[], int pos, STORAGE_OPTIONS* options)
{
	if (!strcmp(argv[pos], "-b") || !strcmp(argv[pos], "-B"))
	{
		/*
		 * Local Port Number
		 */
		pos++;
		options->ListenPort = atoi(argv[pos]);

	}
}

void setRemoteHostName(char* argv[], int pos, STORAGE_OPTIONS* options)
{
	if (!strcmp(argv[pos], "-n") || !strcmp(argv[pos], "-N"))
	{
		/*
		 * Remote Host Name
		 */
		pos++;
		strcpy(options->RemoteHostname, argv[pos]);
	}
}

void setRemotePortNumber(char* argv[], int pos, STORAGE_OPTIONS* options)
{
	if (!strcmp(argv[pos], "-p") || !strcmp(argv[pos], "-P"))
	{
		/*
		 * Remote Port Number
		 */
		pos++;
		options->RemotePort = atoi(argv[pos]);

	}
}

void waitForResponse(int associationID, int timeout, int* responseMsgID, char* responseService, MC_COMMAND* responseCmd)
{
	MC_STATUS mcStatus;
	mcStatus = MC_Read_Message(associationID, timeout, responseMsgID, &responseService, responseCmd);
	if (mcStatus == MC_TIMEOUT) {
		returnValue = (SAMP_TRUE);
		returnFlag = SAMP_TRUE;
	}

	if (mcStatus != MC_NORMAL_COMPLETION)
	{
		PrintError("MC_Read_Message failed", mcStatus);
		fflush(stdout);
		returnValue = (SAMP_FALSE);
		returnFlag = SAMP_TRUE;
	}
}

void getDicomMsgID(int responseMsgID, unsigned int* dicomMsgID)
{
	MC_STATUS mcStatus;
	if (!returnFlag)
	{
		mcStatus = MC_Get_Value_To_UInt(responseMsgID, MC_ATT_MESSAGE_ID_BEING_RESPONDED_TO, dicomMsgID);
		if (mcStatus != MC_NORMAL_COMPLETION)
		{
			PrintError("MC_Get_Value_To_UInt for Message ID Being Responded To failed.  Unable to process response message.", mcStatus);
			fflush(stdout);
			returnValue = (SAMP_TRUE);
			returnFlag = SAMP_TRUE;
		}
	}
}

void getAffectedSopInstance(int* responseMsgID, char* affectedSopID, int bufferSize)
{
	MC_STATUS mcStatus;
	if (!returnFlag)
	{
		mcStatus = MC_Get_Value_To_String(*responseMsgID, MC_ATT_AFFECTED_SOP_INSTANCE_UID, bufferSize, affectedSopID);
		if (mcStatus != MC_NORMAL_COMPLETION)
		{
			PrintError("MC_Get_Value_To_String for affected SOP instance failed.  Unable to process response message.", mcStatus);
			fflush(stdout);
			returnValue = (SAMP_TRUE);
			returnFlag = SAMP_TRUE;
		}
	}
}

bool isNodeWithAffectedSopInstanceID(InstanceNode** node, int dicomMsgID, char* affectedSOPinstance)
{
	
	if ((*node)->dicomMsgID == dicomMsgID)
	{
		if (!strcmp(affectedSOPinstance, (*node)->SOPInstanceUID))
		{
			return false;
		}
	}
	*node = (*node)->Next;
	return ((*node) != NULL);
	
}

void freeMessageIfNodeIsNull(InstanceNode* node, int* responseMsgID, unsigned int* dicomMsgID)
{
	if (!returnFlag)
	{
		if (!node)
		{
			printf("Message ID Being Responded To tag does not match message sent over association: %u\n", *dicomMsgID);
			MC_Free_Message(responseMsgID);
			fflush(stdout);
			returnFlag = (SAMP_TRUE);
			returnFlag = SAMP_TRUE;
		}
		node->responseReceived = SAMP_TRUE;
	}
}

void freeMessage(int responseMsgID)
{
	MC_STATUS mcStatus;;
	if (!returnFlag)
	{
		mcStatus = MC_Free_Message(&responseMsgID);
		if (mcStatus != MC_NORMAL_COMPLETION)
		{
			PrintError("MC_Free_Message failed for response message", mcStatus);
			fflush(stdout);
			returnValue = (SAMP_TRUE);
			returnFlag = SAMP_TRUE;
		}
	}
}

void setRemoteAE(int* argc, char* argv[], STORAGE_OPTIONS* options)
{
	strcpy(options->RemoteAE, argv[1]);
}

void setStartImageNumber(int* argc, char* argv[], STORAGE_OPTIONS* options)
{	
	options->StartImage = atoi(argv[2]);
}

void setStopImageNumber(int* argc, char* argv[], STORAGE_OPTIONS* options, SAMP_BOOLEAN* returnVal)
{
	options->StopImage = atoi(argv[3]);
	if (options->StopImage < options->StartImage)
	{
		printf("Image stop number must be greater than or equal to image start number.\n");
		*returnVal = SAMP_FALSE;
	}
}

SAMP_BOOLEAN getResponseMessageStatus(int responseMsgID, unsigned int* status, char* statusMeaning, size_t statusMeaningLength)
{
	MC_STATUS mcStatus;
	mcStatus = MC_Get_Value_To_UInt(responseMsgID, MC_ATT_STATUS, status);
	if (mcStatus != MC_NORMAL_COMPLETION)
	{
		/* Problem with MC_Get_Value_To_UInt */
		PrintError("MC_Get_Value_To_UInt for response status failed", mcStatus);
		strncpy(statusMeaning, "Unknown Status", statusMeaningLength);
		fflush(stdout);
		return SAMP_FALSE;
	}
	return SAMP_TRUE;
}

void mapResponseStatusToGetMeaning(char* statusMeaning, size_t statusMeaningLength, unsigned int* status, SAMP_BOOLEAN* returnVal)
{
	std::pair<std::string, SAMP_BOOLEAN> statusMsg;
	statusMsg = respStatusMeaningMap[*status];
	if (statusMsg.first == "") {
		sprintf(statusMeaning, "Warning: Unknown status (0x%04x)... Continuing.", *status);
		*returnVal = SAMP_FALSE;
	}
	else {
		strncpy(statusMeaning, statusMsg.first.c_str(), statusMeaningLength);
		*returnVal = statusMsg.second;
	}
}
