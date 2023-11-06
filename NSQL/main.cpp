#include "server.h"
#include "commands.h"
#include "util.h"
#include "Script.h"
#include <map>
#include <string>

// insert all default commands from commands.h
std::map<std::string, std::function<bool(void*)>> commands;

void default_case(Packet pack)
{
    send(pack.sender, "Invalid command.\n", 18, 0);
}

// returns false if command is "logout"
bool execute(std::string command, Packet pack)
{
    std::string sep = " ";
    std::vector<std::string> parsed = split(command, sep);
    auto it = commands.find(parsed[0].c_str());
    if (it != commands.end())
    {
        printf("Executing: %s\n", command.c_str());
        return it->second((void*)new Command {pack, parsed[1]});
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
    showCursor(false);
    std::cout << "---------------------------------------------------------" << std::endl;
    std::cout << "           NSQL Remote Command Receiver V1.0a            " << std::endl;
    std::cout << "                   Made by YasInvolved                   " << std::endl;
    std::cout << "---------------------------------------------------------" << std::endl;
    newline();
    printf("Initiating...\n");
    printf("Updating commands list...\n"); // TODO: Update commands list from script files
    commands["shutdown"] = tshutdown;
    commands["cancel"] = tshutdowncancell;
    commands["cmd"] = cmd;
    commands["logout"] = logout;
    commands["steam"] = steam;
    commands["discord"] = discord;
    Script* script = new Script("D:\\scripts");
    printf("\nFiles to parse: %d\n", script->getFilesSize());
    script->parse();
    Server* s = new Server(handleConnection);
    s->startListen();
    return 0;
}