
#include "general_util.h"
#include "ImageList.h"

//****************************************************************************************************************************
bool hasSentImageRecievedResponse(InstanceNode* A_node);
InstanceNode* goToLastNode(InstanceNode* A_node);
//****************************************************************************************************************************

SAMP_BOOLEAN ImageList::AddFileToList(InstanceNode** A_list, char* A_fname)
{
	InstanceNode*    newNode;
	InstanceNode*    listNode;

	newNode = static_cast<InstanceNode*>(malloc(sizeof(InstanceNode)));
	if (!newNode)
	{
		PrintError("Unable to allocate object to store instance information", MC_NORMAL_COMPLETION);
		return (SAMP_FALSE);
	}

	memset(newNode, 0, sizeof(InstanceNode));

	strncpy_s(newNode->fname, A_fname, sizeof(newNode->fname));
	newNode->fname[sizeof(newNode->fname) - 1] = '\0';

	newNode->responseReceived = SAMP_FALSE;
	newNode->failedResponse = SAMP_FALSE;
	newNode->imageSent = SAMP_FALSE;
	newNode->msgID = -1;
	newNode->transferSyntax = IMPLICIT_LITTLE_ENDIAN;

	if (!*A_list)
	{
		/*
		 * Nothing in the list
		 */
		newNode->Next = *A_list;
		*A_list = newNode;
	}
	else
	{
		listNode = goToLastNode(*A_list);
		

		listNode->Next = newNode;
	}

	return (SAMP_TRUE);
}

SAMP_BOOLEAN ImageList::UpdateNode(InstanceNode* A_node)
{
	MC_STATUS        mcStatus;

	/*
	 * Get DICOM msgID for tracking of responses
	 */
	mcStatus = MC_Get_Value_To_UInt(A_node->msgID, MC_ATT_MESSAGE_ID, &(A_node->dicomMsgID));
	if (mcStatus != MC_NORMAL_COMPLETION)
	{
		//PrintError("MC_Get_Value_To_UInt for Message ID failed", mcStatus);
		A_node->responseReceived = SAMP_TRUE;
		return(SAMP_FALSE);
	}

	A_node->responseReceived = SAMP_FALSE;
	A_node->failedResponse = SAMP_FALSE;
	A_node->imageSent = SAMP_TRUE;

	return (SAMP_TRUE);
}

void ImageList::FreeList(InstanceNode** A_list)
{
	InstanceNode*    node;

	/*
	 * Free the instance list
	 */
	while (*A_list)
	{
		node = *A_list;
		*A_list = node->Next;

		if (node->msgID != -1)
			MC_Free_Message(&node->msgID);

		free(node);
	}
}

int ImageList::GetNumNodes(InstanceNode* A_list)
{
	int            numNodes = 0;
	InstanceNode*  node;

	node = A_list;
	while (node)
	{
		numNodes++;
		node = node->Next;
	}

	return numNodes;
}

int ImageList::GetNumOutstandingRequests(InstanceNode* A_list)
{
	int            outstandingResponseMsgs = 0;
	InstanceNode*  node;

	node = A_list;
	while (node)
	{
		if (hasSentImageRecievedResponse(node))
			outstandingResponseMsgs++;

		node = node->Next;
	}
	return outstandingResponseMsgs;
}

//****************************************************************************************************************************
bool hasSentImageRecievedResponse(InstanceNode* A_node)
{
	return (A_node->imageSent == SAMP_TRUE) && (A_node->responseReceived == SAMP_FALSE);
}

InstanceNode* goToLastNode(InstanceNode* A_node)
{
	while (A_node->Next)
		A_node = A_node->Next;

	return A_node;
}