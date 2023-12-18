#ifndef REGISTRY_IMPL_INCLUDE
#define REGISTRY_IMPL_INCLUDE

#include "Registry.hpp"

#include <type_traits>

/* COMPONENT VECTOR */
template <typename T>
std::shared_ptr<TypeVector<T>> TypeVector<T>::_pInstance;

template <typename T>
std::shared_ptr<TypeVector<T>> TypeVector<T>::getInstance()
{
    if (_pInstance == nullptr)
    {
        _pInstance = std::make_shared<TypeVector<T>>();
    }

    return _pInstance;
}

template <typename T>
std::vector<std::shared_ptr<T>> &TypeVector<T>::getVector()
{
    return _vector;
}

template <typename T>
void TypeVector<T>::push_back(const std::shared_ptr<T> ptr)
{
    _vector.push_back(ptr);
}

template <typename T>
std::shared_ptr<T> TypeVector<T>::back()
{
    return _vector.back();
}

/* COMPONENT QUEUE */
template <typename T>
std::shared_ptr<TypeQueue<T>> TypeQueue<T>::_pInstance;

template <typename T>
std::shared_ptr<TypeQueue<T>> TypeQueue<T>::getInstance()
{
    if (_pInstance == nullptr)
    {
        _pInstance = std::make_shared<TypeQueue<T>>();
    }

    return _pInstance;
}

template <typename T>
std::queue<std::shared_ptr<T>> &TypeQueue<T>::getQueue()
{
    return _queue;
}

template <typename T>
bool TypeQueue<T>::isEmpty()
{
    return _queue.empty();
}

template <typename T>
std::shared_ptr<T> TypeQueue<T>::pop()
{
    auto el = _queue.front();
    _queue.pop();
    return el;
}

template <typename T>
void TypeQueue<T>::push(std::shared_ptr<T> component)
{
    _queue.push(component);
}

/* COMPONENT SET */
template <typename T>
std::shared_ptr<TypeSet<T>> TypeSet<T>::_pInstance;

template <typename T>
std::shared_ptr<TypeSet<T>> TypeSet<T>::getInstance()
{
    if (_pInstance == nullptr)
    {
        _pInstance = std::make_shared<TypeSet<T>>();
    }

    return _pInstance;
}

template <typename T>
std::set<std::shared_ptr<T>> &TypeSet<T>::getSet()
{
    return _set;
}

template <typename T>
void TypeSet<T>::emplace(std::shared_ptr<T> component)
{
    _set.emplace(component);
}

template <typename T>
size_t TypeSet<T>::erase(std::shared_ptr<T> component)
{
    return _set.erase(component);
}

/* TYPE POOL */
template <typename T>
std::shared_ptr<TypePool<T>> TypePool<T>::_pInstance;

template <typename T>
TypePool<T>::TypePool()
{
    expandPool(100);
}

template <typename T>
TypePool<T>::TypePool(int size)
{
    expandPool(size);
}

template <typename T>
std::shared_ptr<TypePool<T>> TypePool<T>::getInstance()
{
    if (_pInstance == nullptr)
    {
        _pInstance = std::make_shared<TypePool<T>>();
    }

    return _pInstance;
}

template <typename T>
std::shared_ptr<T> TypePool<T>::instantiate()
{
    if (!_pool.isEmpty())
    {
        std::shared_ptr<T> component = _pool.pop();
        _alive.emplace(component);
        return component;
    }
    else
    {
        expandPool(10);
        return instantiate();
    }
}

template <typename T>
bool TypePool<T>::release(std::shared_ptr<T> component)
{
    if (_alive.erase(component))
    {
        _pool.push(component);
        return true;
    }
    return false;
}

template <typename T>
void TypePool<T>::expandPool(int amount)
{
    for (int i = 0; i < amount; i++)
        _pool.push(std::make_shared<T>());
}

template <typename T>
std::set<std::shared_ptr<T>> &TypePool<T>::getAlive()
{
    return _alive.getSet();
}

template <typename T>
std::queue<std::shared_ptr<T>> &TypePool<T>::getPooled()
{
    return _pool.getQueue();
}

/* REGISTRY */
template <typename T>
std::shared_ptr<T> Registry::newComponent()
{
    // do not use component creation function if type is not a component
    if (!std::is_base_of<Component, T>::value)
    {
        throw new std::invalid_argument("Registry::newComponent<Type>() : Type must be derived from Component class.");
    }
    
    auto pool = TypePool<T>::getInstance();
    return pool->instantiate();
}

template <typename T>
void Registry::killComponent(std::shared_ptr<T> component)
{
    if(!component)
        return;

    // do not use component kill function if type is not a component
    if (!std::is_base_of<Component, T>::value)
    {
        throw new std::invalid_argument("Registry::killComponent<Type>() : Type must be derived from Component class.");
    }

    std::dynamic_pointer_cast<Component>(component)->kill();
    auto pool = TypePool<T>::getInstance();
    pool->release(component);
}

template <typename T>
std::set<std::shared_ptr<T>> &Registry::getComponents()
{
    return TypePool<T>::getInstance()->getAlive();
}

#endif