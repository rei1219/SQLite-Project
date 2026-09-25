#include <iostream>
#include <string>
#include <sqlite3.h>
#include <filesystem>
#include <format>

#include "sql.hpp"

// SQL Code

bool SQL_create_char(sqlite3 *db)
{
	std::string sql =
		"CREATE TABLE IF NOT EXISTS Characters("
		"id INTEGER PRIMARY KEY AUTOINCREMENT, "
		"name TEXT NOT NULL, "
		"software TEXT NOT NULL);";

	char *err_msg = nullptr;
	int return_code = sqlite3_exec
		(
			db,
			sql.c_str(),
			nullptr,
			nullptr,
			&err_msg
		);

	if ( sqlite_result_ok_basic("sqlite3_exec", return_code, err_msg) )
		{ return 1; }

	return 0;
}

bool SQL_insert_char(sqlite3 *db)
{
	sqlite3_stmt *stmt = nullptr;

	std::string sql =
		"INSERT INTO Characters (name, software)"
		"VALUES (?, ?);";

	char *err_msg = nullptr;
	int return_code = sqlite3_prepare_v2
		(
			db,
			sql.c_str(),
			-1,
			&stmt,
			nullptr
		);

	if ( sqlite_result_ok(db, "sqlite3_prepare_v2", return_code) )
		{ return 1; }

	sqlite3_bind_text(stmt, 1, "Adachi Rei", -1, SQLITE_STATIC);
	sqlite3_bind_text(stmt, 2, "UTAU",		 -1, SQLITE_STATIC);

	return_code = sqlite3_step(stmt);

	if ( sqlite_result_done(db, "sqlite3_step", return_code) )
		{ return 1; }

	sqlite3_finalize(stmt);

	return 0;
}

// sqlite3

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

bool sqlite_result_ok_basic(std::string descriptor, int return_code, char *err_msg)
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

			return 1;
		}

	return 0;
}

bool sqlite_result_ok(sqlite3 *db, std::string descriptor, int return_code)
{
	if ( return_code != SQLITE_OK )
		{
			std::cerr << std::format
			(
				"ERR_[{}]: {}\n",
				descriptor,
				std::string(sqlite3_errmsg(db))
			);

			return 1;
		}

	return 0;
}

bool sqlite_result_done(sqlite3 *db, std::string descriptor, int return_code)
{
    if ( return_code != SQLITE_DONE )
        {
            std::cerr << std::format
            (
                "ERR_[{}]: {}\n",
                descriptor,
                std::string(sqlite3_errmsg(db))
            );

			return 1;
        }

		return 0;
}
