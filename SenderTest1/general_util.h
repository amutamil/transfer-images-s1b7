#pragma once
#include "util_structures.h"
#include <string>
#include <iostream>
#include <utility>
#include <map>

static SAMP_BOOLEAN returnValue;
static SAMP_BOOLEAN returnFlag;


//********************************************************************************************************************************
void setLocalPortNumber(char* argv[], int pos, STORAGE_OPTIONS* options);
void setRemoteHostName(char* argv[], int pos, STORAGE_OPTIONS* options);
void setRemotePortNumber(char* argv[], int pos, STORAGE_OPTIONS* options);
void setRemoteAE(int* argc, char* argv[], STORAGE_OPTIONS* options);
void setStartImageNumber(int* argc, char* argv[], STORAGE_OPTIONS* options);
void setStopImageNumber(int* argc, char* argv[], STORAGE_OPTIONS* options, SAMP_BOOLEAN* returnVal);

void waitForResponse(int associationID, int timeout, int* responseMsgID, char* responseService, MC_COMMAND* responseCmd);
void getDicomMsgID(int responseMsgID, unsigned int* dicomMsgID);
void getAffectedSopInstance(int* responseMsgID, char* affectedSopID, int bufferSize);
bool isNodeWithAffectedSopInstanceID(InstanceNode** node, int dicomMsgID, char* affectedSOPinstance);
void freeMessageIfNodeIsNull(InstanceNode* node, int* responseMsgID, unsigned int* dicomMsgID);
void freeMessage(int responseMsgID);

SAMP_BOOLEAN getResponseMessageStatus(int responseMsgID, unsigned int* status, char* statusMeaning, size_t statusMeaningLength);
void mapResponseStatusToGetMeaning(char* statusMeaning, size_t statusMeaningLength, unsigned int * status, SAMP_BOOLEAN* returnVal);

void PrintError(std::string A_string, MC_STATUS A_status);
void PrintStatus(std::string A_string, MC_STATUS A_status);
void PrintStatus(std::string A_string, bool statusFlag);

SAMP_BOOLEAN TestCmdLine(int A_argc, char* A_argv[], STORAGE_OPTIONS* A_options);

SAMP_BOOLEAN ReadResponseMessages(STORAGE_OPTIONS*  A_options, int A_associationID, int A_timeout, InstanceNode** A_list, InstanceNode* A_node);
SAMP_BOOLEAN CheckResponseMessage(int A_responseMsgID, unsigned int* A_status, char* A_statusMeaning, size_t A_statusMeaningLength, InstanceNode* node);