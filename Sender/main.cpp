#pragma warning(disable:4996)
#include "util_structures.h"
#include "general_util.h"
#include "ImageList.h"
#include "ReadImage.h"
#include "ReadMessageFromFile.h"
#include "SendImage.h"

#include <iostream>

//****************************************************************************************************************************
void openAssociation(int* A_applicationID, int* A_associationID, STORAGE_OPTIONS* A_options);
void exitApplicationIfNotNormalCompletion(bool statusBool);
void addImagesToList(STORAGE_OPTIONS* options, ImageList* imgList, InstanceNode** node);
void setNodeResponseParamsOnSucces(InstanceNode* node, int* imgSent);
void abortAndReleaseApplicationOnFailure(char* statusMsg, bool statusBool, int* associationID, int* applicationID);
void closeAssociation(int* A_pplication, int* A_associationID);
//****************************************************************************************************************************

int main(int argc, char* argv[])
{
	SAMP_BOOLEAN            sampBool;
	STORAGE_OPTIONS         options;
	MC_STATUS               mcStatus;
	int                     applicationID = -1, associationID = -1, imageCurrent = 0;
	int                     imagesSent = 0L, totalImages = 0L, fstatus = 0;
	double                  seconds = 0.0;
	void                    *startTime = NULL, *imageStartTime = NULL;
	char                    fname[512] = { 0 };  /* Extra long, just in case */
	char   				    statusMsg[512] = { 0 };
	size_t                  totalBytesRead = 0L;
	InstanceNode            *instanceList = NULL, *node = NULL;
	FILE*                   fp = NULL;

	ReadImage* reader = new ReadImage;
	SendImage* sender = new SendImage;
	ImageList* imgList = new ImageList;

	sampBool = TestCmdLine(argc, argv, &options);
	exitApplicationIfNotNormalCompletion(sampBool == SAMP_FALSE);

	/*Library Must always be initialized first*/
	mcStatus = MC_Library_Initialization(NULL, NULL, NULL);
	exitApplicationIfNotNormalCompletion(mcStatus != MC_NORMAL_COMPLETION);

	mcStatus = MC_Register_Application(&applicationID, options.LocalAE);
	exitApplicationIfNotNormalCompletion(mcStatus != MC_NORMAL_COMPLETION);

	/*AE Registration successfull*/

	addImagesToList(&options, imgList, &instanceList);

	totalImages = imgList->GetNumNodes(instanceList);

	std::cout << "TotalIMages :" << totalImages << std::endl;

	/*open association to Storing Server*/
	openAssociation(&applicationID, &associationID, &options);
	/*mcStatus = MC_Open_Association(applicationID,
		&associationID,
		options.RemoteAE,
		options.RemotePort != -1 ? &options.RemotePort : NULL,
		options.RemoteHostname[0] ? options.RemoteHostname : NULL,
		options.ServiceList);
	exitApplicationIfNotNormalCompletion(mcStatus != MC_NORMAL_COMPLETION);*/

	mcStatus = MC_Get_Association_Info(associationID, &options.asscInfo);
	PrintStatus("MC_Get_Association_Info failed", mcStatus != MC_NORMAL_COMPLETION);

	printf("Connected to remote system [%s], Max operations %d\n\n", options.RemoteAE, options.asscInfo.MaxOperationsInvoked);
	fflush(stdout);

	node = instanceList;

	while (node)
	{
		sampBool = reader->read(&options, applicationID, node);
		if (!sampBool)
		{
			node->imageSent = SAMP_FALSE;
			printf("Can not open image file [%s]\n", node->fname);
			node = node->Next;
			continue;
		}

		sampBool = sender->send(&options, associationID, node);
		sprintf(statusMsg, "Failure in sending file [%s].", node->fname);
		abortAndReleaseApplicationOnFailure(statusMsg, !sampBool, &associationID, &applicationID);
		

		sampBool = imgList->UpdateNode(node);
		sprintf(statusMsg, "Warning, unable to update node with information [%s].", node->fname);
		abortAndReleaseApplicationOnFailure(statusMsg, !sampBool, &associationID, &applicationID);


		setNodeResponseParamsOnSucces(node, &imagesSent);
		/*if (node->imageSent == SAMP_TRUE)
		{
			imagesSent++;
		}
		else
		{
			node->responseReceived = SAMP_TRUE;
			node->failedResponse = SAMP_TRUE;
		}
		*/

		mcStatus = MC_Free_Message(&node->msgID);
		PrintStatus("MC_Free_Message failed for request message", mcStatus != MC_NORMAL_COMPLETION);


		sampBool = ReadResponseMessages(&options, associationID, 1, &instanceList, NULL);
		sprintf(statusMsg, "Failure in reading response message, aborting association.");
		abortAndReleaseApplicationOnFailure(statusMsg, !sampBool, &associationID, &applicationID);


		/*if (options.asscInfo.MaxOperationsInvoked > 0)
		{
			while (imgList->GetNumOutstandingRequests(instanceList) >= options.asscInfo.MaxOperationsInvoked)
			{
				sampBool = ReadResponseMessages(&options, associationID, 10, &instanceList, NULL);
				if (!sampBool)
				{
					printf("Failure in reading response message, aborting association.\n");
					MC_Abort_Association(&associationID);
					MC_Release_Application(&applicationID);
					break;
				}
			}
		}*/
		node = node->Next;
	}
	/*Close Application after sending images*/
	closeAssociation(&applicationID, &associationID);
	/*mcStatus = MC_Close_Association(&associationID);
	if (mcStatus != MC_NORMAL_COMPLETION)
	{
		PrintError("Close association failed", mcStatus);
		MC_Abort_Association(&associationID);
	}
	mcStatus = MC_Release_Application(&applicationID);
	PrintStatus("MC_Release_Application failed", mcStatus != MC_NORMAL_COMPLETION); */
	

	/* Free the node list's allocated memory */
	imgList->FreeList(&instanceList);

	/*
	 * Release all memory used by the tool kit.
	 */
	PrintStatus("Error releasing the library.\n", MC_Library_Release() != MC_NORMAL_COMPLETION);


	delete reader;
	delete sender;
	delete imgList;

	return(EXIT_SUCCESS);
}

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

void addImagesToList(STORAGE_OPTIONS* options, ImageList* imgList, InstanceNode** node)
{
	SAMP_BOOLEAN sampBool;
	char         fname[512] = { 0 };
	for (int imageCurrent = options->StartImage; imageCurrent <= options->StopImage; imageCurrent++)
	{
		sprintf(fname, "%d.img", imageCurrent);
		sampBool = imgList->AddFileToList(node, fname);
		if (!sampBool)
		{
			printf("Warning, cannot add SOP instance to File List, image will not be sent [%s]\n", fname);
		}
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