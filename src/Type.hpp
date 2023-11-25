#pragma once

#include <string>
#include <vector>
#include <memory>

class JsonSerde;

enum PropertyType
{
    STRING,
    BOOL,
    NUMBER,
    DOUBLE,
    SERDE,
    VECTOR
};

class Type
{
public:
    PropertyType type;
    std::unique_ptr<Type> sub_type{};

    Type() = default;
    Type(const Type &other)
    {
        this->operator=(other);
    }
    Type &operator=(const Type &other)
    {
        type = other.type;
        if (type == PropertyType::VECTOR)
            sub_type = std::make_unique<Type>(*(other.sub_type));
        return *this;
    }

    Type(const std::string *)
    {
        type = PropertyType::STRING;
    }
    Type(const bool *)
    {
        type = PropertyType::BOOL;
    }
    Type(const int *)
    {
        type = PropertyType::NUMBER;
    }
    Type(const double *)
    {
        type = PropertyType::DOUBLE;
    }
    Type(const JsonSerde *)
    {
        type = PropertyType::SERDE;
    }

    template <class ElementType>
    Type(std::vector<ElementType> *vec)
    {
        type = PropertyType::VECTOR;
        sub_type = std::make_unique<Type>((ElementType *)nullptr);
    }
};