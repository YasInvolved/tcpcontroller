#include "server.h"

/*
Constructor:
*   initializes the Server structure
*/

Server::Server(std::function<bool(void*)> handleClient)
{
    Server::handle = handleClient;
    Server::iresult = WSAStartup(MAKEWORD(2, 2), &wsadata);
    if (Server::iresult != 0)
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

    // Create a Server for the server to listen for client connections.
    listenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
    if (listenSocket == INVALID_SOCKET) {
        printf("Server failed with error: %ld\n", WSAGetLastError());
        freeaddrinfo(result);
        WSACleanup();
        return;
    }

    // Setup the TCP listening Server
    iresult = bind(listenSocket, result->ai_addr, (int)result->ai_addrlen);
    if (iresult == SOCKET_ERROR) {
        printf("bind failed with error: %d\n", WSAGetLastError());
        freeaddrinfo(result);
        closesocket(listenSocket);
        WSACleanup();
        return;
    }

    freeaddrinfo(result);
};

Server::~Server()
{
    WSACleanup();
}

void Server::startListen() 
{
    printf("Start listening on port %s\n", DEFAULT_PORT);
    std::future<void> listener = std::async(std::launch::async, &Server::serve, this);
    hWnd = createDummyWindow(GetModuleHandle(NULL));
    MSG msg;
    while (GetMessage(&msg, hWnd, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
        if (clients.size() > 0)
        for (auto& p : clients)
        {
            p.detach();
        }
    }
}

void Server::serve()
{
    listen(listenSocket, SOMAXCONN);
    SOCKET ClientSocket;
    SOCKADDR_IN sockaddr;
    int socklen = sizeof(sockaddr);
    while (1)
    {
        ClientSocket = accept(listenSocket, (SOCKADDR*)&sockaddr, &socklen);
        char* addr = inet_ntoa(sockaddr.sin_addr);
        printf("Got connection from %s\n", addr);
        PostMessage(hWnd, CM_JOINED, (WPARAM)addr, 0);
        clients.emplace_back(std::thread(handle, (void*)new Packet{ ClientSocket, addr }));
    }
}