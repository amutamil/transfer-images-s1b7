
#include "ReadImage.h"
#include "general_util.h"
#include "ReadMessageFromFile.h"
#include <iostream>
#include <cstdio>


SAMP_BOOLEAN ReadImage::read(STORAGE_OPTIONS* A_options, int A_appID, InstanceNode* A_node)
{
	FORMAT_ENUM             format = UNKNOWN_FORMAT;
	SAMP_BOOLEAN            sampBool = SAMP_FALSE;
	MC_STATUS               mcStatus;
	ReadMessageFromFile*	rmff = nullptr;

	format = IMPLICIT_LITTLE_ENDIAN_FORMAT;

	A_node->mediaFormat = SAMP_FALSE;
	sampBool = ReadMessageFromFile::readFromFile(A_options, A_node->fname, format, &A_node->msgID, &A_node->transferSyntax, &A_node->imageBytes);


	if (sampBool == SAMP_TRUE)
	{
		mcStatus = MC_Get_Value_To_String(A_node->msgID, MC_ATT_SOP_CLASS_UID, sizeof(A_node->SOPClassUID), A_node->SOPClassUID);
		PrintStatus("MC_Get_Value_To_String for SOP Class UID failed", mcStatus);

		
		mcStatus = MC_Get_Value_To_String(A_node->msgID, MC_ATT_SOP_INSTANCE_UID, sizeof(A_node->SOPInstanceUID), A_node->SOPInstanceUID);
		PrintStatus("MC_Get_Value_To_String for SOP Instance UID failed", mcStatus);

	}
	fflush(stdout);
	return sampBool;
}