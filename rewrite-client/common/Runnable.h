#ifndef RUNNABLE_H_
#define RUNNABLE_H_

class AbstractRunnable
{
    public:
        virtual void Run() = 0;
        virtual ~AbstractRunnable() = default;
};

#endif