#include "sock.h"

/*
Constructor:
*   initializes the socket structure
*/
Socket::Socket() {
	Socket::iresult = WSAStartup(MAKEWORD(2, 2), &wsadata);
	if (Socket::iresult != 0)
	{
		printf("Startup failed with code: %d\n", iresult);
		return;
	}

	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	hints.ai_flags = AI_PASSIVE;
 

    // Resolve the server address and port
    iresult = getaddrinfo("0.0.0.0", DEFAULT_PORT, &hints, &result);
    if (iresult != 0) {
        printf("getaddrinfo failed with error: %d\n", iresult);
        WSACleanup();
        return;
    }

    // Create a SOCKET for the server to listen for client connections.
    ListenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
    if (ListenSocket == INVALID_SOCKET) {
        printf("socket failed with error: %ld\n", WSAGetLastError());
        freeaddrinfo(result);
        WSACleanup();
        return;
    }

    // Setup the TCP listening socket
    iresult = bind(ListenSocket, result->ai_addr, (int)result->ai_addrlen);
    if (iresult == SOCKET_ERROR) {
        printf("bind failed with error: %d\n", WSAGetLastError());
        freeaddrinfo(result);
        closesocket(ListenSocket);
        WSACleanup();
        return;
    }

    freeaddrinfo(result);
};

Socket::~Socket()
{
    WSACleanup();
}

void Socket::startListen() 
{
    listen(ListenSocket, SOMAXCONN);
    SOCKET ClientSocket = accept(ListenSocket, NULL, NULL);
    std::async(std::launch::async, &Socket::handleClient, this, (void*)ClientSocket);
}

void Socket::handleClient(void* data)
{
    char recvbuf[DEFAULT_BUFLEN];
    SOCKET Client = (SOCKET)data;
    int bytes;
    bool exit = false;
    std::ostringstream command, message;
    do {
        recv(Client, recvbuf, sizeof(recvbuf), 0);
        for (int i = 0; i < sizeof(recvbuf); i++)
        {
            if (recvbuf[i] == -52) break;
            command << recvbuf[i];
        }
        if (command.str().compare("exit") == 1) exit = true;
        message << "Processing command: " << command.str() << std::endl;
        std::cout << message.str();
        send(Client, message.str().c_str(), sizeof(message.str().c_str()), 0);
    } while (exit == false);
    send(Client, "closing connection", 19, 0);
    shutdown(Client, SD_SEND);
    return;
}