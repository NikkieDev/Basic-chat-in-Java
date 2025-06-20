#ifndef LISTENER_H_
#define LISTENER_H_

#include <WinSock2.h>
#include <string>

class AbstractListener
{
    private:
        virtual void Run();
        virtual SOCKET GetSocketFD();
        void SetSocketFD(const SOCKET SockFD);
        bool MatchesCommand(const std::string& input);

    public:
        void Run();
        void Stop();
};


#endif