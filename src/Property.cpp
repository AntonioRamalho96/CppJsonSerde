#include "Property.hpp"
#include "JsonSerde.hpp"

#include <iostream>

property::property(const std::string &name_arg, std::string &str) : name{name_arg},
                                                                    type{Type(&str)}
{
    ptr.str = &str;
}

property::property(const std::string &name_arg, bool &bl) : name{name_arg},
                                                            type{Type(&bl)}
{
    ptr.bl = &bl;
}

property::property(const std::string &name_arg, int &num) : name{name_arg},
                                                            type{Type(&num)}
{
    ptr.num = &num;
}

property::property(const std::string &name_arg, JsonSerde &json_serde) : name{name_arg},
                                                                         type{Type(&json_serde)}
{
    ptr.json_serde = &json_serde;
}

#define PLACE_IN_WRITER_BODY // TODO

void property::PlaceInWriter(rapidjson::Writer<rapidjson::StringBuffer> &writer) const
{

    switch (this->type.type)
    {
    case PropertyType::STRING:
        writer.String(this->ptr.str->c_str());
        return;
    case PropertyType::BOOL:
        writer.Bool(*(this->ptr.bl));
        return;
    case PropertyType::SERDE:
        this->ptr.json_serde->SerializeWithRapidJson(writer);
        return;
    case PropertyType::NUMBER:
        writer.Int(*(this->ptr.num));
        return;
    case PropertyType::VECTOR:
        writer.StartArray();
        // TODO implement
        writer.EndArray();
        return;
    }
    throw;
}

property::~property()
{
    if (type.type == PropertyType::VECTOR)
        delete ptr.vector;
}

void property::PlaceInWriter(rapidjson::PrettyWriter<rapidjson::StringBuffer> &writer) const
{
    switch (this->type.type)
    {
    case PropertyType::STRING:
        writer.String(this->ptr.str->c_str());
        return;
    case PropertyType::BOOL:
        writer.Bool(*(this->ptr.bl));
        return;
    case PropertyType::SERDE:
        this->ptr.json_serde->SerializeWithRapidJson(writer);
        return;
    case PropertyType::NUMBER:
        writer.Int(*(this->ptr.num));
        return;
        // case PropertyType::VEC_STRING:
        //     writer.StartArray();
        //     for (const std::string &str : *(this->ptr.vec_str))
        //         writer.String(str.c_str());
        //     writer.EndArray();
        //     return;
    }
    throw;
}

void property::FromDocument(const rapidjson::Value &doc)
{
    const rapidjson::Value &param = doc[this->name.c_str()];
    switch (this->type.type)
    {
    case PropertyType::STRING:
        *(this->ptr.str) = param.GetString();
        return;
    case PropertyType::BOOL:
        *(this->ptr.bl) = param.GetBool();
        return;
    case PropertyType::SERDE:
        this->ptr.json_serde->DeserializeWithRapidJson(param);
        return;
    case PropertyType::NUMBER:
        *(this->ptr.num) = param.GetInt();
        return;

        // #define PARSE_ARRAY(ptr_name, method)      \
//     this->ptr.ptr_name->clear();           \
//     for (int i = 0; i < param.Size(); i++) \
//         this->ptr.ptr_name->push_back(param[i].method());
        //
        //     case PropertyType::VECTOR:
        //         PARSE_ARRAY(vec_str, GetString)
        //         return;
    }
#undef PARSE_ARRAY
    throw;
}

void property::WriteSchema(rapidjson::Writer<rapidjson::StringBuffer> &writer) const
{
#define ADD_SIMPLE_TYPE_SCHEME(type_name) \
    writer.StartObject();                 \
    writer.String("type");                \
    writer.String(type_name);             \
    writer.EndObject();

    switch (this->type.type)
    {
    case PropertyType::STRING:
        ADD_SIMPLE_TYPE_SCHEME("string")
        break;
    case PropertyType::BOOL:
        ADD_SIMPLE_TYPE_SCHEME("boolean")
        break;
    case PropertyType::SERDE:
        this->ptr.json_serde->WriteSchema(writer);
        break;
    case PropertyType::NUMBER:
        ADD_SIMPLE_TYPE_SCHEME("integer")
        break;
    default:
        throw;
    }

#undef ADADD_SIMPLE_TYPE_SCHEME
}

bool JsonSerde::IsValidAgainstSchema(const std::string &json, bool verbose)
{
    using namespace rapidjson;
    // sd is no longer needed here.

    // TODO refactor to separate method taht returns document
    Document d;
    if (d.Parse(json.c_str()).HasParseError())
    {
        // TODO replace for exception
        if (verbose)
        {
            std::cout << "Provided string does not have a valid json format" << std::endl;
        }
        return false;
    }

    SchemaDocument schema(GetSchemaDocument()); // Compile a Document to SchemaDocument
    SchemaValidator validator(schema);
    if (!d.Accept(validator))
    {
        // TODO replace for exception
        // Input JSON is invalid according to the schema
        // Output diagnostic information
        if (verbose)
        {

            StringBuffer sb;
            validator.GetInvalidSchemaPointer().StringifyUriFragment(sb);
            printf("Invalid schema: %s\n", sb.GetString());
            printf("Invalid keyword: %s\n", validator.GetInvalidSchemaKeyword());
            sb.Clear();
            validator.GetInvalidDocumentPointer().StringifyUriFragment(sb);
            printf("Invalid document: %s\n", sb.GetString());
        }
        return false;
    }
    return true;
}
