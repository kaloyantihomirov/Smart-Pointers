#include <iostream>

#include "SharedPtr.hpp"

template <typename T>
class WeakPtr
{
	T* data;
	Counter* counter;

	void free();
	void copyFrom(const WeakPtr& other);
	void moveFrom(WeakPtr&& dying) noexcept;
public:
	WeakPtr();

	WeakPtr(SharedPtr<T>& sp);

	WeakPtr(const WeakPtr& other);
	WeakPtr& operator=(const WeakPtr& other);

	WeakPtr(WeakPtr&& dying) noexcept;
	WeakPtr& operator=(WeakPtr&& dying) noexcept;

	bool expired() const;

	SharedPtr<T> lock() const;

	~WeakPtr();

};

template<typename T>
void WeakPtr<T>::free()
{
	if (!data && !counter)
	{
		return;
	}

	counter->removeWeakPtr();

	if (counter->weakCount == 0)
	{
		delete counter;
	}
}

template<typename T>
void WeakPtr<T>::copyFrom(const WeakPtr& other)
{
	data = other.data;
	counter = other.counter;

	if (counter)
	{
		counter->addWeakPtr();
	}
}

template<typename T>
void WeakPtr<T>::moveFrom(WeakPtr&& dying) noexcept
{
	data = dying.data;
	dying.data = nullptr;

	counter = dying.counter;
	dying.counter = nullptr;
}

template<typename T>
WeakPtr<T>::WeakPtr() : data(nullptr), counter(nullptr)
{
}

template<typename T>
WeakPtr<T>::WeakPtr(SharedPtr<T>& sp)
{
	data = sp.data;
	counter = sp.counter;

	if (counter)
	{
		counter->addWeakPtr();
	}

}

template<typename T>
WeakPtr<T>::WeakPtr(const WeakPtr& other)
{
	copyFrom(other);
}

template<typename T>
WeakPtr<T>& WeakPtr<T>::operator=(const WeakPtr& other)
{
	if (this != &other)
	{
		free();
		copyFrom(other);
	}

	return *this;
}

template<typename T>
WeakPtr<T>::WeakPtr(WeakPtr&& dying) noexcept
{
	moveFrom(std::move(dying));
}

template<typename T>
WeakPtr<T>& WeakPtr<T>::operator=(WeakPtr&& dying) noexcept
{
	if (this != &dying)
	{
		free();
		moveFrom(std::move(dying));
	}

	return *this;
}

template<typename T>
bool WeakPtr<T>::expired() const
{
	return counter && counter->useCount == 0;
}

template<typename T>
SharedPtr<T> WeakPtr<T>::lock() const
{
	if (expired())
	{
		return SharedPtr<T>();
	}
	else
	{
		return SharedPtr<T>(data);
	}
}

template<typename T>
WeakPtr<T>::~WeakPtr()
{
	free();
}






