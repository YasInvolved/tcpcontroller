#include "server.h"
#include "util.h"
#include "commands.h"
#include <map>

std::map<std::string, std::function<bool(void*)>> commands;

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
        return it->second((void*)new Command {pack, split(command, it->first.c_str())[1]});
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

int main(void)
{
    std::cout << "---------------------------------------------------------" << std::endl;
    std::cout << "           NSQL Remote Command Receiver V1.0a            " << std::endl;
    std::cout << "                   Made by YasInvolved                   " << std::endl;
    std::cout << "---------------------------------------------------------" << std::endl;
    newline();
    std::cout << "Updating commands list..." << std::endl;
    // all content is from "commands.h"
    commands["shutdown"] = tshutdown;
    commands["cmd"] = cmd; 
    commands["logout"] = logout;
    commands["steam"] = steam;
    Server* s = new Server(handleConnection);
    s->startListen();
    return 0;
}