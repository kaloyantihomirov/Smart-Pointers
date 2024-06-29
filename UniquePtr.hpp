#include <iostream>

template <typename T>
class UniquePtr
{
	T* ptr;

public:
	UniquePtr(T* _ptr);

	UniquePtr(const UniquePtr& other) = delete;
	UniquePtr& operator=(const UniquePtr& other) = delete;

	UniquePtr(UniquePtr&& dying) noexcept;
	UniquePtr& operator=(UniquePtr&& dying) noexcept;

	T& operator*();
	const T& operator*() const;

	T* operator->();
	const T* operator->() const;

	~UniquePtr();
};

template<typename T>
UniquePtr<T>::UniquePtr(T* _ptr) : ptr(_ptr)
{
}

template<typename T>
UniquePtr<T>::UniquePtr(UniquePtr&& dying) noexcept
{
	ptr = dying.ptr;
	dying.ptr = nullptr;
}

template<typename T>
UniquePtr<T>& UniquePtr<T>::operator=(UniquePtr&& dying) noexcept
{
	if (this != &other)
	{
		delete ptr;
		ptr = dying.ptr;
		dying.ptr = nullptr;
	}

	return *this;
}

template<typename T>
T& UniquePtr<T>::operator*()
{
	if (!ptr)
	{
		throw std::runtime_error("null ptr");
	}

	return *ptr;
}

template<typename T>
const T& UniquePtr<T>::operator*() const
{
	if (!ptr)
	{
		throw std::runtime_error("null ptr");
	}

	return *ptr;
}

template<typename T>
T* UniquePtr<T>::operator->()
{
	return ptr;
}

template<typename T>
const T* UniquePtr<T>::operator->() const
{
	return ptr;
}




