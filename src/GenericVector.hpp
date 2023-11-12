#pragma once

#include <functional>
#include <vector>

class GenericVector
{
private:
    std::function<size_t()> get_size;
    std::function<void *(int)> get_element;
    std::function<GenericVector(int)> get_generic_vector;
    std::function<void()> clear;
    std::function<void(void *)> add_element;

public:
    bool element_is_vector{};

    template <class ElementClass>
    GenericVector(std::vector<ElementClass> &some_vec)
    {
        element_is_vector = false;
        get_size = [&some_vec]
        { return some_vec.size(); };
        get_element = [&some_vec](int pos)
        { return (void *)&some_vec[pos]; };
        get_generic_vector = [](int) -> GenericVector
        {
            throw;
        };
        clear = [&some_vec]
        { some_vec.clear(); };
        add_element = [&some_vec](void *new_element)
        {
            some_vec.push_back(*((ElementClass *)new_element));
        };
    }

    template <class ElementClass>
    GenericVector(std::vector<std::vector<ElementClass>> &some_vec)
    {
        element_is_vector = true;
        get_size = [&some_vec]
        { return some_vec.size(); };
        get_element = [](int)
        { throw; };
        get_generic_vector = [&some_vec](int pos)
        {
            return GenericVector(some_vec[pos]);
        };
        clear = [&some_vec]
        { some_vec.clear(); };
        add_element = [&some_vec](void *new_element)
        {
            some_vec.push_back(*((ElementClass *)new_element));
        };
    }

    size_t GetSize() const
    {
        return get_size();
    }

    const void *Get(int pos) const
    {
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

    ~GenericVector() = default;
};
