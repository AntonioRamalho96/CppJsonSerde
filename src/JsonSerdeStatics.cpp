#include "JsonSerdeStatics.hpp"
#include "JsonSerde.hpp"

namespace JsonSerdeStatics
{
    std::string ToJson(JsonSerde &obj)
    {
        return obj.Serialize();
    }

    std::string ToJson(std::string &obj)
    {
        return "\"" + obj + "\"";
    }

    std::string ToJson(bool &obj)
    {
        return obj ? "true" : "false";
    }

    std::string ToJson(int &obj)
    {
        return std::to_string(obj);
    }

    std::string ToJson(property &property)
    {
        switch (property.type)
        {
        case PropertyType::STRING:
            return ToJson(*(property.ptr.str));
        case PropertyType::BOOL:
            return ToJson(*(property.ptr.bl));
        case PropertyType::SERDE:
            return ToJson(*(property.ptr.json_serde));
        }
        return "ERROR";
    }
}