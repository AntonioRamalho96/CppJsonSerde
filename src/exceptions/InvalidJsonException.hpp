#pragma once

#include <string>

#include "JsonSerdeException.hpp"

class InvalidJsonException : public JsonSerdeExcption
{
public:
    InvalidJsonException(const std::string &msg) : msg{msg} {}

    const char *what() const noexcept override
    {
        return msg.c_str();
    }

private:
    std::string msg{};
};