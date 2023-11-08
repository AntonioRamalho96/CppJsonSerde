#include <iostream>
#include <gtest/gtest.h>
#include "BaseClassVector.hpp"
#include "JsonSerde.hpp"

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
TEST(BaseClassVector, TestInstance)
{
    std::vector<Child> childs{};
    Child child1{};
    child1.some = 1;
    child1.other = 1;
    Child child2{};
    child2.some = 2;
    child2.other = 2;

    BaseClassVector<JsonSerde> my_vec(childs);

    EXPECT_EQ(my_vec.get_size(), 0);
    childs.push_back(child1);
    EXPECT_EQ(my_vec.GetSize(), 1);
    childs.push_back(child2);
    EXPECT_EQ(my_vec.GetSize(), 2);

    for (int i = 0; i < 2; i++)
    {
        std::cout << my_vec.Get(i)->Serialize() << std::endl;
    }
}