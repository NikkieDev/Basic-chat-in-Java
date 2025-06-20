#ifndef TERMINAL_H
#define TERMINAL_H

#include <string>

class Terminal {
    public:
        void PrintSystemMessage(std::string message);
        void PrintDebugMessage(std::string message);
        void Chatting();
        void NewLine();
};

#endif