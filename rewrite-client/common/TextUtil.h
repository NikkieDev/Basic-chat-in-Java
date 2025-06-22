#ifndef TEXT_UTIL_H_
#define TEXT_UTIL_H_

#include <string>

class TextUtil
{
    public:
        static void removeCarriage(std::string* string);
        static void removeNewlines(std::string* string);
        static bool isSystemMessage(const std::string& message);
};

#endif