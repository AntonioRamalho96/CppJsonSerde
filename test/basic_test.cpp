#include "JsonSerde.hpp"
#include <iostream>

class SubClass : public JsonSerde
{
public:
    bool child;
    bool daughter;

    std::vector<property> GetProperties() override
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

    std::vector<property> GetProperties() override
    {
        return {
            MAKE_PROPERTY(name),
            MAKE_PROPERTY(job),
            MAKE_PROPERTY(family)};
    }
};

int main()
{
    SubClass family{};
    family.daughter = false;
    family.child = true;

    std::cout << family.Serialize() << std::endl;

    MyClass dude;
    dude.name = "Antonio";
    dude.job = "Developper";
    dude.family = family;
    std::cout << dude.Serialize() << std::endl;
};