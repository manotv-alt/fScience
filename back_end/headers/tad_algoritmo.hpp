#ifndef __TAD__ALGORITHM__
#define __TAD__ALGORITHM__

#include "funcoes.hpp"
#include "list.cpp"

struct Algorithm{
    std::string algorithm_name;
    std::string algorithm_path;
    std::string executable_name;
    List<std::string> entries_name;
    List<std::string> entries_path;

    void new_entries(List<std::string> &source, List<std::string> &source2);

    void new_entries_path(List<std::string> &source);

    void remove_entry_path(void);

    void remove_entry_name(void);

    void remove_entry_name_and_path(void);

    unsigned int size(void);
    
};

struct Codes{
    List<Algorithm> executables;

    void append_exec(std::string name, std::string path);

    unsigned int size(void);

};

void recover_data(Codes &list);

void store_data(Codes &list);

#endif