#include <iostream>
#include <sqlite3.h>
#include <format>
#include <filesystem>

// Macros

int main()
{

	//	Defines directory and file name
	//	Checks if directory exists, creates directory if non existant
	const std::filesystem::path db_dir = "db";
	const std::filesystem::path db_file = db_dir / "test.db";

	if (!std::filesystem::exists(db_dir))
	{
		std::filesystem::create_directory(db_dir);
		std::cout << std::format("Created directory: {}/\n", db_dir.string());
	}

	if (std::filesystem::remove(db_file))
		{ std::cout << std::format("Removed file: {}\n", db_file.string().c_str()); }

	//	Initialize database file and connects to it
	sqlite3 *db;
	int rc = sqlite3_open(db_file.string().c_str(), &db);

	if ( rc != SQLITE_OK )
		{
			std::cerr << std::format("Failure opening database: {}\n", sqlite3_errmsg(db));
			sqlite3_close(db);
			return 1;
		}
	else { std::cout << "Success opening database\n"; }

	//	Adds basic SQL Table "Characters"
	std::string create_char =
		"CREATE TABLE IF NOT EXISTS Characters("
		"id INTEGER PRIMARY KEY AUTOINCREMENT, "
		"name TEXT NOT NULL, "
		"software TEXT NOT NULL);";

	char *err_msg = nullptr;
	rc = sqlite3_exec(db, create_char.c_str(), nullptr, nullptr, &err_msg);
	if ( rc != SQLITE_OK )
		{
			std::cerr << std::format("ERR_[sqlite3_exec]: {}\n", err_msg);
			sqlite3_free(err_msg);
		}

	//	Insert SQL statement for Table "Characters"
	sqlite3_stmt *stmt_insert_char;
	std::string insert_char =
		"INSERT INTO Characters (name, software)"
		"VALUES (?, ?);";
	rc = sqlite3_prepare_v2(db, insert_char.c_str(), -1, &stmt_insert_char, nullptr);
	if ( rc != SQLITE_OK )
		{
			std::cerr << std::format("ERR_[sqlite3_prepare_v2]: {}\n", sqlite3_errmsg(db));
			return 1;
		}

	// Inserts SQL values
	sqlite3_bind_text(stmt_insert_char, 1, "Adachi Rei", -1, SQLITE_STATIC);
	sqlite3_bind_text(stmt_insert_char, 2, "UTAU",		 -1, SQLITE_STATIC);

	// Moves to next row
	rc = sqlite3_step(stmt_insert_char);
	if ( rc != SQLITE_DONE )
		{
			std::cerr << std::format("ERR_[sqlite3_step]: {}\n", sqlite3_errmsg(db));
		}

	// Finalizes statement, important otherwise risks memory leaks
	sqlite3_finalize(stmt_insert_char);

	std::string select_char =
		"SELECT *"
		"FROM Characters;";
	sqlite3_stmt *stmt_select_char;

	rc = sqlite3_prepare_v2(db, select_char.c_str(), -1, &stmt_select_char, nullptr);
	if ( rc != SQLITE_OK )
		{
			std::cout << std::format("ERR_[sqlite3_prepare_v2]: {}\n", sqlite3_errmsg(db));
			return 1;
		}

	while ( ( rc = sqlite3_step(stmt_select_char)) == SQLITE_ROW )
		{
			int id = sqlite3_column_int(stmt_select_char, 0);
			std::string name = reinterpret_cast<const char*>(sqlite3_column_text(stmt_select_char, 1));
			std::string software = reinterpret_cast<const char*>(sqlite3_column_text(stmt_select_char, 2)); 

			std::cout << std::format
			(
				"ID: {}, Name: {}, Software: {}",
				id, name, software
			);
		}

	if ( rc != SQLITE_DONE )
		{
			std::cerr << std::format("ERR_[sqlite3_row]: {}\n", sqlite3_errmsg(db));
		}

	sqlite3_finalize(stmt_select_char);

	//	Closes database connection (shocker)
	sqlite3_close(db);
	return 0;

}

// Cool Functions
