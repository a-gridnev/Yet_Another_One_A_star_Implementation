#pragma once


#include <vector>
#include <unordered_map>
#include <utility>

#include <functional>


template<class T>
class Neighbors
{
private:
    std::function<bool (int,int)> _Graph_inBounds;
    std::function<T*(int,int)> _Graph_getNodeAdress;

    std::unordered_map<T*, std::vector<T*>> _data;

    std::vector<std::pair<int,int>> _direction;

public:
    Neighbors();

    void setInBounds(std::function<bool(int,int)>);
    void setGetNodeAdress(std::function<T*(int,int)>);

    void clear();

    void useFourDirectional();
    void useEightDirectional();

    std::vector<T*>& get(T*);

    bool contain(T*) const;

    bool onDiagonal(const T*, const T*);
};


#include "neighbors.inl"