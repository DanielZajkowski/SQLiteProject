#include <stdio.h>
#include "SQLite.h"

int main()
{
	try
	{
		Connection connection = Connection::Memory();
	}
	catch (Exception const& e)
	{
		printf("%s (%d)\n", e.Message.c_str(), e.Result);
	}
}