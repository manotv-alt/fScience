#include "funcoes.hpp"

using namespace std;

bool command(std::string cmd){
    return system(cmd.c_str());
}

void acess_dir(std::string dir){
    chdir(dir.c_str());
}

std::string get_path(void){
    string current_path;
    command("pwd > current_path.txt");
    ifstream path("current_path.txt");
    getline(path, current_path);
    command("rm -r current_path.txt");
    return current_path;
}

std::string create_and_acess_dir(std::string dir){
    command("mkdir " + dir);
    acess_dir(dir);
    return get_path();
}

bool duplicated_file_check (std::string list, std::string add_last) {
    
    for(int i = 0;  i < list.size(); i++) {
        if(list[i] == '\n') {

            int j = i + 1;
            std::string ax = "";

            while(list[j] != '\n') {
                ax = ax + list[j];
                j++;
            }

            if(ax == add_last) {
                return 1;
            }

            i = j - 1;
        }
    }

    return 0;
}

std::string get_file_name_from_path(std::string path) {

    std::string file_name = path, ax;

    for(int i = file_name.size() - 1;  file_name[i] != '/'; i--) {
        ax.push_back(file_name[i]);
    }

    reverse(ax.begin(), ax.end());

    return ax;
}

std::string get_exec_name_from_name(std::string name) {

    std::string file_name = name, ax;

    for(int i = file_name.size() - 1; i >= 0; i--) {
        if(file_name[i] == '.') {
            for(int j = (i-1);  j >= 0; j--) {
                ax.push_back(file_name[j]);
            }
            break;
        }
    }

    reverse(ax.begin(), ax.end());

    return ax;
}

bool extension_file_check (std::string add_last) {
    std::string ax = "";

    for(int i = 0;  i < add_last.size(); i++) {
        if(add_last[i] == '.') {

            int j = i + 1;

            while(j < add_last.size()) {
                ax += add_last[j];
                j++;
            }

            if(ax == "cpp" or ax == "c") {
            return 0;
        }
        }
    }

    return 1;
}

bool extension_file_check_entry (std::string add_last) {
    std::string ax = "";

    for(int i = 0;  i < add_last.size(); i++) {
        if(add_last[i] == '.') {

            int j = i + 1;

            while(j < add_last.size()) {
                ax += add_last[j];
                j++;
            }

            if(ax == "txt") {
            return 0;
        }
        }
    }

    return 1;
}



std::string initiation(void){    
    //command("rm -r exits");
    command("mkdir exits");
    acess_dir("exits");
    return get_path();
}

bool compile_code (std::string code_name, std::string exec_name) {
    int answer;
    string ax = "g++ " + code_name + " -o " + exec_name + " -pg -g";
    answer = command(ax);
    return (answer == 0 ? true : false);
}

bool execute_code (std::string code_name, std::string entry_path, std::string out_name) {
    int answer;
    string a = ("./" + code_name + " < " + entry_path + " > " + out_name);
    answer = command(a);
    return (answer == 0 ? true : false);
}

void profile(std::string exec_name, std::string profile_exec_name, std::string dot_name, std::string image_name, std::string back_path, bool all_func, bool brief){

    string compilation_format = " -c";
    compilation_format += (all_func == true ? " -z" : "");
    compilation_format += (brief == true ? "" : " -b");
    
    command("gprof ./" + exec_name + " > " + back_path + "/" + profile_exec_name + compilation_format);
    this_thread::sleep_for(chrono::milliseconds(200));
    command("gprof2dot " + back_path + "/" + profile_exec_name + " > " + back_path + "/" + dot_name + " -e0 -w" );
    this_thread::sleep_for(chrono::milliseconds(200));
    command("dot -Tpng -o " + back_path + "/" + image_name + " " + back_path + "/" + dot_name);
    this_thread::sleep_for(chrono::milliseconds(200));
}

/*
funcao para retornar o tempo em segundos da "main", recebendo como parametro o nome do arquivo 
.txt do codigo analisado e retornando o tempo em double.

**Local do arquivo passivo de alteracao, mudancas a cargo de quem mexe (luiz/emmanuel)

*/
std::string main_time(std::string exec_name, std::string arquivo){
    std::ifstream texto("../main/exits/"+ exec_name + "_exits/" + exec_name + arquivo + "_p.txt"); //local do arquivo
    if(texto.is_open() == false){
        string falha = "";
        return falha;
    } 
    std::string str, str2;
    std::string linha, numero_rev;
    bool teste = false;

    while(getline(texto, str)){
        stringstream buffer(str);
        
        while (getline(buffer, str2, ' ')) {
            if(str2 != "granularity:"){
                continue;
            }else{
                linha = str;
                teste = true;
                break;
            }      
        }
        if( teste == true) break;
    
    }

    teste = false;
    for(int i = linha.size()-1; i >= 0; i--){
        if(linha[i] == ' '){
            for(int j = i-1; ; j--){
                if(linha[j] == ' ') break;
                numero_rev.push_back(linha[j]);
                teste = true;
            }
            if(teste == true) break;
        }
    }

    reverse(numero_rev.begin(), numero_rev.end());
    if(numero_rev == "time") return "Tempo gasto para a execução da entrada: 0.00 seg";

    return "Tempo gasto para a execução da entrada: " + numero_rev + " seg";
}