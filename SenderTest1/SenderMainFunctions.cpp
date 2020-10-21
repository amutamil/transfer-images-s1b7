
#include<iostream>
#include"SenderMainFunctions.h"
void openAssociation(int* A_applicationID, int* A_associationID, STORAGE_OPTIONS* A_options)
{
	MC_STATUS mcStatus;
	mcStatus = MC_Open_Association(*A_applicationID,
		A_associationID,
		A_options->RemoteAE,
		A_options->RemotePort != -1 ? &A_options->RemotePort : NULL,
		A_options->RemoteHostname[0] ? A_options->RemoteHostname : NULL,
		A_options->ServiceList);
	exitApplicationIfNotNormalCompletion(mcStatus != MC_NORMAL_COMPLETION);
}

void exitApplicationIfNotNormalCompletion(bool statusBool)
{
	if (statusBool) {
		std::cout << "Something went wrong with library/association." << std::endl;
		exit(EXIT_FAILURE);
	}
}



void setNodeResponseParamsOnSucces(InstanceNode* node, int* imgSent)
{
	if (node->imageSent == SAMP_TRUE)
	{
		(*imgSent)++;
	}
	else
	{
		node->responseReceived = SAMP_TRUE;
		node->failedResponse = SAMP_TRUE;
	}
}



void abortAndReleaseApplicationOnFailure(char* statusMsg, bool statusBool, int* associationID, int* applicationID)
{
	if (statusBool) {
		printf("%s\n", statusMsg);
		MC_Abort_Association(associationID);
		MC_Release_Application(applicationID);
		exit(EXIT_FAILURE);
	}
}

void closeAssociation(int* A_applicationID, int* A_associationID)
{
	MC_STATUS mcStatus;
	mcStatus = MC_Close_Association(A_associationID);
	if (mcStatus != MC_NORMAL_COMPLETION)
	{
		PrintError("Close association failed", mcStatus);
		MC_Abort_Association(A_associationID);
	}
	mcStatus = MC_Release_Application(A_applicationID);
	PrintStatus("MC_Release_Application failed", mcStatus != MC_NORMAL_COMPLETION);
}
void addImagesToList(STORAGE_OPTIONS* options, ImageList* imgList, InstanceNode** node)
{
	SAMP_BOOLEAN sampBool;
	char         fname[512] = { 0 };
	for (int imageCurrent = options->StartImage; imageCurrent <= options->StopImage; imageCurrent++)
	{
		sprintf_s(fname, "%d.img", imageCurrent);
		sampBool = imgList->AddFileToList(node, fname);
		if (!sampBool)
		{
			printf("Warning, cannot add SOP instance to File List, image will not be sent [%s]\n", fname);
		}
	}
}
