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

void ConnectionHandler::ListenForInput()
{
    InputListenerWorker worker(this->getSocketFD());
    worker.Run();
}

void ConnectionHandler::ListenForMessages()
{
    IncomingTrafficListenerWorker worker(this->getSocketFD());
    worker.Run();
}