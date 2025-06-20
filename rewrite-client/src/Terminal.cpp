#include "Terminal.h"
#include <iostream>
#include <string>

void Terminal::PrintSystemMessage(
    std::string message
)
{
    std::cout << "[System] " << message << std::endl;
}

void Terminal::PrintDebugMessage(
    std::string message
)
{
    std::cout << "[Debug] " << message << std::endl;
}

void Terminal::Chatting()
{
    std::cout << "You: ";
}

void Terminal::NewLine()
{
    std::cout << std::endl;
}