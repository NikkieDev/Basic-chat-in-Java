#include "ConnectionHandler.h"
#include "../workers/InputListenerWorker.h"
#include "../workers/IncomingTrafficListenerWorker.h"

#include <string>
#include <stdexcept>
#include <WinSock2.h>
#include <WS2tcpip.h>

ConnectionHandler* ConnectionHandler::instance = nullptr;

ConnectionHandler::ConnectionHandler()
{
    SOCKET sockFD = socket(AF_INET, SOCK_STREAM, 0);

    if (0 >= sockFD) {
        throw std::runtime_error("Unable to connect to server");
    }

    this->setSocketFD(sockFD);
}

ConnectionHandler::~ConnectionHandler()
{
    closesocket(this->getSocketFD());
}

void ConnectionHandler::setSocketFD(SOCKET sockFD)
{
    this->SockFd = sockFD;
}

SOCKET ConnectionHandler::getSocketFD()
{
    return this->SockFd;
}

void ConnectionHandler::setConnection(int connection)
{
    this->Connection = connection;
}

ConnectionHandler* ConnectionHandler::GetInstance() {
    if (nullptr == instance) {
        instance = new ConnectionHandler();
    }

    return instance;
}

void ConnectionHandler::ConnectTo(const std::string& ip, int port)
{
    struct sockaddr_in address;

    address.sin_family = AF_INET;
    address.sin_port = htons(port);
    inet_pton(AF_INET, ip.c_str(), &address.sin_addr);

    int connection = connect(this->getSocketFD(), (sockaddr*)&address, sizeof(address));

    if (SOCKET_ERROR == connection) {
        throw std::runtime_error("Unable to connect to server, " + WSAGetLastError());
    }

    this->setConnection(connection);
}

void ConnectionHandler::Disconnect()
{
    this->StopListeningForInput();
    this->StopListeningForMessages();
    this->setConnection(-1);
    this->setSocketFD(-1);
}

void ConnectionHandler::ListenForInput()
{
    InputListenerWorker worker(this->getSocketFD());
    worker.Run();
    this->setInputWorker(&worker);
}

void ConnectionHandler::StopListeningForInput()
{
    this->GetInputWorker()->Stop();
}

void ConnectionHandler::ListenForMessages()
{
    IncomingTrafficListenerWorker worker(this->getSocketFD());
    worker.Run();
    this->setTrafficListenerWorker(&worker);
}

void ConnectionHandler::StopListeningForMessages()
{
    this->GetTrafficListenerWorker()->Stop();
}

void ConnectionHandler::setInputWorker(InputListenerWorker* worker)
{
    this->InputWorker = worker;
}

void ConnectionHandler::setTrafficListenerWorker(IncomingTrafficListenerWorker* worker)
{
    this->TrafficListenerWorker = worker;
}

InputListenerWorker* ConnectionHandler::GetInputWorker()
{
    return this->InputWorker;
}

IncomingTrafficListenerWorker* ConnectionHandler::GetTrafficListenerWorker()
{
    return this->TrafficListenerWorker;
}

void ConnectionHandler::SendMessageToServer(const std::string& message)
{
    int sendResult = send(this->getSocketFD(), message.c_str(), static_cast<int>(message.length()), 0);

    if (SOCKET_ERROR == sendResult) {
        throw std::runtime_error("Unable to send message, " + WSAGetLastError());
    }
}