#pragma once

#include <string>

#include "rapidjson/document.h"     // rapidjson's DOM-style API
#include "rapidjson/prettywriter.h" // for stringify JSON

#include "Property.hpp"

class JsonSerde;

namespace JsonSerdeStatics
{
    void PlaceInWriter(rapidjson::Writer<rapidjson::StringBuffer> &writer, const int &prop);
    void PlaceInWriter(rapidjson::Writer<rapidjson::StringBuffer> &writer, const bool &prop);
    void PlaceInWriter(rapidjson::Writer<rapidjson::StringBuffer> &writer, const std::string &prop);
    void PlaceInWriter(rapidjson::Writer<rapidjson::StringBuffer> &writer, const JsonSerde &prop);
    void PlaceInWriter(rapidjson::Writer<rapidjson::StringBuffer> &writer, const property &prop);

    void FromDocument(const rapidjson::Value &doc, const std::string &name, int &prop);
    void FromDocument(const rapidjson::Value &doc, const std::string &name, bool &prop);
    void FromDocument(const rapidjson::Value &doc, const std::string &name, std::string &prop);
    void FromDocument(const rapidjson::Value &doc, const std::string &name, JsonSerde &prop);
    void FromDocument(const rapidjson::Value &doc, property &prop);
};