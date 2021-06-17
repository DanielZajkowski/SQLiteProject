#include "sqlite3.h"
#include <stdio.h>
#include "Handle.h"

#ifdef _DEBUG
#define VERIFY ASSERT
#define VERIFY_(result, expression) ASSERT(result == expression)
#else
#define VERIFY(expression) (expression)
#define VERIFY_(result, expression) (expression)
#endif // _DEBUG

struct ConnectionHandleTraits : HandleTraits<sqlite3*>
{
	static void Close(Type value) noexcept
	{
		VERIFY_(SQLITE_OK, sqlite3_close(value));
	}
};

using ConnectionHandle = Handle<ConnectionHandleTraits>;

int main()
{
	//sqlite3* connection = nullptr;
	ConnectionHandle connection;

	int result = sqlite3_open(":memory:", connection.Set());

	if (SQLITE_OK != result)
	{
		printf("%s\n", sqlite3_errmsg(connection.Get()));
		return result;
	}
}