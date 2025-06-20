#ifndef INCOMING_TRAFFIC_LISTENER_WORKER_H
#define INCOMING_TRAFFIC_LISTENER_WORKER_H

#include "../common/Runnable.h"

class IncomingTrafficListenerWorker : public AbstractRunnable
{
    public:
        IncomingTrafficListenerWorker();
        ~IncomingTrafficListenerWorker();
        void Run();
};

#endif