#include "IncomingTrafficListenerWorker.h"
#include "../src/Terminal.h"
#include <string>

IncomingTrafficListenerWorker::IncomingTrafficListenerWorker(const SOCKET SockFD)
{
    this->listening = true;
    this->SetSockFD(SockFD);
}

IncomingTrafficListenerWorker::~IncomingTrafficListenerWorker()
{
}

void IncomingTrafficListenerWorker::Stop()
{
    this->listening = false;
}

void IncomingTrafficListenerWorker::SetSockFD(const SOCKET SockFD)
{
    this->SockFD = SockFD;
}

SOCKET IncomingTrafficListenerWorker::GetSocketFD()
{
    return this->SockFD;
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


    }
}