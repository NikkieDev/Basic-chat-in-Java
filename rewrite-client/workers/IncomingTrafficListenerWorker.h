#ifndef INCOMING_TRAFFIC_LISTENER_WORKER_H
#define INCOMING_TRAFFIC_LISTENER_WORKER_H

#include "../common/Runnable.h"
#include "../common/Listener.h"
#include <WinSock2.h>

class IncomingTrafficListenerWorker : public AbstractRunnable, public AbstractListener
{
    private:
        bool listening;
        SOCKET SockFD;
        void SetSocketFD(const SOCKET SockFD);
        SOCKET GetSocketFD();
        bool MatchesCommand(const std::string& input);

    public:
        IncomingTrafficListenerWorker(const SOCKET SockFD);
        ~IncomingTrafficListenerWorker();
        void Run();
        void Stop();
};

#endif