#pragma comment (lib, "ws2_32.lib")
#include "WSAInitializer.h"
#include "Communicator.h"

void main()
{
	WSAInitializer wsaInit;
	Communicator comm;
	comm.startHandleRequests();
}