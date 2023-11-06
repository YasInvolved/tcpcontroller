#pragma once
#pragma comment (lib, "Ws2_32.lib")
#undef UNICODE

#define WIN32_LEAN_AND_MEAN
#define _WINSOCK_DEPRECATED_NO_WARNINGS



#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>
#include <future>

constexpr int DEFAULT_BUFLEN = 512;
constexpr char DEFAULT_PORT[] = "21037";

struct Packet {
	SOCKET sender;
	char* senderaddress;
};

struct Command {
	Packet pack;
	std::string args;
};

class Server {
public:
	Server(std::function<bool(void*)>);
	~Server();
	void startListen();
private:
	std::function<bool(void*)> handle;
	std::vector<std::thread> clients;
	void serve();
	struct addrinfo* result = NULL;
	struct addrinfo hints;
	WSADATA wsadata;
	int iresult;
	SOCKET listenSocket;
};

