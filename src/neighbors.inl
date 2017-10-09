template<class T>
Neighbors<T>::Neighbors()
{
    useEightDirectional();
}

template<class T>
void Neighbors<T>::setInBounds(std::function<bool(int,int)> func)
{
    _Graph_inBounds = func;
}

template<class T>
void Neighbors<T>::setGetNodeAdress(std::function<T*(int,int)> func)
{
    _Graph_getNodeAdress = func;
}

template<class T>
void Neighbors<T>::clear()
{
    _data.clear();
}

template<class T>
void Neighbors<T>::useFourDirectional()
{
    _direction = {{{0,-1},{0,1},{1,0},{-1,0}}};
}

template<class T>
void Neighbors<T>::useEightDirectional()
{
    _direction = {{{0,-1},{1,0},{0,1}, {-1,0 },
                   {1,-1},{1,1},{-1,1},{-1,-1}}};
}

template<class T>
std::vector<T*>& Neighbors<T>::get(T* node)
{
    if(!contain(node))
    {
        std::vector<T*> neighbors;

        for(auto& direction: _direction)
        {
            int x = node->x + direction.first;
            int y = node->y + direction.second;

            if(_Graph_inBounds(x,y))
            {
                neighbors.push_back(_Graph_getNodeAdress(x,y));
            }
        }

        _data[node] = neighbors;
    }
        
    return(_data[node]);
}

template<class T>
bool Neighbors<T>::contain(T* node) const
{
    return(_data.find(node) != _data.end());
}

template<class T>
bool Neighbors<T>::onDiagonal(const T* left, const T* right)
{
    return(left->x - right->x != 0 &&
           left->y - right->y != 0);
}