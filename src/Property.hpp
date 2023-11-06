#pragma once

#include <string>

#include "rapidjson/document.h"     // rapidjson's DOM-style API
#include "rapidjson/prettywriter.h" // for stringify JSON

class JsonSerde;

#define MAKE_PROPERTY(prop) std::move(property(#prop, prop))
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

    explicit property(const std::string &name_arg, std::string &str);

    explicit property(const std::string &name_arg, bool &bl);

    explicit property(const std::string &name_arg, int &num);

    explicit property(const std::string &name_arg, JsonSerde &json_serde);

    void PlaceInWriter(rapidjson::Writer<rapidjson::StringBuffer> &writer) const;
    void PlaceInWriter(rapidjson::PrettyWriter<rapidjson::StringBuffer> &writer) const;

    void FromDocument(const rapidjson::Value &doc);
};