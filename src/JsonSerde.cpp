#include "JsonSerde.hpp"
#include "rapidjson/prettywriter.h" // for stringify JSON

std::string JsonSerde::MakePretty(const std::string &json)
{
    rapidjson::Document document{};
    document.Parse(json.c_str());
    rapidjson::StringBuffer sb{};
    rapidjson::PrettyWriter<rapidjson::StringBuffer> pretty_writer(sb);
    return sb.GetString();
}

std::string JsonSerde::Serialize(bool pretty) const
{
    rapidjson::StringBuffer sb{};
    rapidjson::Writer<rapidjson::StringBuffer> writer{sb};
    SerializeWithRapidJson(writer);
    if (pretty)
        return MakePretty(sb.GetString());
    return sb.GetString();
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

void JsonSerde::SerializeWithRapidJson(rapidjson::Writer<rapidjson::StringBuffer> &writer) const
{
    writer.StartObject();
    for (const auto &prop : GetPropertiesConst())
    {
        writer.String(prop.name.c_str());
        prop.PlaceInWriter(writer);
    }
    writer.EndObject();
}

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
    Writer<StringBuffer> writer(sb);
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