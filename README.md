# CppJsonSerde

This library exposes a parent class that can be used to easily serialize and desserialize C++ structures to json.
It also allows to generate json schemas for your classes.
It is built on top of [rapidjson](https://rapidjson.org/).

## Usage

Lets create a class Employee that extends our JsonSerde:

```cpp
// In Employee.hpp
#include <string>
#include "JsonSerde.hpp"

class Employee : public JsonSerde
{
public:
    std::string name;
    int age;

    std::vector<property> DefineProperties() override
    {
        return {
            MAKE_PROPERTY(name),
            MAKE_PROPERTY(age)};
    }
};
```

We need to implement _DefineProperties_ stating everything we want in the json, as in the example.
Now we can serialize and deserialize the objects:

```cpp
#include "Employee.hpp"
#include <iostream>
int main()
{
    // Serializing
    Employee john{};
    john.name = "John";
    john.age = 27;
    std::string json_1 = john.Serialize(); // Will output {"name": "John", "age":27 }
    JsonSerde::MakePretty(json_1); // Outputs same json but indented and with line breaks

    // Deserializing
    Employee marta{};
    std::string json_2 = "{\"name\": \"Marta\", \"age\":26 }";
    marta.Deserialize(json_2);
    assert(marta.name == "Marta");
    assert(marta.age == 26);
}

```

## More features

We support schema generation and schema validation. The full documentation can be found [here](./html_docs/index.html).

## Supported types

We support multiple property types:

- string
- int
- bool
- other JsonSerde objects
- vectors of any of the previous ones
- vectors of vectors etc

We do not support maps yet, but a PR is welcome!
Here is a more complex example, making use of other types of properties:

```cpp
#include "Employee.hpp"
#include <vector>

class Company : public JsonSerde
{
public:
std::string name;
bool public;
std::vector<Employee> employees;

    std::vector<property> DefineProperties() override
    {
        return {
            MAKE_PROPERTY(name),
            MAKE_PROPERTY(public),
            MAKE_PROPERTY(emplyees)};
    }

};
```
