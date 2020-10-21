#define CATCH_CONFIG_MAIN//  This tells Catch to provide a main() - only do this in one cpp file

#include "catch.hpp"
#include "ImageList.h"
#include "general_util.h"
#include "SenderMainFunctions.h"
TEST_CASE("when the character array of -p and port number and its corresponding position and storage option is given then setLocalPortNumber sets"
"the corresponding port number")
{
	char *argv[] = { (char*)"-b",(char*)"676" };
	int pos = 0;
	STORAGE_OPTIONS options;
	setLocalPortNumber(argv, pos, &options);
	REQUIRE(options.ListenPort == 676);
}

TEST_CASE("when the character array of -n and host name and its corresponding position and storage option is given then setRemoteHostName sets"
	"the corresponding host name to storage options")
{
	char *argv[] = { (char*)"-n",(char*)"localhost" };
	int pos = 0;
	STORAGE_OPTIONS options;
	setRemoteHostName(argv, pos, &options);
	REQUIRE(strcmp(options.RemoteHostname ,"localhost")==0);
}

TEST_CASE("when the character array of -p and its port number and its corresponding position and storage option is given then setRemotePortNumber sets"
	"the corresponding port number to storage options")
{
	char *argv[] = { (char*)"-p",(char*)"6767" };
	int pos = 0;
	STORAGE_OPTIONS options;
	setRemotePortNumber(argv, pos, &options);
	REQUIRE(options.RemotePort == 6767);
}

TEST_CASE("when the command line argument and the argument count is given then the TestCmdLine function returns TRUE SAMP_BOOLEAN if all parameters are already set")
{
	char* argv[] = { (char*)"sender.exe",(char*)"MERGE_STORE_SCP",(char*)"0.img",(char*)"1.img",(char*)"-p",(char*)"6767" }; 

	int argc = 6;
	STORAGE_OPTIONS options;
	TestCmdLine(argc, argv, &options);
	REQUIRE(TestCmdLine(argc, argv, &options) == SAMP_TRUE);
}

TEST_CASE("AddFiletoList when called on an empty list then returns true on succesfull allocation of memory")
{
	InstanceNode* node = nullptr;
	char name[10] = {0};
	ImageList image;
	
	REQUIRE(image.AddFileToList(&node,name) == SAMP_TRUE);
}


TEST_CASE("AddFiletoList when called on non-empty list then returns true on succesfull allocation of memory")
{
	auto node1 = static_cast<InstanceNode*>(malloc(sizeof(InstanceNode)));
	auto node2 = static_cast<InstanceNode*>(malloc(sizeof(InstanceNode)));

	node1->Next = node2;
	node2->Next = nullptr;
	char name[10] = { 0 };
	name[0] = '0';
	ImageList image;

	REQUIRE(image.AddFileToList(&node1, name) == SAMP_TRUE);
}


TEST_CASE("Get number of nodes in the linked list when called then returns number of nodes")
{
	auto node1 = static_cast<InstanceNode*>(malloc(sizeof(InstanceNode)));
	auto node2 = static_cast<InstanceNode*>(malloc(sizeof(InstanceNode)));

	node1->Next = node2;
	node2->Next = nullptr;

	ImageList image;
	REQUIRE(image.GetNumNodes(node1) == 2);
}

TEST_CASE("Get number of nodes in the linked list when called with no nodes then returns 0 number of nodes")
{
	auto node1 = static_cast<InstanceNode*>(malloc(sizeof(InstanceNode)));
	auto node2 = static_cast<InstanceNode*>(malloc(sizeof(InstanceNode)));

	node1 = nullptr;

	ImageList image;
	REQUIRE(image.GetNumNodes(node1) == 0);
}

TEST_CASE("Get number of outstanding requests when called then returns number of nodes whose certain parameters are set to some value")
{
	auto node1 = static_cast<InstanceNode*>(malloc(sizeof(InstanceNode)));
	auto node2 = static_cast<InstanceNode*>(malloc(sizeof(InstanceNode)));
	auto node3 = static_cast<InstanceNode*>(malloc(sizeof(InstanceNode)));

	node1->Next = node2;
	node2->Next = node3;
	
	node1->imageSent = SAMP_TRUE;
	node1->responseReceived = SAMP_FALSE;
	
	node2->imageSent = SAMP_TRUE;
	node2->responseReceived = SAMP_FALSE;

	node3->Next = nullptr;
	
	ImageList image;
	REQUIRE(image.GetNumOutstandingRequests(node1) == 2);
}

TEST_CASE("FreeList when called frees the linked list passed")
{
	auto node1 = static_cast<InstanceNode*>(malloc(sizeof(InstanceNode)));
	auto node2 = static_cast<InstanceNode*>(malloc(sizeof(InstanceNode)));
	auto node3 = static_cast<InstanceNode*>(malloc(sizeof(InstanceNode)));

	node1->Next = node2;
	node2->Next = node3;
	node3->Next = nullptr;
	
	ImageList image;
	image.FreeList(&node1);
	REQUIRE(node1 == nullptr);
}

TEST_CASE("When the instanceList pointer address,storage options and imageList if given then the addImageTOList function"
"adds the image to the list")
{
	STORAGE_OPTIONS options;
	InstanceNode *instanceList = NULL, *node = NULL;
	ImageList* imgList = new ImageList;
	options.StartImage =0;
	options.StopImage = 1;
	addImagesToList(&options, imgList, &instanceList);
	int totalImages = 2;

	REQUIRE(imgList->GetNumNodes(instanceList)==totalImages);

}
//TEST_CASE("when the address of  applicationId ,AssociationId and storage Options is given then the"
//	"openAssociation Function open the association sets the applicationID and AssociationId")
//{
//	int  applicationID = -1, associationID = -1;
//	STORAGE_OPTIONS options;
//	MC_STATUS mcStatus;
//	char* argv[] = { (char*)"sender.exe",(char*)"MERGE_STORE_SCP",(char*)"0.img",(char*)"1.img",(char*)"-p",(char*)"6767" };
//
//	int argc = 6;
//	InstanceNode *instanceList = NULL, *node = NULL;
//	ImageList* imgList = new ImageList;
//	TestCmdLine(argc, argv, &options);
//	mcStatus = MC_Library_Initialization(NULL, NULL, NULL);
//	mcStatus = MC_Register_Application(&applicationID, options.LocalAE);
//	addImagesToList(&options, imgList, &instanceList);
//	openAssociation(&applicationID, &associationID, &options);
//	mcStatus = MC_Get_Association_Info(associationID, &options.asscInfo);
//	REQUIRE(mcStatus == MC_NORMAL_COMPLETION);
//
//}
TEST_CASE("when the options,applicationID and instanceList with no library initialization is given then read function return SAMP_FALSE")
{
	SAMP_BOOLEAN            sampBool;
	ReadImage* reader = new ReadImage;
	STORAGE_OPTIONS options;
	int applicationID = -1;
	InstanceNode   *node = NULL;
	MC_STATUS mcStatus;
	
	char* argv[] = { (char*)"sender.exe",(char*)"MERGE_STORE_SCP",(char*)"0.img",(char*)"1.img",(char*)"-p",(char*)"6767" };

	int argc = 6;
	InstanceNode *instanceList = NULL;
	ImageList* imgList = new ImageList;
	mcStatus = MC_Library_Initialization(NULL, NULL, NULL);
	REQUIRE(mcStatus != MC_NORMAL_COMPLETION);
	TestCmdLine(argc, argv, &options);
	addImagesToList(&options, imgList, &instanceList);
	node = instanceList;
	sampBool = reader->read(&options, applicationID, node);
	REQUIRE(sampBool == SAMP_FALSE);
}
TEST_CASE("when the valid storage options,associationID,and node then send fucntion return SAMP_TRUE")
{
	STORAGE_OPTIONS options;
	int associationID = -1;
	InstanceNode   *node = NULL;
	MC_STATUS mcStatus;

	char* argv[] = { (char*)"sender.exe",(char*)"MERGE_STORE_SCP",(char*)"0.img",(char*)"1.img",(char*)"-p",(char*)"6767" };

	int argc = 6;
	InstanceNode *instanceList = NULL;
	ImageList* imgList = new ImageList;
	mcStatus = MC_Library_Initialization(NULL, NULL, NULL);
	REQUIRE(mcStatus != MC_NORMAL_COMPLETION);
	TestCmdLine(argc, argv, &options);
	addImagesToList(&options, imgList, &instanceList);
	node = instanceList;

	SAMP_BOOLEAN sampBool;
	SendImage *sender = new SendImage;
	
	sampBool = sender->send(&options, associationID, node);
	REQUIRE(sampBool == SAMP_TRUE);
}

TEST_CASE("When the imageSent as SAMP_TRUE are given then setnodeResponseParamsOnSucces set the imagesSent count to 1")
{
	STORAGE_OPTIONS options;
	int associationID = -1;
	InstanceNode   *node = NULL;
	MC_STATUS mcStatus;

	char* argv[] = { (char*)"sender.exe",(char*)"MERGE_STORE_SCP",(char*)"0.img",(char*)"1.img",(char*)"-p",(char*)"6767" };

	int argc = 6;
	InstanceNode *instanceList = NULL;
	ImageList* imgList = new ImageList;
	mcStatus = MC_Library_Initialization(NULL, NULL, NULL);
	REQUIRE(mcStatus != MC_NORMAL_COMPLETION);
	TestCmdLine(argc, argv, &options);
	addImagesToList(&options, imgList, &instanceList);
	node = instanceList;
	node->imageSent = SAMP_TRUE;
	int imagesSent = 0;
	setNodeResponseParamsOnSucces(node, &imagesSent);
	REQUIRE(imagesSent == 1);
}

TEST_CASE("When the imageSent as SAMP_FALSE is  given then setnodeResponseParamsOnSucces sets the ResponseReceived and FailedResponse as SAMP_TRUE")
{
	STORAGE_OPTIONS options;
	int associationID = -1;
	InstanceNode   *node = NULL;
	MC_STATUS mcStatus;

	char* argv[] = { (char*)"sender.exe",(char*)"MERGE_STORE_SCP",(char*)"0.img",(char*)"1.img",(char*)"-p",(char*)"6767" };

	int argc = 6;
	InstanceNode *instanceList = NULL;
	ImageList* imgList = new ImageList;
	mcStatus = MC_Library_Initialization(NULL, NULL, NULL);
	REQUIRE(mcStatus != MC_NORMAL_COMPLETION);
	TestCmdLine(argc, argv, &options);
	addImagesToList(&options, imgList, &instanceList);
	node = instanceList;
	node->imageSent = SAMP_FALSE;
	int imagesSent = 0;
	setNodeResponseParamsOnSucces(node, &imagesSent);
	REQUIRE(node->failedResponse==SAMP_TRUE);
	REQUIRE(node->responseReceived == SAMP_TRUE);
}
