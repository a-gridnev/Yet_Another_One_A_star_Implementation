template<class T>
Graph<T>::Graph(int size): _size(size)
{
    // init Graph
    for(int i=0; i<_size; ++i)
    {
        std::vector<T> row;

        for(int j=0; j<_size; ++j)
        {
            row.push_back(T(i,j));
        }

        _data.push_back(row);
    }

    _start = &_data[1][1];
    _goal  = &_data[_size-2][_size-2];

    // init Neighbors
    using namespace std::placeholders;
    _neighbors.setInBounds(std::bind(&Graph<T>::inBounds, this, _1, _2));
    _neighbors.setGetNodeAdress(std::bind(&Graph<T>::getNodeAdress, this, _1, _2));
}


template<class T>
int Graph<T>::size() const
{
    return(_size);
}

template<class T>
std::vector<std::vector<T>>& Graph<T>::data()
{
    return(_data);
}


template<class T>
T*& Graph<T>::start()
{
    return(_start);
}

template<class T>
T*& Graph<T>::goal()
{
    return(_goal);
}


template<class T>
Wall<T>& Graph<T>::wall()
{
    return(_wall);
}

template<class T>
Neighbors<T>& Graph<T>::neighbors()
{
    return(_neighbors);
}

template<class T>
bool Graph<T>::inBounds(int x, int y) const
{
    return(x >= 0 && x < _size &&
           y >= 0 && y < _size);
}

template<class T>
T* Graph<T>::getNodeAdress(int x, int y)
{
    return(&_data[x][y]);
}


template<class T>
double Graph<T>::cost(T* left, T* right)
{
    if(_neighbors.onDiagonal(left, right))
    {
        // if separated by the wall
        if(_wall.contain(getNodeAdress( left->x, right->y)) &&
           _wall.contain(getNodeAdress(right->x,  left->y)))
        {
            return(std::numeric_limits<double>::max());
        }
        else
        {
            return(std::sqrt(2));            
        }
    }

    else
    {
        return(1);
    }
}


template<class T>
bool Graph<T>::canBeMoved(T* node) const
{
    return(node == _start || node == _goal);
}

template<class T>
bool Graph<T>::canBePlaced(T* node) const
{
    return(node != _start && node != _goal && !_wall.contain(node));
}

template<class T>
bool Graph<T>::canBeWalled(T* node) const
{
    return(!canBeMoved(node));
}