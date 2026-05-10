#pragma once
#include<iostream>

using namespace std;

template <typename T>
class Storage
{
private:
    T* data;
    int count;

public:
    Storage();// CONSTRUCTOR AND DESTRUCTOR
    ~Storage();

    void add(T item);// ADD AND REMOVE LOGIC
    void removeByID(int id);

    T* findByID(int id);
    T* getAll();

    Storage(const Storage& other);
    Storage& operator=(const Storage& other);

    int size();
};

template <typename T>
Storage<T>::Storage()
{
    data = new T[100];
    count = 0;
}

template<typename T>
Storage<T>::~Storage()
{
    delete[] data;// deallocation
}

template <typename T>
void Storage<T>::add(T item)
{
    if (count == 100)
    {
        cout << "Storage is full!" << endl;
    }
    else
    {
        data[count] = item;
        count++;
    }
}

template <typename T>
T* Storage<T>::findByID(int id)
{
    for (int i = 0; i < count; i++)
    {
        if (data[i].getID() == id)
        {
            return &data[i];  // returns pointer to the actual object
        }
    }
    return nullptr;  // not found
}

template <typename T>
void Storage<T>::removeByID(int id)
{
    bool foundID = false;
    int index;

    // first find which index has this ID
    for (int i = 0; i < count; i++)
    {
        if (data[i].getID() == id)
        {
            index = i;
            foundID = true;
            break;
        }
    }

    if (foundID == false)
    {
        cout << "Item not found!" << endl;
        return;// function will end here
    }

    for (int i = index; i < count - 1; i++)
    {
        data[i] = data[i + 1];// data shifts leftwards
    }

    count--;  // one less item now
}

template <typename T>
T* Storage<T>::getAll()
{
    return data;
}

template <typename T>
int Storage<T>::size()
{
    return count;
}

template <typename T>
Storage<T>::Storage(const Storage& other)
{
    data = new T[100];
    count = other.count;
    for (int i = 0; i < count; i++)
        data[i] = other.data[i];
}

template <typename T>
Storage<T>& Storage<T>::operator=(const Storage& other)
{
    if (this != &other)
    {
        delete[] data;
        data = new T[100];
        count = other.count;
        for (int i = 0; i < count; i++)
            data[i] = other.data[i];
    }
    return *this;
}