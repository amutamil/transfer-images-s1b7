#pragma once
#include "util_structures.h"
#include "general_util.h"
#include "ReadMessageFromFile.h"
#include "SendImage.h"
#include "ImageList.h"
#include "ReadImage.h"
void openAssociation(int* A_applicationID, int* A_associationID, STORAGE_OPTIONS* A_options);
void exitApplicationIfNotNormalCompletion(bool statusBool);
void addImagesToList(STORAGE_OPTIONS* options, ImageList* imgList, InstanceNode** node);
void setNodeResponseParamsOnSucces(InstanceNode* node, int* imgSent);
void abortAndReleaseApplicationOnFailure(char* statusMsg, bool statusBool, int* associationID, int* applicationID);
void closeAssociation(int* A_pplication, int* A_associationID);

