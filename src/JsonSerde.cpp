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

void JsonSerde::WriteSchema(rapidjson::Writer<rapidjson::StringBuffer> &writer) const
{
    writer.StartObject();
    // type
    writer.String("type");
    writer.String("object");

    // required (all fields for now)
    writer.String("required");
    writer.StartArray();
    for (const auto &prop : GetPropertiesConst())
        writer.String(prop.name.c_str());
    writer.EndArray();

    // properties
    writer.String("properties");
    writer.StartObject();
    for (const auto &prop : GetPropertiesConst())
    {
        writer.String(prop.name.c_str());
        prop.WriteSchema(writer);
    }
    writer.EndObject();

    writer.EndObject();
}

std::string JsonSerde::GetSchema() const
{
    using namespace rapidjson;
    StringBuffer sb;
    PrettyWriter<StringBuffer> writer(sb);
    GetSchemaDocument().Accept(writer);
    return sb.GetString();
}

const rapidjson::Document &JsonSerde::GetSchemaDocument() const
{
    using namespace rapidjson;
    if (!m_schema_set)
    {
        StringBuffer sb;
        Writer<StringBuffer> writer(sb);
        WriteSchema(writer);
        const_cast<JsonSerde *>(this)->m_schema = std::make_unique<rapidjson::Document>();
        const_cast<JsonSerde *>(this)->m_schema->Parse(sb.GetString());
        const_cast<JsonSerde *>(this)->m_schema_set = true;
    }
    return *m_schema;
}

JsonSerde &JsonSerde::operator=(const JsonSerde &other)
{
    // DO NOT copy properties or property is set
    // Copying properties would make the properties of the
    // assigned to object to point to the assigned from object properties
    // I am also not copying the schema because I am lazy, feel free to change
    return *this;
}