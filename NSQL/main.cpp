#pragma comment(lib, "rpcrt4.lib")
#include "server.h"
#include "commands.h"
#include "util.h"
#include "Script.h"
#include "resource.h"
#include <map>
#include <string>
#include <rpc.h>1

// insert all default commands from commands.h
std::map<std::string, std::function<bool(void*)>> commands;
std::map<std::string, std::string> config;

void default_case(Packet pack)
{
    send(pack.sender, "Invalid command.\n", 18, 0);
}

// returns false if command is "logout"
bool execute(std::string command, Packet pack)
{
    std::vector<std::string> parsed = split(command, " ");
    auto it = commands.find(parsed[0].c_str());
    if (it != commands.end())
    {
        printf("Executing: %s\n", command.c_str());
        std::vector <std::string> args(parsed.begin() + 1, parsed.end());
        return it->second((void*)new Command{ pack, args});
    }
    else {
        default_case(pack);
        return true;
    }
    return false;
}

bool handleConnection(void* data)
{
    Packet* pack = (Packet*)data;
    char buf[512] = {'\0'};
    int bytes = 0;
    if (config.find("password") != config.end())
    {
        printf("%s: Authorization required\n", pack->senderaddress);
        send(pack->sender, "Podaj haslo: \n", 15, 0);
        bytes = recv(pack->sender, buf, sizeof(buf), 0);
        std::string password = buf;
        if (password != config["password"]) {
            send(pack->sender, "zle haslo.\n", 12, 0);
            Sleep(100);
            shutdown(pack->sender, SD_SEND);
            printf("%s: connection closed due to bad password.\n", pack->senderaddress);
            return true;
        }
        printf("%s: Authorization complete\n", pack->senderaddress);
        send(pack->sender, "Haslo poprawne.\n", 17, 0);
    }
    ZeroMemory(&buf, sizeof(buf));

    while (1)
    {
        bytes = recv(pack->sender, buf, sizeof(buf), 0);
        if (bytes <= 0) break;
        printf("Received %d bytes from %s\n", bytes, pack->senderaddress);
        std::string command = buf;
        command.erase(std::remove(command.begin(), command.end(), (char)-52));
        printf("%s: %s\n", pack->senderaddress, command.c_str());
        if (execute(command, *pack) == false)
        {
            send(pack->sender, "closing connection", 19, 0);
            Sleep(100);
            break;
        }
        else {
        }
        ZeroMemory(buf, sizeof(buf));
    }
    shutdown(pack->sender, SD_SEND);
    printf("Connection closed: %s\n", pack->senderaddress);
    return true;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pCmdLine, int nCmdShow)
{
    showCursor(false);
    std::cout << "---------------------------------------------------------" << std::endl;
    std::cout << "           NSQL Remote Command Receiver V1.0b            " << std::endl;
    std::cout << "                   Made by YasInvolved                   " << std::endl;
    std::cout << "---------------------------------------------------------" << std::endl;
    newline();
    printf("Initiating...\n");
    printf("Updating config...\n"); // TODO: Update config from script files
    commands["shutdown"] = tshutdown;
    commands["cancel"] = tshutdowncancell;
    commands["cmd"] = cmd;
    commands["logout"] = logout;
    commands["steam"] = steam;
    commands["discord"] = discord;
    Script* script = new Script(__argv[1]);
    printf("\nFiles to parse: %d\n", script->getFilesArrSize());
    script->parse(&commands, &config);
    newline();
    Server s(handleConnection);
    s.startListen();
    return 0;
}