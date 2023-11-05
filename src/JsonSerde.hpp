#pragma once
#include <string>
#include <vector>

#include "rapidjson/document.h"     // rapidjson's DOM-style API
#include "rapidjson/prettywriter.h" // for stringify JSON

#include "JsonSerdeStatics.hpp"

class JsonSerde
{
public:
    virtual std::string Serialize() const
    {
        rapidjson::StringBuffer sb;
        rapidjson::Writer<rapidjson::StringBuffer> writer{sb};
        SerializeWithRapidJson(writer);
        return std::string(sb.GetString());
    }
    virtual void Deserialize(const std::string &json)
    {
        rapidjson::Document document;
        document.Parse(json.c_str());
        DeserializeWithRapidJson(document);
    }

    // Defines the properties of the object
    virtual std::vector<property> DefineProperties() = 0;

    virtual ~JsonSerde() = default;

    void DeserializeWithRapidJson(const rapidjson::Value &doc)
    {
        for (auto &prop : GetProperties())
            JsonSerdeStatics::FromDocument(doc, prop);
    }

    void SerializeWithRapidJson(rapidjson::Writer<rapidjson::StringBuffer> &writer) const
    {

        writer.StartObject();
        for (const auto &prop : GetPropertiesConst())
        {
            writer.String(prop.name.c_str());
            JsonSerdeStatics::PlaceInWriter(writer, prop);
        }
        writer.EndObject();
    }

    // Returns a reference to the properties of the object
    std::vector<property> &GetProperties()
    {
        if (!m_properties_set)
        {
            m_properties = DefineProperties();
            m_properties_set = true;
        }
        return m_properties;
    }

    const std::vector<property> &GetPropertiesConst() const
    {

        return const_cast<JsonSerde *>(this)->GetProperties();
    }

private:
    std::vector<property> m_properties{};
    bool m_properties_set{false};
};
