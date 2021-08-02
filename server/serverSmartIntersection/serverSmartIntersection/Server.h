#pragma once
#include "Communicator.h"

class Server
{
public:
	Server();
	~Server();
	void run();
private:
	Communicator* com;
	//smart interstion

};