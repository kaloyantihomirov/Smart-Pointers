#include <iostream>

struct Counter
{
	unsigned useCount = 0;
	unsigned weakCount = 0;

	void addWeakPtr()
	{
		weakCount++;
	}

	void removeWeakPtr()
	{
		weakCount--;
	}

	void addSharedPtr()
	{
		useCount++;

		if (useCount == 1)
		{
			weakCount++;
		}
	}

	void removeSharedPtr()
	{
		useCount--;

		if (useCount == 0)
		{
			weakCount--;
		}
	}
};

template <typename T>
class SharedPtr
{
	T* data;
	Counter* counter;

	void copyFrom(const SharedPtr& other);
	void moveFrom(SharedPtr&& dying) noexcept;
	void free();

public:
	template <typename V> friend class WeakPtr;

	SharedPtr();
	SharedPtr(T* _data);

	SharedPtr(const SharedPtr& other);
	SharedPtr& operator=(const SharedPtr& other);

	SharedPtr(SharedPtr&& dying) noexcept;
	SharedPtr& operator=(SharedPtr&& dying) noexcept;
	
	T& operator*();
	const T& operator*() const;

	T* operator->();
	const T* operator->() const;

	bool isInitialized() const;

	~SharedPtr();
};

template <typename T>
SharedPtr<T>::SharedPtr() : data(nullptr), counter(nullptr)
{
}

template <typename T>
void SharedPtr<T>::copyFrom(const SharedPtr& other)
{
	data = other.data;
	counter = other.counter;

	if (counter)
	{
		counter->addSharedPtr();
	}
}

template <typename T>
SharedPtr<T>::SharedPtr(const SharedPtr& other)
{
	copyFrom(other);
}

template <typename T>
void SharedPtr<T>::free()
{
	if (!data && !counter)
	{
		return;
	}

	counter->removeSharedPtr();

	if (counter->useCount == 0)
	{
		delete data;
	}

	if (counter->weakCount == 0)
	{
		delete counter;
	}
}

template <typename T>
SharedPtr<T>& SharedPtr<T>::operator=(const SharedPtr& other)
{
	if (this != &other)
	{
		free();
		copyFrom(other);
	}

	return *this;
}

template <typename T>
SharedPtr<T>::~SharedPtr()
{
	free();
}

template <typename T>
void SharedPtr<T>::moveFrom(SharedPtr&& dying) noexcept
{
	data = dying.data;
	dying.data = nullptr;

	counter = dying.counter;
	dying.counter = nullptr;
}

template <typename T>
SharedPtr<T>::SharedPtr(SharedPtr&& dying) noexcept
{
	moveFrom(std::move(dying));
}

template <typename T>
SharedPtr<T>& SharedPtr<T>::operator=(SharedPtr&& dying) noexcept
{
	if (this != &dying)
	{
		free();
		moveFrom(std::move(dying));
	}

	return *this;
}

template <typename T>
SharedPtr<T>::SharedPtr(T* _data)
{
	data = _data;
	
	if (data)
	{
		counter = new Counter();
		counter->addSharedPtr();
	}
}

template <typename T>
T& SharedPtr<T>::operator*()
{
	if (!data)
	{
		throw std::runtime_error("null ptr");
	}

	return *data;
}

template <typename T>
const T& SharedPtr<T>::operator*() const
{
	if (!data)
	{
		throw std::runtime_error("null ptr");
	}

	return *data;
}

template <typename T>
T* SharedPtr<T>::operator->()
{
	return data;
}

template <typename T>
const T* SharedPtr<T>::operator->() const
{
	return data;
}

template <typename T>
bool SharedPtr<T>::isInitialized() const
{
	return data != nullptr;
}