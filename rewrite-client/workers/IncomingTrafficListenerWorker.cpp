#include "IncomingTrafficListenerWorker.h"
#include "InputListenerWorker.h"
#include "../src/Terminal.h"
#include <string>

IncomingTrafficListenerWorker::IncomingTrafficListenerWorker(const SOCKET SockFD)
{
    this->listening = true;
    this->SetSocketFD(SockFD);
}

IncomingTrafficListenerWorker::~IncomingTrafficListenerWorker()
{
}

void IncomingTrafficListenerWorker::Stop()
{
    this->listening = false;
}

void IncomingTrafficListenerWorker::SetSocketFD(const SOCKET SockFD)
{
    this->SockFD = SockFD;
}

SOCKET IncomingTrafficListenerWorker::GetSocketFD()
{
    return this->SockFD;
}

bool IncomingTrafficListenerWorker::MatchesCommand(const std::string& command)
{
    InputListenerWorker* inputWorker;

    if ("KEEPALIVE" == command) {
        inputWorker->SendMessageToServer("HEARTBEAT");
        return true;
    }

    return false;
}

void IncomingTrafficListenerWorker::Run()
{
    Terminal* terminal;
    char memoryBuffer[512];

    while (this->listening) {
        int received = recv(this->GetSocketFD(), memoryBuffer, sizeof(memoryBuffer)-1, 0);
        
        if (0 >= received) continue;

        memoryBuffer[received] = '\0';
        std::string message(memoryBuffer);

        if (this->MatchesCommand(message)) {
            continue;
        }

        terminal->PrintChatMessage(message);
    }
}