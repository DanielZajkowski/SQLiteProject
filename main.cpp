#include <stdio.h>
#include "SQLite.h"

static char const* TypeName(Type const type)
{
	switch (type)
	{
	case Type::Integer: return "Integer";
	case Type::Float: return "Float";
	case Type::Blob: return "Blob";
	case Type::Null: return "Null";
	case Type::Text: return "Text";
	}

	ASSERT(false);
	return "Invalid";
}

static void SaveToDisk(Connection const& source, char const* const filename)
{
	Connection destination(filename);
	Backup backup(destination, source);
	backup.Step();
}

int main()
{
	try
	{
		Connection connection = Connection::Memory();

		Statement statement(connection, "INSERT INTO Things values (?)");

		for (int i = 0; i != 100000; ++i)
		{
			statement.Reset(i);
			statement.Execute();
		}

		Execute(connection, "DELETE FROM Things WHERE Content > 10");
		Execute(connection, "vacuum");

		SaveToDisk(connection, "path");
	}
	catch (Exception const& e)
	{
		printf("%s (%d)\n", e.Message.c_str(), e.Result);
	}
}