#pragma once


template<class T>
struct Node
{
    Node()        {x = 0; y = 0;}
    Node(T X, T Y){x = X; y = Y;}

    // Member data
    T x;
    T y;
};


template<class T>
bool operator ==(const Node<T>& left, const Node<T>& right)
    {return left.x == right.x &&
            left.y == right.y;}

template<class T>
bool operator !=(const Node<T>& left, const Node<T>& right)
    {return !(left == right);}

// template<class T>
// bool operator >(const Node<T>& left, const Node<T>& right)
//     {return(left.x > right.x && left.y > right.y);}

// template<class T>
// bool operator <(const Node<T>& left, const Node<T>& right)
//     {return(left.x < right.x && left.y < right.y);}

using iNode = Node<int>;