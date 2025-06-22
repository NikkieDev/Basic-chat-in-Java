#ifndef MESSAGE_HANDLER_H_
#define MESSAGE_HANDLER_H_

#include <WinSock2.h>
#include <string>

class MessageHandler
{
    private:
        SOCKET SocketFD;

    public:
        MessageHandler(SOCKET SocketFD);
        ~MessageHandler();
        void SendMessageToServer(const std::string& message);
        std::string ReceiveMessage();
};

#endif