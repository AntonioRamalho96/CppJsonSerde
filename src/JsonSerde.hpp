#pragma once
#include <string>
#include <vector>

#include "JsonSerdeStatics.hpp"

class JsonSerde
{
public:
    virtual std::string Serialize()
    {
        std::string result{"{"};
        for (auto &prop : GetProperties())
        {
            result.append(" \"" + prop.name + "\" : " + JsonSerdeStatics::ToJson(prop) + ",");
        }
        result.pop_back();
        result.append("}");
        return result;
    }
    virtual void Deserialize(const std::string &json){};

    virtual std::vector<property> GetProperties() = 0;

    virtual ~JsonSerde(){};
};
