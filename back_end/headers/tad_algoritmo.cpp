#include "tad_algoritmo.hpp"

void Algorithm::new_entries(List<std::string> &source, List<std::string> &source2)
{
    source.restart();
    for (int c = 0; c < source.size(); c++)
    {
        this->entries_name.append(source.current_node->value);
        source >> 1;
    }
    source2.restart();
    for (int c = 0; c < source2.size(); c++)
    {
        this->entries_path.append(source2.current_node->value);
        source2 >> 1;
    }
}

void Algorithm::new_entries_path(List<std::string> &source)
{
    source.restart();
    for (int c = 0; c < source.size(); c++)
    {
        this->entries_path.append(source.current_node->value);
        source >> 1;
    }
}

void Algorithm::remove_entry_path(void)
{
    this->entries_path.remove();
}

void Algorithm::remove_entry_name(void)
{
    this->entries_name.remove();
}

void Algorithm::remove_entry_name_and_path(void)
{
    this->entries_name.remove();
    this->entries_path.remove();
}

unsigned int Algorithm::size(void)
{
    return this->entries_path.size();
}

void Codes::append_exec(std::string name, std::string path)
{
    Algorithm code;
    code.algorithm_name = name;
    code.algorithm_path = path;
    code.executable_name = get_exec_name_from_name(name);

    this->executables.append(code);
}

unsigned int Codes::size(void)
{
    return this->executables.size_of_list;
}

void recover_data(Codes &list)
{

    std::ifstream recover("database.txt");
    std::string data, data2;

    if (recover.peek() == EOF)
    {
        return;
    }

    getline(recover, data);

    for (int algorithms_qtd = atoi(data.c_str()); algorithms_qtd > 0; algorithms_qtd--)
    {
        getline(recover, data);
        getline(recover, data2);
        list.append_exec(data, data2);
        list.executables >> 1;
        getline(recover, data);
        for (int entries_qtd = atoi(data.c_str()); entries_qtd > 0; entries_qtd--)
        {
            getline(recover, data);
            getline(recover, data2);
            list.executables.current_node->value.entries_name.append(data);
            list.executables.current_node->value.entries_path.append(data2);
        }
    }
    recover.close();
}

void store_data(Codes &list)
{

    std::ofstream store("database.txt");
    store << list.size() << "\n";
    list.executables.restart();
    for (int codes = list.size(); codes > 0; codes--)
    {
        store << list.executables.current_node->value.algorithm_name << "\n";
        store << list.executables.current_node->value.algorithm_path << "\n";
        store << list.executables.current_node->value.entries_path.size() << "\n";
        list.executables.current_node->value.entries_name.restart();
        list.executables.current_node->value.entries_path.restart();
        for (int entries = list.executables.current_node->value.entries_path.size(); entries > 0; entries--)
        {
            store << list.executables.current_node->value.entries_name.current_node->value << "\n";
            store << list.executables.current_node->value.entries_path.current_node->value << "\n";
            list.executables.current_node->value.entries_name >> 1;
            list.executables.current_node->value.entries_path >> 1;
        }
        list.executables >> 1;
    }
    store.close();
}
