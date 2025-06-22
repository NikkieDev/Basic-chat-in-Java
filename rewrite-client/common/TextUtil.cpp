#include "TextUtil.h"
#include <algorithm>

void TextUtil::removeCarriage(std::string* string)
{
    string->erase(std::remove(string->begin(), string->end(), '\r'), string->end());
}

void TextUtil::removeNewlines(std::string* string)
{
    string->erase(std::remove(string->begin(), string->end(), '\n'), string->end());
}

bool TextUtil::isSystemMessage(const std::string& message)
{
    if (0 <= message.rfind("[System]", 9)) {
        return false;
    }

    return true;
}