#include <iostream>
#include <thread>
#include <string>
#include <WinSock2.h>

#include "src/Terminal.h"
#include "src/ConnectionHandler.h"

#pragma comment(lib, "ws2_32.lib")

#define UNABLE_TO_CONNECT 1

void InitWindows();
void CleanupWindows();

int main()
{
    InitWindows();

    Terminal* terminal = new Terminal();
    ConnectionHandler* connectionHandler = nullptr;

    std::string message = "Welcome, we are connecting you.";

    terminal->PrintSystemMessage(message);

    try {
        connectionHandler = ConnectionHandler::GetInstance();
        connectionHandler->ConnectTo("127.0.0.1", 8080);

        terminal->PrintSystemMessage("You've been connected you can start typing now!");
        terminal->Chatting();
    } catch (const std::runtime_error& e) {
        terminal->PrintSystemMessage(e.what());
        exit(UNABLE_TO_CONNECT);
    }

    std::thread InputThread(&ConnectionHandler::ListenForInput, connectionHandler);
    InputThread.join();

    terminal->PrintSystemMessage("You've left the chatroom.");

    delete terminal;

    CleanupWindows();
    return 0;
}

void InitWindows()
{
    WSAData wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);
}

void CleanupWindows()
{
    WSACleanup();
}