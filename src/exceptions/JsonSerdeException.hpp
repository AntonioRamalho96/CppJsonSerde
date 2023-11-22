#pragma once

#include <iostream>
#include <stdexcept>

class JsonSerdeExcption : public std::exception
{
public:
    JsonSerdeExcption() = default;
    virtual const char *what() const noexcept = 0;
};