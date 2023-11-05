#pragma once

#include <string>
#include "Property.hpp"

class JsonSerde;

namespace JsonSerdeStatics
{
    std::string ToJson(JsonSerde &obj);

    std::string ToJson(std::string &obj);

    std::string ToJson(bool &obj);

    std::string ToJson(int &obj);

    std::string ToJson(property &property);
};