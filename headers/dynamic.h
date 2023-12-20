/************************************************************
*  Author:         Nicholas Tibbetts
*  Date:           11/13/2023 T20:35:04
*  Course Code:    
*  License:        Copyright 2023 Nic Tibbetts
*  Resources used: CS172 final project file
*  Worked with:    _
*  Description:    _
***********************************************************/

#ifndef DARR_H
#define DARR_H

#include <stdexcept>

//Simplified Dynamic class for the hash table
template <typename T>
class Dynamic
{
private:
    T *array;
    unsigned elementSize;
    unsigned capacity;

    void resizeCapacity(unsigned newCapacity)
    {
        T *newArray = new T[newCapacity];
        for (unsigned i = 0; i < (elementSize < newCapacity ? elementSize : newCapacity); ++i)
        {
            newArray[i] = array[i]; 
        }
        delete[] array;
        array = newArray;
        capacity = newCapacity;
    }

public:
    Dynamic() : array(nullptr), elementSize(0), capacity(0) {}

    Dynamic(unsigned size) : array(new T[size]), elementSize(size), capacity(size)
    {
        for (unsigned i = 0; i < size; ++i)
        {
            array[i] = T(); 
        }
    }

    Dynamic(const Dynamic &other) : array(new T[other.capacity]), elementSize(other.elementSize), capacity(other.capacity)
    {
        for (unsigned i = 0; i < elementSize; ++i)
        {
            array[i] = other.array[i]; 
        }
    }

    Dynamic(Dynamic &&other) noexcept : array(other.array), elementSize(other.elementSize), capacity(other.capacity)
    {
        other.array = nullptr;
        other.elementSize = 0;
        other.capacity = 0;
    }

    Dynamic &operator=(Dynamic other)
    {
        swap(*this, other);
        return *this;
    }

    ~Dynamic()
    {
        delete[] array;
    }

    T &operator[](unsigned index)
    {
        if (index >= elementSize)
        {
            throw std::out_of_range("Index out of bounds");
        }
        return array[index];
    }

    const T &operator[](unsigned index) const
    {
        if (index >= elementSize)
        {
            throw std::out_of_range("Index out of bounds");
        }
        return array[index];
    }

    void push(const T &element)
    {
        if (elementSize == capacity)
        {
            resizeCapacity(capacity > 0 ? capacity * 2 : 1);
        }
        array[elementSize++] = element;
    }

    void insert(unsigned index, const T &element)
    {
        if (index > elementSize)
        {
            throw std::out_of_range("Index out of bounds");
        }
        if (elementSize == capacity)
        {
            resizeCapacity(capacity * 2);
        }
        for (unsigned i = elementSize; i > index; --i)
        {
            array[i] = array[i - 1]; 
        }
        array[index] = element;
        ++elementSize;
    }

    // void erase(unsigned index)
    // {
    //     if (index >= elementSize)
    //     {
    //         throw std::out_of_range("Index out of bounds");
    //     }
    //     for (unsigned i = index; i < elementSize - 1; ++i)
    //     {
    //         array[i] = array[i + 1]; 
    //     }
    //     --elementSize;
    // }

    void clear()
    {
        elementSize = 0;
    }

    void reserve(unsigned newCapacity)
    {
        if (newCapacity > capacity)
        {
            resizeCapacity(newCapacity);
        }
    }

    bool empty() const
    {
        return elementSize == 0;
    }

    unsigned size() const
    {
        return elementSize;
    }

    unsigned maxCapacity() const
    {
        return capacity;
    }

    friend void swap(Dynamic &first, Dynamic &second)
    {
        T *tempArray = first.array;
        first.array = second.array;
        second.array = tempArray;

        unsigned tempElementSize = first.elementSize;
        first.elementSize = second.elementSize;
        second.elementSize = tempElementSize;

        unsigned tempCapacity = first.capacity;
        first.capacity = second.capacity;
        second.capacity = tempCapacity;
    }
   
    T *begin()
    {
        return array;
    }

    const T *begin() const
    {
        return array;
    }

    T *end()
    {
        return array + elementSize;
    }

    const T *end() const
    {
        return array + elementSize;
    }
};

#endif
