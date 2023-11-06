#include "Property.hpp"
#include "JsonSerde.hpp"

property::property(const std::string &name_arg, std::string &str) : name{name_arg},
                                                                    type{PropertyType::STRING}
{
    ptr.str = &str;
}

property::property(const std::string &name_arg, bool &bl) : name{name_arg},
                                                            type{PropertyType::BOOL}
{
    ptr.bl = &bl;
}

property::property(const std::string &name_arg, int &num) : name{name_arg},
                                                            type{PropertyType::NUMBER}
{
    ptr.num = &num;
}

property::property(const std::string &name_arg, JsonSerde &json_serde) : name{name_arg},
                                                                         type{PropertyType::SERDE}
{
    ptr.json_serde = &json_serde;
}

void property::PlaceInWriter(rapidjson::Writer<rapidjson::StringBuffer> &writer) const
{

    switch (this->type)
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
    }
    throw;
}

void property::PlaceInWriter(rapidjson::PrettyWriter<rapidjson::StringBuffer> &writer) const
{
    switch (this->type)
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
    }
    throw;
}

void property::FromDocument(const rapidjson::Value &doc)
{
    const rapidjson::Value &param = doc[this->name.c_str()];
    switch (this->type)
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
    }
    throw;
}