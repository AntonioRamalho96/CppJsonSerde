#pragma once
#include <string>
#include <vector>
#include <memory>
#include "Property.hpp"
#include "rapidjson/document.h"     // rapidjson's DOM-style API
#include "rapidjson/prettywriter.h" // for stringify JSON
#include "rapidjson/schema.h"       // For schema validation

class JsonSerde
{
public:
    JsonSerde &operator=(const JsonSerde &other);
    JsonSerde() = default;
    JsonSerde(const JsonSerde &){};

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
     */
    virtual void Deserialize(const std::string &json);

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

    virtual ~JsonSerde() = default;

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

    /**
     * @brief Returns the representation for the json schema of the present object
     *
     * @return std::string
     */
    std::string GetSchema() const;

    static std::string MakePretty(const std::string &json);

    bool IsValidAgainstSchema(const std::string &json, bool verbose = true);

private:
    std::vector<property> m_properties{};
    bool m_properties_set{false};

    bool m_schema_set = false;
    std::unique_ptr<rapidjson::Document> m_schema{};

    void DeserializeWithRapidJson(const rapidjson::Value &doc);
    void SerializeWithRapidJson(rapidjson::Writer<rapidjson::StringBuffer> &writer) const;

    const rapidjson::Document &GetSchemaDocument() const;
    void WriteSchema(rapidjson::Writer<rapidjson::StringBuffer> &writer) const;

    friend class property;
};
