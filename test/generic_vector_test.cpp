#include <iostream>
#include <gtest/gtest.h>
#include "GenericVector.hpp"
#include "JsonSerde.hpp"

void DisplayVector(const GenericVector &vec);

class Child : public JsonSerde
{
public:
    int some;
    int other;

    std::vector<property> DefineProperties()
    {
        return {MAKE_PROPERTY(some), MAKE_PROPERTY(other)};
    }
};

class GenericVectorTest : public ::testing::Test
{
protected:
    Child m_elem_1{};
    Child m_elem_2{};
    void SetUp() override
    {
        m_elem_1.some = 1;
        m_elem_1.other = 1;

        m_elem_2.some = 2;
        m_elem_2.other = 2;
    }
};

TEST_F(GenericVectorTest, TestGetSize)
{
    std::vector<Child> childs{};
    GenericVector my_vec(childs);

    EXPECT_EQ(my_vec.GetSize(), 0);
    childs.push_back(m_elem_1);
    EXPECT_EQ(my_vec.GetSize(), 1);
    childs.push_back(m_elem_2);
    EXPECT_EQ(my_vec.GetSize(), 2);

    DisplayVector(my_vec);
}

TEST_F(GenericVectorTest, TestAddElement)
{
    std::vector<Child> childs{};
    GenericVector my_vec(childs);

    EXPECT_EQ(my_vec.GetSize(), 0);
    my_vec.AddElement((void *)&m_elem_1);
    EXPECT_EQ(my_vec.GetSize(), 1);
    my_vec.AddElement((void *)&m_elem_2);
    EXPECT_EQ(my_vec.GetSize(), 2);

    DisplayVector(my_vec);
}

void DisplayVector(const GenericVector &vec)
{
    for (int i = 0; i < 2; i++)
    {
        std::cout << ((const JsonSerde *)(vec.Get(i)))->Serialize() << std::endl;
    }
}