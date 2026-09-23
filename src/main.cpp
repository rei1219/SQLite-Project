// Library Headers

#include <iostream>
#include <sqlite3.h>
#include <format>
#include <filesystem>
#include <ncurses.h>

// Project Headers

#include "sql.hpp"

// Macros and Declarations and stuff



int main()
{

	//	Initialize database file and connects to it
	std::filesystem::path db_file = db_setup();
	sqlite3 *db = nullptr;
	int return_code = sqlite3_open(db_file.string().c_str(), &db);

	if (db_check(db, db_file, return_code) == 1) { return 1; }

	//	Adds basic SQL Table "Characters"
	std::string SQL_create_char =
		"CREATE TABLE IF NOT EXISTS Characters("
		"id INTEGER PRIMARY KEY AUTOINCREMENT, "
		"name TEXT NOT NULL, "
		"software TEXT NOT NULL);";

	char *err_msg = nullptr;
	return_code = sqlite3_exec
		(
			db,
			SQL_create_char.c_str(),
			nullptr,
			nullptr,
			&err_msg
		);

	sql_result_ok("sqlite3_exec", return_code, err_msg);

	//	Insert SQL statement for Table "Characters"
	sqlite3_stmt *stmt_insert_char;
	std::string insert_char =
		"INSERT INTO Characters (name, software)"
		"VALUES (?, ?);";
	return_code = sqlite3_prepare_v2(db, insert_char.c_str(), -1, &stmt_insert_char, nullptr);
	if ( return_code != SQLITE_OK )
		{
			std::cerr << std::format("ERR_[sqlite3_prepare_v2]: {}\n", sqlite3_errmsg(db));
			return 1;
		}

	// Inserts SQL values
	sqlite3_bind_text(stmt_insert_char, 1, "Adachi Rei", -1, SQLITE_STATIC);
	sqlite3_bind_text(stmt_insert_char, 2, "UTAU",		 -1, SQLITE_STATIC);

	// Moves to next row
	return_code = sqlite3_step(stmt_insert_char);
	if ( return_code != SQLITE_DONE )
		{
			std::cerr << std::format("ERR_[sqlite3_step]: {}\n", sqlite3_errmsg(db));
		}

	// Finalizes statement, important otherwise risks memory leaks
	sqlite3_finalize(stmt_insert_char);

	std::string select_char =
		"SELECT *"
		"FROM Characters;";
	sqlite3_stmt *stmt_select_char;

	return_code = sqlite3_prepare_v2(db, select_char.c_str(), -1, &stmt_select_char, nullptr);
	if ( return_code != SQLITE_OK )
		{
			std::cout << std::format("ERR_[sqlite3_prepare_v2]: {}\n", sqlite3_errmsg(db));
			return 1;
		}

	while ( ( return_code = sqlite3_step(stmt_select_char)) == SQLITE_ROW )
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

	if ( return_code != SQLITE_DONE )
		{
			std::cerr << std::format("ERR_[sqlite3_row]: {}\n", sqlite3_errmsg(db));
		}

	sqlite3_finalize(stmt_select_char);

	//	Closes database connection (shocker)
	sqlite3_close(db);
	return 0;

}

// Cool Functions

