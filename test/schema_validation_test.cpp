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

class SubClass : public JsonSerde
{
public:
    bool child;
    bool daughter;

    std::vector<property> DefineProperties() override
    {
        return {
            MAKE_PROPERTY(child),
            MAKE_PROPERTY(daughter)};
    }
};

class MyClass : public JsonSerde
{
public:
    std::string name;
    std::string job;
    SubClass family;

    std::vector<property> DefineProperties() override
    {
        return {
            MAKE_PROPERTY(name),
            MAKE_PROPERTY(job),
            MAKE_PROPERTY(family)};
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

TEST(SchemaValidationTest, TestSchemaValidationValid)
{
    std::string proper_json = "{\"name\":\"Antonio\",\"job\":\"Developper\",\"family\":{\"child\":true,\"daughter\":false}}";
    MyClass antonio;
    EXPECT_TRUE(antonio.IsValidAgainstSchema(proper_json, true));
}

TEST(SchemaValidationTest, TestSchemaValidationFailDueToBadType)
{
    // Job is now a number
    std::string bad_json = "{\"name\":\"Antonio\",\"job\":1,\"family\":{\"child\":true,\"daughter\":false}}";
    MyClass antonio;
    EXPECT_FALSE(antonio.IsValidAgainstSchema(bad_json, true));
}

TEST(SchemaValidationTest, TestSchemaValidationFailDueToMissingFieldInChildObject)
{
    // Job is now a number
    std::string bad_json = "{\"name\":\"Antonio\",\"job\":\"Developper\",\"family\":{\"daughter\":false}}";
    MyClass antonio;
    EXPECT_FALSE(antonio.IsValidAgainstSchema(bad_json, true));
}

TEST(SerdeTest, TestSchemaValidationFailDueToBadFieldInChildObject)
{
    // Job is now a number
    std::string bad_json = "{\"name\":\"Antonio\",\"job\":\"Developper\",\"family\":{\"child\":1,\"daughter\":false}}";
    MyClass antonio;
    EXPECT_FALSE(antonio.IsValidAgainstSchema(bad_json, true));
}
