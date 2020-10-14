#pragma once
#include "util_structures.h"
#include <string>



void PrintError(std::string A_string, MC_STATUS A_status);
void PrintStatus(std::string A_string, MC_STATUS A_status);
void PrintStatus(std::string A_string, bool statusFlag);

SAMP_BOOLEAN TestCmdLine(int A_argc, char* A_argv[], STORAGE_OPTIONS* A_options);

SAMP_BOOLEAN ReadResponseMessages(STORAGE_OPTIONS*  A_options, int A_associationID, int A_timeout, InstanceNode** A_list, InstanceNode* A_node);
SAMP_BOOLEAN CheckResponseMessage(int A_responseMsgID, unsigned int* A_status, char* A_statusMeaning, size_t A_statusMeaningLength);
