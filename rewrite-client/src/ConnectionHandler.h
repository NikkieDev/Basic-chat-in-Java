#ifndef CONNECTION_HANDLER_H
#define CONNECTION_HANDLER_H

#include <WinSock2.h>
#include <string>

class ConnectionHandler
{
    private:
        ConnectionHandler();
        ~ConnectionHandler();
        void setSocketFD(SOCKET sockFD);
        SOCKET getSocketFD();
        void setConnection(int connection);
        static ConnectionHandler* instance;
        SOCKET SockFd;
        int Connection;
        
    
    public:
       static ConnectionHandler* GetInstance();
       void ConnectTo(const std::string& ip, int port);
       void ListenForInput();
};


#endif