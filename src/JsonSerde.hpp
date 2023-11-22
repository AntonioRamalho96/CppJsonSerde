#pragma once

#include <string>
#include <vector>
#include <memory>

#include "rapidjson/document.h" // rapidjson's DOM-style API
#include "rapidjson/schema.h"   // For schema validation

#include "Property.hpp"

class JsonSerde
{
public:
    JsonSerde() = default;
    JsonSerde(const JsonSerde &){};
    JsonSerde &operator=(const JsonSerde &other);
    virtual ~JsonSerde() = default;

    /**
     * @brief The child class must implement to state which are its properties, for example:
     * struct some_class : public JsonSerde{
     *  string name;
     *  int age;
     *
     *  vector<property> DefineProperties(){
     *    return {
     *              MAKE_PROPERTY(name),
     *              MAKE_PROPERTY(age)
     *           };
     *  }
     * }
     *
     * @return std::vector<property> properties of the class, each property contains the
     *                                 name, type and pointer to the class member
     */
    virtual std::vector<property> DefineProperties() = 0;

    /**
     * @brief Serializes the object to a json string
     *
     * @param pretty if set to true the resulting json will contain linebreaks and indentation
     * @return std::string json representation of the object
     */
    virtual std::string Serialize(bool pretty = false) const;

    /**
     * @brief Deserializes a json object to set the class properties
     *
     * @param json string wirth the json representation of the object
     * @param validate if set to true the schema of the input json is checked
     */
    virtual void Deserialize(const std::string &json, bool validate = true);

    /**
     * @brief Returns the representation for the json schema of the present object
     *
     * @return std::string
     */
    std::string GetSchema() const;

    /**
     * @brief Validates a json against the schema. If it is a malformed json it throws InvalidJsonexception,
     * if there is a schema mismatch it throws InvalidSchemaException. It has no impact in the case the json
     * matches the schema
     *
     * @param json json to validate
     */
    void ValidateAgainstSchema(const std::string &json) const;

    /**
     * @brief Determines if the input json schema matches the current class's schema
     *
     * @param json input json
     * @param verbose in case of invalid, it prints the error to the console
     * @return true if the json matches the schema
     * @return false if the json is malformed or does not match the schema
     */
    bool IsValidAgainstSchema(const std::string &json, bool verbose = false) const noexcept;

    /**
     * @brief Receives a json and outputs a json with indentation.
     * It does not check weather the json is valid
     *
     * @param json input json
     * @return std::string output json (with indentation and linebreaks)
     */
    static std::string MakePretty(const std::string &json);

protected:
    /**
     * @brief Get the Properties object
     *
     * @return std::vector<property> properties of the class, each property contains the
     *                                 name, type and pointer to the class member
     */
    std::vector<property> &GetProperties();

    /**
     * @brief Get the Properties object, as read-only
     *
     * @return std::vector<property> properties of the class, each property contains the
     *                                 name, type and pointer to the class member
     */
    const std::vector<property> &GetPropertiesConst() const;

private:
    std::vector<property> m_properties{};
    bool m_properties_set{false};

    bool m_schema_set = false;
    std::unique_ptr<rapidjson::Document> m_schema{};

    void DeserializeWithRapidJson(const rapidjson::Value &doc);
    void SerializeWithRapidJson(rapidjson::Writer<rapidjson::StringBuffer> &writer) const;

    const rapidjson::Document &GetSchemaDocument() const;
    void WriteSchema(rapidjson::Writer<rapidjson::StringBuffer> &writer) const;

    void GetRapidjsonDocument(const std::string &json, bool validate_with_schema, rapidjson::Document &out_doc) const;

    friend class property;
};
