#include "nthread.hpp"9
#include <iostream>

using namespace std;

void control::watcher_stop(void)
{
    pthread_cancel(this->hold_watcher);
}

void control::watched_stop(void)
{
    pthread_cancel(this->hold_watched);
}

void control::process(string &exec_name, string &entry_path_name, string &entry_name, string &back_path)
{ 
    string entry_pure_name = get_exec_name_from_name(entry_name);
    pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL);
    bool sucess = execute_code(exec_name, entry_path_name, exec_name + entry_name);
    this->watcher_stop();
    this->free_thread = true;
    if(sucess){
        profile(exec_name, exec_name + entry_pure_name + "_p.txt", exec_name + entry_pure_name + "_p.dot", exec_name + entry_pure_name + ".png", back_path, true, true);
    }
    std::cout << "Feito\n";
    return;
}

void control::watcher_process(int &time)
{
    pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL);
    std::this_thread::sleep_for(std::chrono::milliseconds(time));
    this->killed = true;
    this->watched_stop();
    this->free_thread = true;
    std::cout << "Morto\n";
    return;
}

bool control::execute(string &exec_name, string &entry_path_name, string &entry_name, string &back_path, int &time)
{
    string exec_and_entry = exec_name + get_exec_name_from_name(entry_name);
    command("rm -rf " + exec_and_entry + ".txt " + exec_and_entry + "_p.txt " + exec_and_entry + "_p.dot " + exec_and_entry + ".png");
    this->free_thread = false;
    thread p1 = thread(&control::process, std::ref(*this), std::ref(exec_name), std::ref(entry_path_name), std::ref(entry_name), std::ref(back_path));
    thread p2 = thread(&control::watcher_process, std::ref(*this), std::ref(time));
    this->hold_watched = p1.native_handle();
    this->hold_watcher = p2.native_handle();
    p1.detach();
    p2.detach();
    return this->killed;
}

bool control::check_if_avalaible(void){
    return this->free_thread;
}

automatic_execution::automatic_execution(Codes &list_of_codes, int &time){
    initiation();
    vector<control> avalaible_threads(std::thread::hardware_concurrency() - 1);
    list_of_codes.executables.restart();

    for (int codes_walk = 0; codes_walk < list_of_codes.size(); )
    {
        string back_path = create_and_acess_dir(list_of_codes.executables.current_node->value.executable_name + "_exits");
        compile_code(list_of_codes.executables.current_node->value.algorithm_path, list_of_codes.executables.current_node->value.executable_name);
        list_of_codes.executables.current_node->value.entries_path.restart();
        list_of_codes.executables.current_node->value.entries_name.restart();
        for (int entries_walk = 0; entries_walk < list_of_codes.executables.current_node->value.entries_path.size();)
        {
            for (int threads_walk = 0; threads_walk < avalaible_threads.size(); threads_walk++)
            {
                if (avalaible_threads[threads_walk].check_if_avalaible())
                {
                    avalaible_threads[threads_walk].execute(list_of_codes.executables.current_node->value.executable_name, list_of_codes.executables.current_node->value.entries_path.current_node->value, list_of_codes.executables.current_node->value.entries_name.current_node->value, back_path,  time);
                    list_of_codes.executables.current_node->value.entries_path >> 1;
                    list_of_codes.executables.current_node->value.entries_name >> 1;
                    entries_walk++;
                }
            }
            this_thread::sleep_for(chrono::milliseconds(50));

        }
        while (1)
        {
            bool reset = false;
            for (int c = 0; c < avalaible_threads.size(); c++)
            {
                if (avalaible_threads[c].check_if_avalaible() == false)
                {
                    reset = true;
                }
            }
            if (reset == false)
            {
                break;
            }
            this_thread::sleep_for(chrono::milliseconds(100));
        }
        acess_dir("..");
        codes_walk++;
        list_of_codes.executables >> 1;
    }


    while(1){
        bool reset = false;
        for(int c = 0; c < avalaible_threads.size(); c++){
            if(avalaible_threads[c].check_if_avalaible() == false){
                reset = true;
            }
        }
        if(reset == false){
            break;
        }
        this_thread::sleep_for(chrono::milliseconds(100));
    }
    acess_dir("..");
}

automatic_execution::automatic_execution(Algorithm &list, int &time){ // Um código, várias entradas
    initiation();
    string back_path = create_and_acess_dir(list.executable_name + "_exits");
    vector<control> avalaible_threads(std::thread::hardware_concurrency() - 1);
    list.entries_path.restart();
    list.entries_name.restart();
    compile_code(list.algorithm_path, list.executable_name);
    for(int c = 0; c < list.entries_path.size(); ){
        for(int d = 0; d < avalaible_threads.size(); d++){
           if(avalaible_threads[d].check_if_avalaible() == true){
                avalaible_threads[d].execute(list.executable_name, list.entries_path.current_node->value, list.entries_name.current_node->value, back_path, time);
                c++;
                list.entries_path >> 1;
                list.entries_name >> 1;
           } 
        }
           this_thread::sleep_for(chrono::milliseconds(50));
    }

    while(1){
        bool reset = false;
        for(int c = 0; c < avalaible_threads.size(); c++){
            if(avalaible_threads[c].check_if_avalaible() == false){
                reset = true;
            }
        }
        if(reset == false){
            break;
        }
        this_thread::sleep_for(chrono::milliseconds(50));
    }
    acess_dir("../..");
}

automatic_execution::automatic_execution(Algorithm &single_code, string &entry_path, int &time){ // Um código, uma entrada
    initiation();
    string bla = get_file_name_from_path(entry_path);
    string back_path = create_and_acess_dir(single_code.executable_name + "_exits");
    compile_code(single_code.algorithm_path, single_code.executable_name);
    control one;
    one.execute(single_code.executable_name, entry_path, bla, back_path, time);
    while(one.free_thread == false){
        this_thread::sleep_for(chrono::milliseconds(50));
    }
    acess_dir("../..");
}
