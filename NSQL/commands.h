#pragma once
#include <string>
#include <iostream>
#include <shellapi.h>

bool tshutdown(void* data)
{
	Command* cmd = (Command*)data;
	std::string command = "shutdown -s -t ";
	command += std::to_string(std::stoi(cmd->args[0]) * 60);
	std::cout << command << std::endl;
	system(command.c_str());
	return true;
}

bool tshutdowncancell(void* data)
{
	system("shutdown -a");
	return true;
}

bool cmd(void* data) 
{
	Command* cmd = (Command*)data;
	std::string command = "";
	for (std::string arg : cmd->args)
	{
		command += arg + " ";
	}
	system(command.c_str());
	return true;
}

bool steam(void* data)
{
	STARTUPINFO si;
	PROCESS_INFORMATION pi;
	ZeroMemory(&si, sizeof(si));
	ZeroMemory(&pi, sizeof(pi));
	si.cb = sizeof(si);
	CreateProcess("C:\\PROGRA~2\\steam\\steam.exe", NULL, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi);
	CloseHandle(pi.hProcess);
	CloseHandle(pi.hThread);
	return true;
}

bool discord(void* data)
{
	char buff[128];
	ZeroMemory(&buff, sizeof(buff));
	GetEnvironmentVariableA("userprofile", buff, sizeof(buff));
	std::string path = buff;
	path += "\\Desktop\\Discord.lnk";
	ShellExecute(NULL, NULL, path.c_str(), NULL, NULL, SW_SHOWNORMAL);
	return true;
}

bool logout(void* data)
{
	return false; // means logout :)
}