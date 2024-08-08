#ifndef __FUNCOES_BACK__
#define __FUNCOES_BACK__

#include <iostream>
#include <cstdlib>
#include <fstream>
#include <string>
#include <unistd.h>
#include <thread>
#include <algorithm>
#include <sstream>
//#include "nthread.hpp"

bool command(std::string cmd);

void acess_dir(std::string dir);

std::string get_path(void);

std::string create_and_acess_dir(std::string);

bool duplicated_file_check (std::string list, std::string add_last);

bool extension_file_check (std::string add_last);

bool extension_file_check_entry (std::string add_last);

std::string get_file_name_from_path(std::string path);

std::string get_exec_name_from_name(std::string name);

std::string initiation(void);

bool compile_code(std::string code_name, std::string exec_name);

bool execute_code (std::string code_name, std::string entry_path, std::string out_name);

void profile(std::string exec_name, std::string profile_exec_name, std::string dot_name, std::string image_name, std::string back_path, bool all_func, bool brief);

void auto_exec_entries(std::string exec_name, std::string paths);

void auto_exec_codes(std::string return_path);

std::string main_time(std::string exec_name, std::string arquivo);

#endif