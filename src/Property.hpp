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

struct property
{
    std::string name;
    PropertyType type;

    // TODO make union
    void *ptr;
    GenericVector vec;

    property &operator=(const property &other)
    {
        name = other.name;
        type = other.type;
        if (type == PropertyType::VECTOR)
            vec = other.vec;
        else
            ptr = other.ptr;
        return *this;
    }

    template <typename SimpleType>
    property(const std::string &name_arg, SimpleType &arg) : name{name_arg}, type{Type(&arg).type}, ptr{(void *)(&arg)} {}

    template <class ElementType>
    property(const std::string &name_arg, std::vector<ElementType> &stuff_vector)
    {
        name = name_arg;
        type = Type(&stuff_vector).type;
        vec = GenericVector(stuff_vector);
    }

    void PlaceInWriter(rapidjson::Writer<rapidjson::StringBuffer> &writer) const;
    void PlaceInWriter(rapidjson::PrettyWriter<rapidjson::StringBuffer> &writer) const;

    void FromDocument(const rapidjson::Value &doc);
    void WriteSchema(rapidjson::Writer<rapidjson::StringBuffer> &writer) const;

    ~property() = default;

private:
    static void PlaceInWriterStatic(const PropertyType &type, const void *ptr, rapidjson::Writer<rapidjson::StringBuffer> &writer);
    static void PlaceInWriterStatic(const PropertyType &type, const void *ptr, rapidjson::PrettyWriter<rapidjson::StringBuffer> &writer);
    static void PlaceInWriterStatic(const PropertyType &type, const GenericVector &vec, rapidjson::Writer<rapidjson::StringBuffer> &writer);
    static void PlaceInWriterStatic(const PropertyType &type, const GenericVector &vec, rapidjson::PrettyWriter<rapidjson::StringBuffer> &writer);

    static void FromDocumentStatic(const PropertyType &type, void *ptr, const rapidjson::Value &param);
    static void FromDocumentStatic(GenericVector &vec, const rapidjson::Value &param);

    static void WriteSchemaStatic(const GenericVector &vec, rapidjson::Writer<rapidjson::StringBuffer> &writer);
    static void WriteSchemaStatic(const PropertyType &type, void *ptr, rapidjson::Writer<rapidjson::StringBuffer> &writer);
};