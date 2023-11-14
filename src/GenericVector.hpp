#pragma once

#include <functional>
#include <vector>

#include "Type.hpp"

class GenericVector
{
private:
    std::function<size_t()> get_size;
    std::function<void *(int)> get_element;
    std::function<GenericVector(int)> get_generic_vector;
    std::function<void()> clear;
    std::function<void(void *)> add_element;
    std::function<void()> add_default_element;
    // For example instance
    std::function<void *()> get_element_instance;
    std::function<void(void *)> delete_element_instance;
    // For generic vector instance
    std::function<GenericVector()> get_generic_vector_instance;

    bool initialized = false;

public:
    Type element_type;

    GenericVector() = default;

    GenericVector &operator=(const GenericVector &other)
    {
        element_type = other.element_type;
        get_size = other.get_size;
        get_generic_vector = other.get_generic_vector;
        get_element = other.get_element;
        get_element_instance = other.get_element_instance;
        delete_element_instance = other.delete_element_instance;
        get_generic_vector_instance = other.get_generic_vector_instance;
        clear = other.clear;
        add_element = other.add_element;
        initialized = other.initialized;
        add_default_element = other.add_default_element;

        return *this;
    }

    template <class ElementClass>
    GenericVector(std::vector<ElementClass> &some_vec)
    {
        element_type = Type{(ElementClass *)nullptr};
        get_size = [&some_vec]
        { return some_vec.size(); };
        get_element = [&some_vec](int pos)
        { return (void *)&some_vec[pos]; };
        get_element_instance = []()
        {
            return (void *)new ElementClass();
        };
        delete_element_instance = [](void *arg)
        {
            delete (ElementClass *)arg;
        };
        get_generic_vector_instance = []() -> GenericVector
        {
            throw std::runtime_error("Cannot ask for GetGenericVector if the element is a not a vector, use GetElementInstance instead");
        };
        get_generic_vector = [](int) -> GenericVector
        {
            throw std::runtime_error("Cannot ask for GetGenericVector if the element is a not a vector, use Get instead");
        };
        clear = [&some_vec]
        { some_vec.clear(); };
        add_element = [&some_vec](void *new_element)
        {
            some_vec.push_back(*((ElementClass *)new_element));
        };
        add_default_element = [&some_vec]()
        {
            some_vec.push_back(ElementClass{});
        };
        initialized = true;
    }

    template <class ElementClass>
    GenericVector(std::vector<std::vector<ElementClass>> &some_vec)
    {
        element_type = Type{(std::vector<ElementClass> *)nullptr};
        get_size = [&some_vec]
        { return some_vec.size(); };

        get_element = [&some_vec](int pos)
        { return (void *)&some_vec[pos]; };
        get_element_instance = []()
        {
            return (void *)new std::vector<ElementClass>();
        };
        delete_element_instance = [](void *arg)
        {
            delete (std::vector<ElementClass> *)arg;
        };
        get_generic_vector_instance = []()
        {
            std::vector<ElementClass> empty_vector{};
            return GenericVector(empty_vector);
        };
        get_generic_vector = [&some_vec](int pos)
        {
            return GenericVector(some_vec[pos]);
        };
        clear = [&some_vec]
        { some_vec.clear(); };
        add_element = [&some_vec](void *new_element)
        {
            some_vec.push_back(*((std::vector<ElementClass> *)new_element));
        };
        add_default_element = [&some_vec]()
        {
            some_vec.push_back(std::vector<ElementClass>{});
        };
        initialized = true;
    }

    size_t GetSize() const
    {
        return get_size();
    }

    const void *Get(int pos) const
    {
        if (!initialized)
            throw std::runtime_error("Generic vector was not initialized");
        return (const void *)get_element(pos);
    }

    GenericVector GetGenericVector(int pos) const
    {
        return get_generic_vector(pos);
    }

    void AddElement(void *element)
    {
        add_element(element);
    }

    void AddElement()
    {
        add_default_element();
    }

    void Clear()
    {
        if (!initialized)
            throw std::runtime_error("Generic vector was not initialized");
        clear();
    }

    void *GetElementInstance() const
    {
        return get_element_instance();
    }

    void DeleteElementInstance(void *arg) const
    {
        delete_element_instance(arg);
    }

    GenericVector GetGenericVectorInstance() const
    {
        return get_generic_vector_instance();
    }

    ~GenericVector() = default;
};
