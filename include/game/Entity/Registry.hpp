#ifndef REGISTRY_INCLUDE
#define REGISTRY_INCLUDE

#include <vector>
#include <memory>
#include <map>
#include <type_traits>
#include <queue>
#include <set>

#include "../../SDL2/SDL.h"

#include "Components.hpp"

template <typename T>
class TypeVector
{
private:
    static std::shared_ptr<TypeVector<T>> _pInstance; // singleton vector for each type

    std::vector<std::shared_ptr<T>> _vector;

public:
    static std::shared_ptr<TypeVector<T>> getInstance();
    std::vector<std::shared_ptr<T>> &getVector();

    // wrapper functions
    void push_back(const std::shared_ptr<T> ptr);
    std::shared_ptr<T> back();
};

template <typename T>
class TypeQueue
{
private:
    static std::shared_ptr<TypeQueue<T>> _pInstance; // singleton queue for each type

    std::queue<std::shared_ptr<T>> _queue;

public:
    static std::shared_ptr<TypeQueue<T>> getInstance();
    std::queue<std::shared_ptr<T>> &getQueue();

    // wrapper functions
    bool isEmpty();
    std::shared_ptr<T> pop();
    void push(std::shared_ptr<T>);
};

template <typename T>
class TypeSet
{
private:
    static std::shared_ptr<TypeSet<T>> _pInstance; // singleton set for each type

    std::set<std::shared_ptr<T>> _set;

public:
    static std::shared_ptr<TypeSet<T>> getInstance();
    std::set<std::shared_ptr<T>> &getSet();

    // wrapper functions
    void emplace(std::shared_ptr<T> component);
    size_t erase(std::shared_ptr<T> component);
};

template <typename T>
class TypePool
{
private:
    static std::shared_ptr<TypePool<T>> _pInstance;

    TypeQueue<T> _pool;
    TypeSet<T> _alive;

public:
    TypePool();
    TypePool(int size);
    static std::shared_ptr<TypePool> getInstance();
    std::shared_ptr<T> instantiate();
    bool release(std::shared_ptr<T> component);
    void expandPool(int amount);
    std::set<std::shared_ptr<T>> &getAlive();
    std::queue<std::shared_ptr<T>> &getPooled();
};

class Registry
{
public:
    template <typename T>
    std::shared_ptr<T> newComponent(); // create/add a new component to TypeVector<T>

    template <typename T>
    void killComponent(std::shared_ptr<T> component);

    template <typename T>
    std::set<std::shared_ptr<T>> &getComponents();
};

#include "Registry_impl.hpp"

#endif