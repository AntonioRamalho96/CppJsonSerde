#include <gtest/gtest.h>

#include "Type.hpp"

TEST(TestType, simple_type)
{
    std::string prop;
    Type type{&prop};
    ASSERT_EQ(type.type, PropertyType::STRING);
    ASSERT_EQ(type.sub_type.get(), nullptr);
}

TEST(TestType, complex_type)
{
    std::vector<std::string> prop;
    Type type(&prop);
    ASSERT_EQ(type.type, PropertyType::VECTOR);

    ASSERT_NE(type.sub_type.get(), nullptr);
    ASSERT_EQ(type.sub_type->type, PropertyType::STRING);
    ASSERT_EQ(type.sub_type->sub_type.get(), nullptr);
}

TEST(TestType, deep_type)
{
    std::vector<std::vector<std::string>> prop;
    Type type(&prop);

    ASSERT_EQ(type.type, PropertyType::VECTOR);
    ASSERT_NE(type.sub_type.get(), nullptr);

    auto &sub_type_1 = *(type.sub_type);
    ASSERT_EQ(sub_type_1.type, PropertyType::VECTOR);
    ASSERT_NE(sub_type_1.sub_type.get(), nullptr);

    auto &sub_type_2 = *(sub_type_1.sub_type);
    ASSERT_EQ(sub_type_2.type, PropertyType::STRING);
    ASSERT_EQ(sub_type_2.sub_type.get(), nullptr);
}