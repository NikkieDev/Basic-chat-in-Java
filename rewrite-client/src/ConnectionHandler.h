#ifndef CONNECTION_HANDLER_H
#define CONNECTION_HANDLER_H

#include "../workers/IncomingTrafficListenerWorker.h"
#include "../workers/InputListenerWorker.h"

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
        void setInputWorker(InputListenerWorker* worker);
        void setTrafficListenerWorker(IncomingTrafficListenerWorker* worker);
        InputListenerWorker* GetInputWorker();
        IncomingTrafficListenerWorker* GetTrafficListenerWorker();
        static ConnectionHandler* instance;
        SOCKET SockFd;
        int Connection;
        IncomingTrafficListenerWorker* TrafficListenerWorker = nullptr;
        InputListenerWorker* InputWorker = nullptr;
    
    public:
       static ConnectionHandler* GetInstance();
       void ConnectTo(const std::string& ip, int port);
       void Disconnect();
       void ListenForInput();
       void StopListeningForInput();
       void ListenForMessages();
       void StopListeningForMessages();
       void SendMessageToServer(const std::string& message);

};


#endif