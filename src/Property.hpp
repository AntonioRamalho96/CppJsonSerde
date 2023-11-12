#pragma once

#include <string>
#include <vector>
#include <memory>
#include <functional>

#include "rapidjson/document.h"     // rapidjson's DOM-style API
#include "rapidjson/prettywriter.h" // for stringify JSON

#include "GenericVector.hpp"
#include "Type.hpp"

class JsonSerde;

#define MAKE_PROPERTY(prop) std::move(property(#prop, prop))

union PropertyPtr
{
    std::string *str;
    bool *bl;
    int *num;
    JsonSerde *json_serde;
    GenericVector *vector;
};

struct property
{
    std::string name;
    Type type;
    PropertyPtr ptr;

    explicit property(const std::string &name_arg, std::string &str);

    explicit property(const std::string &name_arg, bool &bl);

    explicit property(const std::string &name_arg, int &num);

    explicit property(const std::string &name_arg, JsonSerde &json_serde);

    template <class ElementType>
    property(const std::string &name_arg, std::vector<ElementType> &stuff_vector)
    {
        name = name_arg;
        type = Type(&stuff_vector);
        ptr.vector = new GenericVector(stuff_vector);
    }

    ~property();

    void PlaceInWriter(rapidjson::Writer<rapidjson::StringBuffer> &writer) const;
    void PlaceInWriter(rapidjson::PrettyWriter<rapidjson::StringBuffer> &writer) const;

    void FromDocument(const rapidjson::Value &doc);
    void WriteSchema(rapidjson::Writer<rapidjson::StringBuffer> &writer) const;
};