#include <stdio.h>
#include "SQLite.h"

int main()
{
	try
	{
		Connection connection = Connection::Memory();

		Execute(connection, "CREATE TABLE Users (Name)");

		Execute(connection, "INSERT INTO Users values(?)", "Joe");
		Execute(connection, "INSERT INTO Users values(?)", "Beth");

		for (Row row : Statement(connection, "SELECT Name FROM Users"))
		{
			printf("%s\n", row.GetString());
		}
	}
	catch (Exception const& e)
	{
		printf("%s (%d)\n", e.Message.c_str(), e.Result);
	}
}