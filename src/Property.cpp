#include "Property.hpp"
#include "JsonSerde.hpp"

#include <iostream>

#define PLACE_IN_WRITER_BODY // TODO

void property::PlaceInWriterStatic(const PropertyType &type, const void *ptr, rapidjson::Writer<rapidjson::StringBuffer> &writer)
{
    switch (type)
    {
    case PropertyType::STRING:
        writer.String(((const std::string *)ptr)->c_str());
        return;
    case PropertyType::BOOL:
        writer.Bool(*(const bool *)(ptr));
        return;
    case PropertyType::SERDE:
        ((const JsonSerde *)ptr)->SerializeWithRapidJson(writer);
        return;
    case PropertyType::NUMBER:
        writer.Int(*((const int *)ptr));
        return;
    }
    throw;
}

void property::PlaceInWriterStatic(const PropertyType &type, const GenericVector &vec, rapidjson::Writer<rapidjson::StringBuffer> &writer)
{
    PropertyType element_type = vec.element_type.type;
    size_t size = vec.GetSize();
    writer.StartArray();
    for (int i = 0; i < size; i++)
        if (element_type != PropertyType::VECTOR)
            PlaceInWriterStatic(element_type, vec.Get(i), writer);
        else
            PlaceInWriterStatic(element_type, vec.GetGenericVector(i), writer);
    writer.EndArray();
}

void property::PlaceInWriter(rapidjson::Writer<rapidjson::StringBuffer> &writer) const
{
    if (type == PropertyType::VECTOR)
        PlaceInWriterStatic(this->type, this->vec, writer);
    else
        PlaceInWriterStatic(this->type, this->ptr, writer);
}

void property::FromDocument(const rapidjson::Value &doc)
{
    const rapidjson::Value &param = doc[this->name.c_str()];
    if (this->type == PropertyType::VECTOR)
        FromDocumentStatic(this->vec, param);
    else
        FromDocumentStatic(this->type, this->ptr, param);
}

void property::WriteSchema(rapidjson::Writer<rapidjson::StringBuffer> &writer) const
{
    if (type == PropertyType::VECTOR)
        WriteSchemaStatic(vec, writer);
    else
        WriteSchemaStatic(type, ptr, writer);
}

void property::FromDocumentStatic(const PropertyType &type, void *ptr, const rapidjson::Value &param)
{
    switch (type)
    {
    case PropertyType::STRING:
        *((std::string *)ptr) = param.GetString();
        return;
    case PropertyType::BOOL:
        *((bool *)ptr) = param.GetBool();
        return;
    case PropertyType::SERDE:
        ((JsonSerde *)ptr)->DeserializeWithRapidJson(param);
        return;
    case PropertyType::NUMBER:
        *((int *)ptr) = param.GetInt();
        return;
    }
    throw std::runtime_error("Unnexpected behaviour, unknown type");
}
void property::FromDocumentStatic(GenericVector &vec, const rapidjson::Value &array)
{
    std::string str{};
    GenericVector aux_vec{};
    vec.Clear();
    for (int i = 0; i < array.Size(); i++)
    {
        vec.AddElement();
        switch (vec.element_type.type)
        {
        case PropertyType::STRING:
            *((std::string *)vec.Get(i)) = array[i].GetString();
            break;
        case PropertyType::BOOL:
            *((bool *)vec.Get(i)) = array[i].GetBool();
            break;
        case PropertyType::SERDE:
            ((JsonSerde *)vec.Get(i))->DeserializeWithRapidJson(array[i]);
            break;
        case PropertyType::NUMBER:
            *((bool *)vec.Get(i)) = array[i].GetInt();
            break;
        case PropertyType::VECTOR:
            aux_vec = vec.GetGenericVector(i);
            FromDocumentStatic(aux_vec, array[i]);
            break;
        }
    }
}

void property::WriteSchemaStatic(const GenericVector &vec, rapidjson::Writer<rapidjson::StringBuffer> &writer)
{
    writer.StartObject();
    writer.String("type");
    writer.String("array");
    writer.String("items");
    if (vec.element_type.type == PropertyType::VECTOR)
    {
        GenericVector aux = vec.GetGenericVectorInstance();
        WriteSchemaStatic(aux, writer);
    }
    else
    {
        void *ptr = vec.GetElementInstance();
        WriteSchemaStatic(vec.element_type.type, ptr, writer);
        vec.DeleteElementInstance(ptr);
    }
    writer.EndObject();
}

void property::WriteSchemaStatic(const PropertyType &type, void *ptr, rapidjson::Writer<rapidjson::StringBuffer> &writer)
{
#define ADD_SIMPLE_TYPE_SCHEME(type_name) \
    writer.StartObject();                 \
    writer.String("type");                \
    writer.String(type_name);             \
    writer.EndObject();

    switch (type)
    {
    case PropertyType::STRING:
        ADD_SIMPLE_TYPE_SCHEME("string")
        break;
    case PropertyType::BOOL:
        ADD_SIMPLE_TYPE_SCHEME("boolean")
        break;
    case PropertyType::SERDE:
        ((JsonSerde *)ptr)->WriteSchema(writer);
        break;
    case PropertyType::NUMBER:
        ADD_SIMPLE_TYPE_SCHEME("integer")
        break;
    default:
        throw;
    }

#undef ADADD_SIMPLE_TYPE_SCHEME
}