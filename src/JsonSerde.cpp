#include "JsonSerde.hpp"

#include <iostream>

#include "rapidjson/prettywriter.h" // for stringify JSON
#include "rapidjson/filereadstream.h"

#include "exceptions/InvalidSchemaException.hpp"
#include "exceptions/InvalidJsonException.hpp"

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
void JsonSerde::Deserialize(const std::string &json, bool validate)
{
    using namespace rapidjson;
    Document document;
    GetRapidjsonDocument(json, validate, document);
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
    // DO NOT copy anything!
    // Copying properties would make the properties of the
    // assigned to object to point to the assigned from object properties
    // I am also not copying the schema because I am lazy, feel free to change
    return *this;
}

void JsonSerde::GetRapidjsonDocument(const std::string &json, bool validate_with_schema, rapidjson::Document &out_doc) const
{
    using namespace rapidjson;
    StringStream ss{json.c_str()};
    SchemaDocument schema{GetSchemaDocument()};
    SchemaValidatingReader<kParseDefaultFlags, StringStream, UTF8<>> reader(ss, schema);
    out_doc.Populate(reader);
    if (!reader.GetParseResult())
    {
        if (!reader.IsValid())
        {
            if (!validate_with_schema) // Check if schema should be disregarded
                return;
            throw InvalidSchemaException(reader);
        }
        else
            throw InvalidJsonException("The parsed json was not valid"); // TODO implement
    }
}

void JsonSerde::ValidateAgainstSchema(const std::string &json) const
{
    rapidjson::Document unused;
    GetRapidjsonDocument(json, true, unused);
}

bool JsonSerde::IsValidAgainstSchema(const std::string &json, bool verbose) const noexcept
{
    try
    {
        ValidateAgainstSchema(json);
    }
    catch (const JsonSerdeExcption &e)
    {
        if (verbose)
            std::cout << e.what() << std::endl;
        return false;
    }
    return true;
}