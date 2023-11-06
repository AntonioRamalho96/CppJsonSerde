#include "JsonSerde.hpp"

std::string JsonSerde::Serialize(bool pretty) const
{
    rapidjson::StringBuffer sb{};
    if (pretty)
    {
        rapidjson::PrettyWriter<rapidjson::StringBuffer> writer{sb};
        SerializeWithRapidJson(writer);
    }
    else
    {
        rapidjson::Writer<rapidjson::StringBuffer> writer{sb};
        SerializeWithRapidJson(writer);
    }
    return std::string(sb.GetString());
}
void JsonSerde::Deserialize(const std::string &json)
{
    rapidjson::Document document;
    document.Parse(json.c_str());
    DeserializeWithRapidJson(document);
}

void JsonSerde::DeserializeWithRapidJson(const rapidjson::Value &doc)
{
    for (auto &prop : GetProperties())
        prop.FromDocument(doc);
}

#define SERIALIZE_WITH_RAPIDJAOSN_BODY            \
    writer.StartObject();                         \
    for (const auto &prop : GetPropertiesConst()) \
    {                                             \
        writer.String(prop.name.c_str());         \
        prop.PlaceInWriter(writer);               \
    }                                             \
    writer.EndObject();

void JsonSerde::SerializeWithRapidJson(rapidjson::PrettyWriter<rapidjson::StringBuffer> &writer) const
{
    SERIALIZE_WITH_RAPIDJAOSN_BODY
}

void JsonSerde::SerializeWithRapidJson(rapidjson::Writer<rapidjson::StringBuffer> &writer) const {
    SERIALIZE_WITH_RAPIDJAOSN_BODY}

// Returns a reference to the properties of the object
std::vector<property> &JsonSerde::GetProperties()
{
    if (!m_properties_set)
    {
        m_properties = DefineProperties();
        m_properties_set = true;
    }
    return m_properties;
}

const std::vector<property> &JsonSerde::GetPropertiesConst() const
{
    return const_cast<JsonSerde *>(this)->GetProperties();
}