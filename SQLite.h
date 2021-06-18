#pragma once

#include <string>

#include "Handle.h"
#include "sqlite3.h"

#ifdef _DEBUG
#define VERIFY ASSERT
#define VERIFY_(result, expression) ASSERT(result == expression)
#else
#define VERIFY(expression) (expression)
#define VERIFY_(result, expression) (expression)
#endif // _DEBUG

struct Exception
{
	int Result = 0;
	std::string Message;

	explicit Exception(sqlite3* const connection) : Result(sqlite3_extended_errcode(connection)), Message(sqlite3_errmsg(connection))
	{}
};

class Connection
{
	struct ConnectionHandleTraits : HandleTraits<sqlite3*>
	{
		static void Close(Type value) noexcept
		{
			VERIFY_(SQLITE_OK, sqlite3_close(value));
		}
	};

	using ConnectionHandle = Handle<ConnectionHandleTraits>;
	ConnectionHandle m_handle;

	template <typename F, typename C>
	void InternalOpen(F open, C const* const filename)
	{
		Connection temp;

		if (SQLITE_OK != open(filename, temp.m_handle.Set()))
		{
			temp.ThrowLastError();
		}

		swap(m_handle, temp.m_handle);
	}

public:
	Connection() noexcept = default;

	template <typename C>
	explicit Connection(C const* const filename)
	{
		Open(filename);
	}

	static Connection Memory()
	{
		return Connection(":memory:");
	}

	static Connection WideMemory()
	{
		return Connection(L":memory:");
	}

	explicit operator bool() const noexcept
	{
		return static_cast<bool>(m_handle);
	}

	sqlite3* GetAbi() const noexcept
	{
		return m_handle.Get();
	}

	__declspec(noreturn) void ThrowLastError() const
	{
		// ABI - application binary interface - term used to refer to the underlying binary api for some component
		throw Exception(GetAbi());
	}

	void Open(char const* const filename)
	{
		InternalOpen(sqlite3_open, filename);
	}

	// For wide characters
	void Open(wchar_t const* const filename)
	{
		InternalOpen(sqlite3_open16, filename);
	}
};