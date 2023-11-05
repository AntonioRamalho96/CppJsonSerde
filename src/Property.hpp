#pragma once

#include <string>

class JsonSerde;

#define MAKE_PROPERTY(prop) property(#prop, prop)
enum PropertyType
{
    STRING,
    BOOL,
    NUMBER,
    SERDE
};

union PropertyPtr
{
    std::string *str;
    bool *bl;
    int *num;
    JsonSerde *json_serde;
};

struct property
{
    std::string name;
    PropertyType type;
    PropertyPtr ptr;

    explicit property(const std::string &name_arg, std::string &str)
    {
        name = name_arg;
        type = PropertyType::STRING;
        ptr.str = &str;
    }

    explicit property(const std::string &name_arg, bool &bl)
    {
        name = name_arg;
        type = PropertyType::BOOL;
        ptr.bl = &bl;
    }

    explicit property(const std::string &name_arg, int &num)
    {
        name = name_arg;
        type = PropertyType::NUMBER;
        ptr.num = &num;
    }

    explicit property(const std::string &name_arg, JsonSerde &json_serde)
    {
        name = name_arg;
        type = PropertyType::SERDE;
        ptr.json_serde = &json_serde;
    }
};

struct ConstProperty
{
    ConstProperty(const property &prop_in)
        : prop{prop_in}
    {
    }
    const property prop;
};