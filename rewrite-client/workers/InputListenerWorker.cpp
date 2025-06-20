#include "InputListenerWorker.h"
#include "../src/Terminal.h"
#include <string>
#include <iostream>
#include <stdexcept>

InputListenerWorker::InputListenerWorker(const SOCKET SockFD)
{
    this->listening = true;
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

void InputListenerWorker::SendMessageToServer(const std::string& message)
{
    int sendResult = send(this->GetSocketFD(), message.c_str(), static_cast<int>(message.length()), 0);

    if (SOCKET_ERROR == sendResult) {
        throw std::runtime_error("Unable to send message, " + WSAGetLastError());
    }
}

void InputListenerWorker::Stop()
{
    this->listening = false;
}

bool InputListenerWorker::MatchesCommand(const std::string& command)
{
    if ("DISCONNECT" == command) {
        this->Stop();
        return true;
    }

    return false;
}

void InputListenerWorker::Run()
{
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
        this->SendMessageToServer(input);

        terminal->Chatting();
    }
}

// Current problem -> debug the "keepalive"
// -> beautify the reading process and writing process
// -> fix disconnect not stopping the listeners