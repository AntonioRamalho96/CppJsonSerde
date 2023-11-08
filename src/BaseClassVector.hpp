#pragma once

#include <functional>
#include <vector>

template <class BaseType>
class BaseClassVector
{
public:
    std::function<size_t() const> get_size;
    std::function<char *()> get_head;
    size_t element_size;

    template <class ChildClass>
    BaseClassVector(std::vector<ChildClass> &some_vec)
    {
        get_size = [&some_vec]
        { return some_vec.size(); };
        get_head = [&some_vec]
        { return (char *)some_vec.data(); };
        element_size = sizeof(ChildClass);
    }

    size_t GetSize() const
    {
        return get_size();
    }

    BaseType *Get(int pos)
    {
        return (BaseType *)(get_head() + element_size * pos);
    }
};
