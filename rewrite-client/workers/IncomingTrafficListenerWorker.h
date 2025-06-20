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
        void SetSockFD(const SOCKET SockFD);
        SOCKET GetSocketFD();
    public:
        IncomingTrafficListenerWorker(const SOCKET SockFD);
        ~IncomingTrafficListenerWorker();
        void Run();
        void Stop();
};

#endif