
#include "SendImage.h"
#include "general_util.h"


//****************************************************************************************************************************
void getSOPClassUID(InstanceNode* node);
void setService(InstanceNode* node);
void setSOPInstanceUID(InstanceNode* node);
void sendMessage(int A_associationID, InstanceNode* node);
//****************************************************************************************************************************

SAMP_BOOLEAN SendImage::send(STORAGE_OPTIONS* A_options, int A_associationID, InstanceNode* A_node)
{
	returnFlag		  = SAMP_FALSE;
	A_node->imageSent = SAMP_FALSE;


	getSOPClassUID(A_node);
	setService(A_node);
	setSOPInstanceUID(A_node);


	/*
	 *  Send the message
	 */
	sendMessage(A_associationID, A_node);

	if (!returnValue)
	{
		A_node->imageSent = SAMP_TRUE;
		fflush(stdout);
		returnValue = SAMP_TRUE;
	}

	return returnValue;
}

void getSOPClassUID(InstanceNode* node)
{
	MC_STATUS mcStatus;
	if (!returnFlag)
	{
		mcStatus = MC_Get_MergeCOM_Service(node->SOPClassUID, node->serviceName, sizeof(node->serviceName));
		if (mcStatus != MC_NORMAL_COMPLETION)
		{
			PrintError("MC_Get_MergeCOM_Service failed", mcStatus);
			fflush(stdout);
			returnValue =  (SAMP_TRUE);
			returnFlag = SAMP_TRUE;
		}
	}
}

void setService(InstanceNode* node)
{
	MC_STATUS mcStatus;
	if (!returnFlag)
	{
		mcStatus = MC_Set_Service_Command(node->msgID, node->serviceName, C_STORE_RQ);
		if (mcStatus != MC_NORMAL_COMPLETION)
		{
			PrintError("MC_Set_Service_Command failed", mcStatus);
			fflush(stdout);
			returnFlag = (SAMP_TRUE);
			returnValue  = (SAMP_TRUE);
		}
	}
}

void setSOPInstanceUID(InstanceNode* node)
{
	MC_STATUS mcStatus;
	if (!returnFlag)
	{
		mcStatus = MC_Set_Value_From_String(node->msgID, MC_ATT_AFFECTED_SOP_INSTANCE_UID, node->SOPInstanceUID);
		if (mcStatus != MC_NORMAL_COMPLETION)
		{
			PrintError("MC_Set_Value_From_String failed for affected SOP Instance UID", mcStatus);
			fflush(stdout);
			returnFlag = (SAMP_TRUE);
			returnValue = (SAMP_TRUE);
		}
	}
}

void sendMessage(int A_associationID, InstanceNode* node)
{
	MC_STATUS mcStatus;
	if (!returnFlag)
	{
		mcStatus = MC_Send_Request_Message(A_associationID, node->msgID);
		if (mcStatus != MC_NORMAL_COMPLETION)
		{
			/* This is a failure condition we can continue with*/
			PrintError("Warning: MC_Send_Request_Message failed", mcStatus);
			fflush(stdout);
			returnValue = (SAMP_FALSE);
			returnFlag = SAMP_TRUE;
		}
	}
}