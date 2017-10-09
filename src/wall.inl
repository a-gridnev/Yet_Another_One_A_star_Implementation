template<class T>
void Wall<T>::insert(T* node)
{
    _data.insert(node);
}

template<class T>
void Wall<T>::erase(T* node)
{
    _data.erase(node);
}

template<class T>
void Wall<T>::clear()
{
    return(_data.clear());
}

template<class T>
bool Wall<T>::contain(T* node) const
{
    return(_data.find(node) != _data.end());
}

template<class T>
const std::unordered_set<T*>& Wall<T>::get() const
{
    return(_data);
}