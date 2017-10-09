#pragma once


#include "neighbors.hpp"
#include "wall.hpp"

#include <cmath>
#include <functional>
#include <limits>
#include <vector>


template<class T>
class Graph
{
private:
    int _size;
    std::vector<std::vector<T>> _data;    

    T* _start;
    T* _goal;

    Wall<T>      _wall;
    Neighbors<T> _neighbors;

public:
    Graph(int size);


    int size() const;
    std::vector<std::vector<T>>& data();

    T*& start();
    T*& goal();

    Wall<T>& wall();
    Neighbors<T>& neighbors();

    bool inBounds(int, int) const;

    T* getNodeAdress(int, int);

    double cost(T*, T*);


    bool canBeMoved(T*) const;
    bool canBePlaced(T*) const;

    bool canBeWalled(T*) const;
};


#include "graph.inl"