#pragma once

#include "util_structures.h"

class ImageList
{
public:
	ImageList() = default;	
	static SAMP_BOOLEAN AddFileToList(InstanceNode** A_list, char* A_fname);
	static SAMP_BOOLEAN UpdateNode(InstanceNode* A_node);
	static int GetNumNodes(InstanceNode* A_list);
	static int GetNumOutstandingRequests(InstanceNode* A_list);
	static void FreeList(InstanceNode** A_list);	
};