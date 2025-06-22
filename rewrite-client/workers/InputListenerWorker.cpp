#include "InputListenerWorker.h"
#include "../src/ConnectionHandler.h"
#include "../src/Terminal.h"
#include <string>
#include <iostream>
#include <stdexcept>

InputListenerWorker::InputListenerWorker(const SOCKET SockFD)
{
    this->listening = true;
    this->Connection = ConnectionHandler::GetInstance();
    this->SetSocketFD(SockFD);
}

InputListenerWorker::~InputListenerWorker()
{
}

void InputListenerWorker::SetSocketFD(const SOCKET SockFD)
{
    this->SockFD = SockFD;
}

SOCKET InputListenerWorker::GetSocketFD()
{
    return this->SockFD;
}

void InputListenerWorker::Stop()
{
    this->listening = false;
}

bool InputListenerWorker::MatchesCommand(const std::string& command)
{
    if ("DISCONNECT" == command) {
        this->Connection->Disconnect();
        return true;
    }

    return false;
}

void InputListenerWorker::Run()
{
    ConnectionHandler* handler = ConnectionHandler::GetInstance();
    Terminal* terminal;

    while (this->listening) {
        std::string input;
        std::getline(std::cin, input);
        
        if (input.empty()) {
            continue;
        }

        if (this->MatchesCommand(input)) {
            continue;
        }

        input = input + "\n";
        handler->SendMessageToServer(input);
    }
}

// -> beautify the reading process and writing process
// -> fix disconnect not stopping the listeners