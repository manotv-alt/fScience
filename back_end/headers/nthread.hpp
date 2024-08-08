#ifndef __NTHREAD__

#define __NTHREAD__

#include <thread>
#include <pthread.h>
#include "funcoes.hpp"
#include <chrono>
#include <vector>
#include "tad_algoritmo.hpp"

using std::thread;
using std::ref;
using std::string;
using std::vector;

struct control{
    bool killed = false;
    bool free_thread = true;
    pthread_t hold_watcher;
    pthread_t hold_watched;
    void watcher_stop(void);
    void watched_stop(void);
    void process(string &exec_name, string &entry_path_name, string &entry_name, string &back_path);
    void watcher_process(int &time);
    bool execute(string &exec_name, string &entry_path_name, string &entry_name, string &back_path, int &time);
    bool check_if_avalaible(void);
};

struct automatic_execution{

    automatic_execution(Codes &list_of_codes, int &time); /*varios codigos, varias entradas*/

    automatic_execution(Algorithm &list, int &time); /* Um codigo, varias entradas*/

    automatic_execution(Algorithm &code, string &entry_path, int &time); /* Um codigo, uma entrada*/  
};

#endif