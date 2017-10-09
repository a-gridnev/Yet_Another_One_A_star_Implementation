#pragma once


#include <unordered_set>


template<class T>
class Wall
{
private:
    std::unordered_set<T*> _data;

public:
    void insert(T*);
    void erase(T*);

    void clear();

    bool contain(T*) const;

    const std::unordered_set<T*>& get() const;
};


#include "wall.inl"