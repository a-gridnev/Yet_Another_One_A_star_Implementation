#include <cstdlib>


template<class T>
double manhattan(const T* first, const T* second)
{
    return(std::abs(first->x - second->x) + std::abs(first->y - second->y));
}