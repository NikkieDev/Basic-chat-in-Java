#ifndef LISTENER_H_
#define LISTENER_H_

#include <WinSock2.h>
#include <string>

class AbstractListener
{

    public:
        virtual void Stop() = 0;
        virtual SOCKET GetSocketFD() = 0;
        virtual void SetSocketFD(const SOCKET SockFD) = 0;
        virtual bool MatchesCommand(const std::string& input) = 0;
        virtual ~AbstractListener() = default;
};


#endif