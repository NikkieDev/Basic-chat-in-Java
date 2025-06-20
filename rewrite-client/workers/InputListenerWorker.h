#ifndef INPUT_LISTENER_WORKER_H_
#define INPUT_LISTENER_WORKER_H_

#include "../common/Runnable.h"
#include "../common/Listener.h"

#include <string>
#include <WinSock2.h>

class InputListenerWorker : public AbstractRunnable, public AbstractListener
{
    private:
        SOCKET SockFD;
        bool listening;
        SOCKET GetSocketFD();
        void SetSockFD(const SOCKET SockFD);
        void SendMessageToServer(const std::string& message);
        bool MatchesCommand(const std::string& input);

    public:
        InputListenerWorker(const SOCKET SockFD);
        ~InputListenerWorker();
        void Run();
        void Stop();
};

#endif