#ifndef SQL_HPP
#define SQL_HPP

#include <iostream>
#include <string>
#include <sqlite3.h>
#include <filesystem>

std::filesystem::path db_setup();

int db_check(sqlite3 *db, const std::filesystem::path db_file, int result_code);
void sql_result_ok(std::string descriptor, int return_code, char *err_msg);

#endif