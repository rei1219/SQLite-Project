// Library Headers

#include <iostream>
#include <sqlite3.h>
#include <format>
#include <filesystem>
#include <ncurses.h>

// Project Headers

#include "sql.hpp"
#include "data.cpp"

// Macros and Declarations and stuff



int main()
{

	//	Initialize database file and connects to it
	std::filesystem::path db_file = db_setup();
	sqlite3 *db = nullptr;
	int return_code = sqlite3_open(db_file.string().c_str(), &db);

	if (db_check(db, db_file, return_code) == 1)
		{ return 1; }

	if ( SQL_create_char(db) )
		{ return 1; }
	if ( SQL_insert_char(db) )
		{ return 1; }
	if ( SQL_update_char(db) )
		{ return 1; }
	if ( SQL_delete_char(db) )
		{ return 1; }
	if ( SQL_select_char(db) )
		{ return 1; }

	//	Closes database connection (shocker)
	sqlite3_close(db);
	return 0;

}

// Cool Functions

