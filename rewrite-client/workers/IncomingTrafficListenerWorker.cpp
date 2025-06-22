#include "IncomingTrafficListenerWorker.h"
#include "../src/ConnectionHandler.h"
#include "../src/Terminal.h"
#include "../common/TextUtil.h"
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
    ConnectionHandler* handler = ConnectionHandler::GetInstance();
    if ("KEEPALIVE" == command) {
        handler->SendMessageToServer("HEARTBEAT\n");
        return true;
    }

    return false;
}

void IncomingTrafficListenerWorker::Run()
{
    while (this->listening) {
        char memoryBuffer[512];
        int received = recv(this->GetSocketFD(), memoryBuffer, sizeof(memoryBuffer)-1, 0);
        
        if (0 >= received) continue;

        memoryBuffer[received] = '\0';
        std::string message(memoryBuffer, received);

        std::string rawMessage = message;
        TextUtil::removeCarriage(&rawMessage);
        TextUtil::removeNewlines(&rawMessage);

        if (!TextUtil::isSystemMessage(rawMessage) && this->MatchesCommand(rawMessage)) {
            continue;
        }

        this->terminal.PrintChatMessage(message);
    }
}