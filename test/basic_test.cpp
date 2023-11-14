#include "JsonSerde.hpp"
#include <iostream>
#include <gtest/gtest.h>

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

class SerdeTest : public ::testing::Test
{
protected:
    SubClass m_family{};
    MyClass m_dude{};

    virtual void SetUp()
    {
        // Arrange instance of dude
        m_family.daughter = false;
        m_family.child = true;

        m_dude.name = "Antonio";
        m_dude.job = "Developper";
        m_dude.family = m_family;
    }
};

TEST_F(SerdeTest, SerializationDesserialization)
{

    std::cout << m_family.Serialize() << std::endl;
    std::cout << m_dude.Serialize() << std::endl;

    std::string json_serialized{m_dude.Serialize()};
    MyClass other;
    other.Deserialize(json_serialized);
    EXPECT_EQ(other.Serialize(), json_serialized);
};

TEST_F(SerdeTest, SchemaGeneration)
{
    std::cout << m_dude.GetSchema() << std::endl;
    MyClass other{};
    ASSERT_EQ(other.GetSchema(), m_dude.GetSchema());
}

TEST_F(SerdeTest, TestSchemaValidationValid)
{
    std::string proper_json = "{\"name\":\"Antonio\",\"job\":\"Developper\",\"family\":{\"child\":true,\"daughter\":false}}";
    MyClass antonio;
    EXPECT_TRUE(antonio.IsValidAgainstSchema(proper_json));
}

TEST_F(SerdeTest, TestSchemaValidationFailDueToBadType)
{
    // Job is now a number
    std::string bad_json = "{\"name\":\"Antonio\",\"job\":1,\"family\":{\"child\":true,\"daughter\":false}}";
    MyClass antonio;
    EXPECT_FALSE(antonio.IsValidAgainstSchema(bad_json));
}

TEST_F(SerdeTest, TestSchemaValidationFailDueToMissingFieldInChildObject)
{
    // Job is now a number
    std::string bad_json = "{\"name\":\"Antonio\",\"job\":\"Developper\",\"family\":{\"daughter\":false}}";
    MyClass antonio;
    EXPECT_FALSE(antonio.IsValidAgainstSchema(bad_json));
}

TEST_F(SerdeTest, TestSchemaValidationFailDueToBadFieldInChildObject)
{
    // Job is now a number
    std::string bad_json = "{\"name\":\"Antonio\",\"job\":\"Developper\",\"family\":{\"child\":1,\"daughter\":false}}";
    MyClass antonio;
    EXPECT_FALSE(antonio.IsValidAgainstSchema(bad_json));
}

class AnotherObj : public JsonSerde
{
public:
    std::vector<std::string> multi_names{};

    std::vector<property> DefineProperties()
    {
        return {
            MAKE_PROPERTY(multi_names)};
    }
};

TEST_F(SerdeTest, TestClassWithArrays)
{
    AnotherObj source;
    source.DefineProperties();
    source.multi_names = {"Hello", "Goodbye"};
    std::string serialized = source.Serialize();
    std::cout << serialized << std::endl;
    AnotherObj receiver{};
    receiver.Deserialize(serialized);

    EXPECT_EQ(source.multi_names, receiver.multi_names);
}

class ObjWithVectorOfVectors : public JsonSerde
{
public:
    std::vector<std::vector<std::string>> multi_names{};

    std::vector<property> DefineProperties()
    {
        return {
            MAKE_PROPERTY(multi_names)};
    }
};

TEST_F(SerdeTest, TestClassWithArraysOfArrays)
{
    ObjWithVectorOfVectors source;
    source.DefineProperties();
    source.multi_names = {{"Hello", "Goodbye"}, {"Yes", "No"}};
    std::string serialized = source.Serialize();
    std::cout << serialized << std::endl;
    ObjWithVectorOfVectors receiver{};
    receiver.Deserialize(serialized);

    EXPECT_EQ(source.multi_names.size(), 2);
    EXPECT_EQ(source.multi_names.size(), receiver.multi_names.size());
    EXPECT_EQ(source.multi_names, receiver.multi_names);
}

class ComplexSchemaObject : public JsonSerde
{
public:
    std::string name{};
    std::vector<std::vector<ObjWithVectorOfVectors>> multi_names{};

    std::vector<property> DefineProperties()
    {
        return {
            MAKE_PROPERTY(name),
            MAKE_PROPERTY(multi_names)};
    }
};

TEST_F(SerdeTest, TestComplexSchema)
{
    std::cout << ComplexSchemaObject().GetSchema() << std::endl;
}
