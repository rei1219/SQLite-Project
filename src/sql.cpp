#include <iostream>
#include <string>
#include <sqlite3.h>
#include <filesystem>
#include <format>

#include "sql.hpp"

std::filesystem::path db_setup()
{
	const std::filesystem::path db_dir = "db";
	const std::filesystem::path db_file = db_dir / "test.db";

	if (!std::filesystem::exists(db_dir))
	{
		std::filesystem::create_directory(db_dir);
		std::cout << std::format("Created directory: {}/\n", db_dir.string());
	}

	if (std::filesystem::remove(db_file))
		{ std::cout << std::format("Removed file: {}\n", db_file.string()); }

	return db_file;
}

int db_check(sqlite3 *db, const std::filesystem::path db_file, int return_code)
{
	if ( return_code != SQLITE_OK )
		{
			std::cerr << std::format
			(
				"Connection to {} failure: {}\n",
				db_file.string(), sqlite3_errmsg(db)
			);

			sqlite3_close(db);
			return 1;
		}

	else { std::cout << std::format
			(
				"Connection to {} successful\n",
				db_file.string()
			); }

	return 0;
}

void sql_result_ok(std::string descriptor, int return_code, char *err_msg)
{

	if ( return_code != SQLITE_OK )
		{
			std::cerr << std::format
			(
				"ERR_[{}]: {}\n",
				descriptor,
				err_msg
			);
				sqlite3_free(err_msg);
		}
}