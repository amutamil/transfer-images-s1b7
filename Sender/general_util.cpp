

#include "general_util.h"
#include <iostream>

void PrintError(std::string A_string, typename MC_STATUS A_status)
{
	std::string prefix = "                              ";
	/*
	 *  Need process ID number for messages
	 */
#ifdef UNIX
	sprintf(prefix, "PID %d", getpid());
#endif
	if (A_status == -1)
	{
		std::cout << prefix << "\t" << A_string << std::endl;
	}
	else
	{
		std::cout << prefix << "\t" << A_string << ":\n";
		std::cout << prefix << "\t" << MC_Error_Message(A_status) << std::endl;
	}
}

void PrintStatus(std::string A_string, MC_STATUS A_status)
{
	if (A_status != MC_NORMAL_COMPLETION)
	{
		std::cout << A_string << std::endl;
	}
}
void PrintStatus(std::string A_string, bool statusFlag)
{
	if (statusFlag)
	{
		std::cout << A_string << std::endl;
	}
}