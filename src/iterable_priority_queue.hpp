#include <queue>


template<
    class T,
    class Container = std::vector<T>,
    class Compare   = std::greater<typename Container::value_type>
>struct iterable_priority_queue: std::priority_queue<T, Container, Compare>
{
    void clear()
    {
        while(!std::priority_queue<T, Container, Compare>::empty())
        {
            std::priority_queue<T, Container, Compare>::pop();
        }
    }

    auto begin() const {return std::priority_queue<T, Container, Compare>::c.begin();}
    auto end()   const {return std::priority_queue<T, Container, Compare>::c.end();}
};