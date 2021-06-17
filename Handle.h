#pragma once

#if _DEBUG
	#include <crtdbg.h>
	#define ASSERT _ASSERTE
#else
	#define ASSERT __noop
#endif // _DEBUG

template <typename T>
struct HandleTraits
{
	using Type = T;

	static Type Invalid() noexcept
	{
		return nullptr;
	}

	// static void Close(Type value) noexcept
};

template <typename Traits>
class Handle
{
	using Type = decltype(Traits::Invalid());
	Type m_value;

	// Private method for safely and conveniently closing or destroying the resourse using the Traits class
	void Close() noexcept
	{
		if (*this)
		{
			Traits::Close(m_value);
		}
	}

public:
	Handle(Handle const&) = delete;
	Handle& operator=(Handle const&) = delete;

	// Default constructor
	explicit Handle(Type value = Traits::Invalid()) noexcept : m_value(value) {}

	Handle(Handle&& other) noexcept : m_value(other.Detach()) {}

	Handle& operator=(Handle&& other) noexcept
	{
		// Checking for self assingemnt
		if (this != &other)
		{
			Reset(other.Detach());
		}

		return *this;
	}

	~Handle() noexcept
	{
		Close();
	}

	explicit operator bool() const noexcept
	{
		// Used to reliably and simply check the validity off some handle
		return m_value != Traits::Invalid();
	}

	Type Get() const noexcept
	{
		return m_value;
	}

	Type* Set() noexcept
	{
		ASSERT(!*this);
		return &m_value;
	}

	// Detach the ownership of some handle
	Type Detach() noexcept
	{
		Type value = m_value;
		m_value = Traits::Invalid();
		return value;
	}

	bool Reset(Type value = Traits::Invalid()) noexcept
	{
		if (m_value != value)
		{
			Close();
			m_value = value;
		}

		// Optional value to quickly doing a validity check in case of an if statement. This relies on explicit bool operator
		return static_cast<bool>(*this);
	}

	// Provides the most efficient management of handles within standart library containers
	void Swap(Handle<Traits>& other) noexcept
	{
		// To avoid a fairly deep dependency on some standart library headers
		Type temp = m_value;
		m_value = other.m_value;
		other.m_value = temp;
	}
};

template <typename Traits>
void swap(Handle<Traits>& left, Handle<Traits>& right) noexcept
{
	left.Swap(right);
}

template <typename Traits>
bool operator==(Handle<Traits> const& left, Handle<Traits> const& right) noexcept
{
	// Helper methods to compare the underlying handle values
	return left.Get() == right.Get();
}

template <typename Traits>
bool operator!=(Handle<Traits> const& left, Handle<Traits> const& right) noexcept
{
	return !(left == right);
}