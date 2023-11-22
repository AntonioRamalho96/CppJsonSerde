#pragma once

#include <sstream>

#include "rapidjson/schema.h"

#include "JsonSerdeException.hpp"

typedef rapidjson::SchemaValidatingReader<rapidjson::kParseDefaultFlags, rapidjson::StringStream, rapidjson::UTF8<>> RJSchemaValidatingReader;

class InvalidSchemaException : public JsonSerdeExcption
{

public:
    InvalidSchemaException(const std::string &msg) : msg{msg} {}
    InvalidSchemaException(const RJSchemaValidatingReader &reader) : InvalidSchemaException{GetMessage(reader)} {}

    static std::string GetMessage(const RJSchemaValidatingReader &reader)
    {
        // Input JSON is invalid according to the schema
        // Output diagnostic information
        using namespace rapidjson;
        StringBuffer sb;
        std::stringstream message{};

        reader.GetInvalidSchemaPointer().StringifyUriFragment(sb);
        message << "Invalid schema: " << sb.GetString() << std::endl;
        message << "Invalid keyword: " << reader.GetInvalidSchemaKeyword() << std::endl;
        sb.Clear();
        reader.GetInvalidDocumentPointer().StringifyUriFragment(sb);
        message << "Invalid json: " << sb.GetString() << std::endl;

        return message.str();
    }

    const char *what() const noexcept override
    {
        return msg.c_str();
    }

private:
    std::string msg;
};