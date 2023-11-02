#pragma once
#pragma comment (lib, "Ws2_32.lib")
#undef UNICODE

#define WIN32_LEAN_AND_MEAN
#define newline() std::cout << std::endl;

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>
#include <thread>
#include <string>
#include <sstream>
#include <iostream>
#include <future>

#define DEFAULT_BUFLEN 512
#define DEFAULT_PORT "21037"

class Socket {
public:
	Socket();
	~Socket();
	void startListen();
private:
	void handleClient(void* data);
	struct addrinfo* result = NULL;
	struct addrinfo hints;
	WSADATA wsadata;
	int iresult;
	SOCKET ListenSocket = INVALID_SOCKET;
};

