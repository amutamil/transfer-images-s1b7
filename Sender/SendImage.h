#pragma once

#include "util_structures.h"

class SendImage {
public:
	static SAMP_BOOLEAN send(STORAGE_OPTIONS* A_options, int A_associationID, InstanceNode* A_node);
};
