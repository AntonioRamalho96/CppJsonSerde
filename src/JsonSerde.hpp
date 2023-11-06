#pragma once
#include <string>
#include <vector>
#include <memory>
#include "Property.hpp"
#include "rapidjson/document.h"     // rapidjson's DOM-style API
#include "rapidjson/prettywriter.h" // for stringify JSON

class JsonSerde
{
public:
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

private:
    std::vector<property> m_properties{};
    bool m_properties_set{false};

    void DeserializeWithRapidJson(const rapidjson::Value &doc);
    void SerializeWithRapidJson(rapidjson::PrettyWriter<rapidjson::StringBuffer> &writer) const;
    void SerializeWithRapidJson(rapidjson::Writer<rapidjson::StringBuffer> &writer) const;
    friend class property;
};
