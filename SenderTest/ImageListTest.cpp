#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file

#include "catch.hpp"
#include "../Sender/ImageList.cpp"


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