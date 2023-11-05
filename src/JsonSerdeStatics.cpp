#include "JsonSerdeStatics.hpp"
#include "JsonSerde.hpp"

namespace JsonSerdeStatics
{
    void PlaceInWriter(rapidjson::Writer<rapidjson::StringBuffer> &writer, const int &prop)
    {
        writer.Int(prop);
    }
    void PlaceInWriter(rapidjson::Writer<rapidjson::StringBuffer> &writer, const bool &prop)
    {
        writer.Bool(prop);
    }
    void PlaceInWriter(rapidjson::Writer<rapidjson::StringBuffer> &writer, const std::string &prop)
    {
        writer.String(prop.c_str());
    }
    void PlaceInWriter(rapidjson::Writer<rapidjson::StringBuffer> &writer, const JsonSerde &prop)
    {
        prop.SerializeWithRapidJson(writer);
    }
    void PlaceInWriter(rapidjson::Writer<rapidjson::StringBuffer> &writer, const property &prop)
    {
        switch (prop.type)
        {
        case PropertyType::STRING:
            PlaceInWriter(writer, *(prop.ptr.str));
            return;
        case PropertyType::BOOL:
            PlaceInWriter(writer, *(prop.ptr.bl));
            return;
        case PropertyType::SERDE:
            PlaceInWriter(writer, *(prop.ptr.json_serde));
            return;
        case PropertyType::NUMBER:
            PlaceInWriter(writer, *(prop.ptr.num));
            return;
        }
        throw;
    }

    void FromDocument(const rapidjson::Value &doc, const std::string &name, int &prop)
    {
        prop = doc[name.c_str()].GetInt();
    }
    void FromDocument(const rapidjson::Value &doc, const std::string &name, bool &prop)
    {
        prop = doc[name.c_str()].GetBool();
    }
    void FromDocument(const rapidjson::Value &doc, const std::string &name, std::string &prop)
    {
        prop = doc[name.c_str()].GetString();
    }
    void FromDocument(const rapidjson::Value &doc, const std::string &name, JsonSerde &prop)
    {
        prop.DeserializeWithRapidJson(doc[name.c_str()].GetObject());
    }
    void FromDocument(const rapidjson::Value &doc, property &prop)
    {
        switch (prop.type)
        {
        case PropertyType::STRING:
            FromDocument(doc, prop.name, *(prop.ptr.str));
            return;
        case PropertyType::BOOL:
            FromDocument(doc, prop.name, *(prop.ptr.bl));
            return;
        case PropertyType::SERDE:
            FromDocument(doc, prop.name, *(prop.ptr.json_serde));
            return;
        case PropertyType::NUMBER:
            FromDocument(doc, prop.name, *(prop.ptr.num));
            return;
        }
        throw;
    }
}