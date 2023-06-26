#ifndef STACK_H
#define STACK_H

#include <cstddef>
#include <iostream>

constexpr size_t MAX_STACK_SIZE{32};

template <typename T>
class Stack
{
private:
    T _data[MAX_STACK_SIZE];
    size_t capacity;

public:
    Stack() : capacity(0) {}
    ~Stack() = default;

    Stack &push(const T &);
    T pop(size_t);
    Stack &mod(size_t, const T &);
    const T &peek() const { return capacity > 0 ? _data[capacity - 1] : _data[0]; };

    void print() const;

    const T &operator[](size_t index) const { return index < capacity ? _data[index] : _data[0]; }
    bool operator==(const Stack &) const;
    bool operator!=(const Stack &src) const { return !(*this == src); }
};

template <typename T>
Stack<T> &Stack<T>::push(const T &value)
{
    if (capacity < MAX_STACK_SIZE)
    {
        _data[capacity++] = value;
    }

    return *this;
}

template <typename T>
Stack<T> &Stack<T>::mod(size_t index, const T &value)
{
    if (index < capacity)
    {
        _data[index] = value;
    }

    return *this;
}

template <typename T>
T Stack<T>::pop(size_t index)
{
    if (!(index < capacity) || capacity == 0)
    {
        return _data[0];
    }

    T temp{_data[index]};
    capacity--;
    for (size_t i = index; i < capacity; i++)
    {
        _data[i] = _data[i + 1];
    }

    return temp;
}

template <typename T>
void Stack<T>::print() const
{
    for (size_t i = 0; i < capacity; i++)
    {
        std::cout << _data[i] << ' ';
    }
    std::cout << '\n';
}

template <typename T>
bool Stack<T>::operator==(const Stack<T> &src) const
{
    if (capacity != src.capacity)
    {
        return false;
    }

    for (size_t i = 0; i < capacity; i++)
    {
        if (_data[i] != src._data[i])
        {
            return false;
        }
    }

    return true;
}

#endif // STACK_H