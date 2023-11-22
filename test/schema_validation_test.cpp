#include <gtest/gtest.h>

#include "JsonSerde.hpp"
#include "exceptions/InvalidSchemaException.hpp"
#include "exceptions/InvalidJsonException.hpp"
#include <string>

class Person : public JsonSerde
{
    std::string name;
    int age;

    std::vector<property> DefineProperties() override
    {
        return {MAKE_PROPERTY(name), MAKE_PROPERTY(age)};
    }
};

TEST(SchemaValidationTest, ValidJson)
{
    Person person{};
    ASSERT_NO_THROW(person.Deserialize("{\"name\":\"antonio\", \"age\": 27 }"));
}

TEST(SchemaValidationTest, InvalidJson)
{
    Person person{};
    ASSERT_THROW(person.Deserialize("{\"name\":antonio\", \"age\": 27 }"), InvalidJsonException);
}

TEST(SchemaValidationTest, InvalidSchema)
{
    Person person{};
    ASSERT_THROW(person.Deserialize("{\"name\":\"antonio\",\"age\":\"27\"}  "), InvalidSchemaException);
}