#ifndef SQL_HPP
#define SQL_HPP

#include <iostream>
#include <string>
#include <sqlite3.h>
#include <filesystem>

// SQL Code

bool SQL_create_char(sqlite3 *db);
bool SQL_insert_char(sqlite3 *db);
bool SQL_select_char(sqlite3 *db);
bool SQL_update_char(sqlite3 *db);
bool SQL_delete_char(sqlite3 *db);

// sqlite3

std::filesystem::path db_setup(bool delete_file = true);

int db_check(sqlite3 *db, const std::filesystem::path db_file, int result_code);

bool sqlite_result_ok_basic(std::string descriptor, int return_code, char *err_msg);
bool sqlite_result_ok(sqlite3 *db, std::string descriptor, int return_code);
bool sqlite_result_done(sqlite3 *db, std::string descriptor, int return_code);

#endif