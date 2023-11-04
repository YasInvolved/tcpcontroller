#include "server.h"
#include <shellapi.h>
#pragma once

bool tshutdown(void* data)
{
	Command* cmd = (Command*)data;
	std::string command = "shutdown -s -t ";
	command += cmd->args;
	system("command");
	return true;
}

bool cmd(void* data) 
{
	Command* cmd = (Command*)data;
	system(cmd->args.c_str());
	return true;
}

bool steam(void* data)
{
	system("start C:\\PROGRA~2\\steam\\steam.exe");
	return true;
}

bool logout(void* data)
{
	return false; // means logout :)
}