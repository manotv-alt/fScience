
#include "/usr/include/gtk-3.0/gtk/gtk.h"
#include "../back_end/headers/list.cpp"
#include "../back_end/headers/funcoes.hpp"
#include "../back_end/headers/tad_algoritmo.hpp"
#include "../back_end/headers/nthread.hpp"
#include <string.h>

using namespace std;

// Declaração dos Objetos
GtkBuilder *builder;
GtkLabel *choose_files;
GtkLabel *main_time_label;
GtkStack *stack;
GtkFileChooser *file;
GtkMessageDialog *message;
GtkComboBoxText *adicted_codes;
GtkComboBoxText *adicted_entrys;
GtkComboBoxText *choose_codes_to_add_entry;
GtkEntry *time_code;
GtkEntry *codes_entry_box;
GtkEntry *entry_box_entry;
GtkEntry *choose_codes_entry;

// Declaração das variáveis de controle global
int time_code_value;
string file_path;
string file_type;
string code_to_add_entry;
string current_algorithm_name;
string current_entry;
Codes code_list;
List<string> list_file_name;
List<string> list_file_path;
FILE *data_base = NULL;


/*Declaração das funcões*/

// Função que checa se o algoritmo já foi adicionado
bool check_duplicate_codes(string name)
{
    for (int i = 0; i < code_list.size(); i++)
    {

        if (name == code_list.executables.current_node->value.algorithm_name)
            return 1;
        code_list.executables >> 1;
    }

    return 0;
}

// Função que exibe uma notificação na tela
void one_buttom_message(string title, string text)
{
    message = GTK_MESSAGE_DIALOG(gtk_builder_get_object(builder, "mensagem_um_botao"));

    g_object_set(message, "text", title.c_str(), NULL);
    g_object_set(message, "secondary_text", text.c_str(), NULL);

    gtk_widget_show_all(GTK_WIDGET(message));
    gtk_dialog_run(GTK_DIALOG(message));
}

// Função que checa se a entrada já foi adicionada
bool check_duplicate_entrys(string name)
{
    for (int i = 0; i < code_list.size(); i++)
    {

        if (code_to_add_entry == code_list.executables.current_node->value.algorithm_name)
        {
            for (int j = 0; j < code_list.executables.current_node->value.entries_name.size(); j++)
            {
                if (code_list.executables.current_node->value.entries_name.current_node->value == name)
                    return 1;

                code_list.executables.current_node->value.entries_name >> 1;
            }
        }

        code_list.executables >> 1;
    }

    return 0;
}

// Função que define o tempo máximo de tempo de execução
void on_time_define_buttom_clicked()
{
    string ax = gtk_entry_get_text(GTK_ENTRY(gtk_builder_get_object(builder, "time_limit_code")));
    time_code_value = atoi(ax.c_str()) * 1000;
    one_buttom_message("Status", "Tempo limite de execução atualizado para: " + ax + " seg");
}

// Função que adiciona um arquivo na tela de execução
void add_to_list(string name)
{
    if (file_type == "C")
    {
        gtk_combo_box_text_append_text(adicted_codes, name.c_str());
        gtk_combo_box_text_append_text(choose_codes_to_add_entry, name.c_str());
    }
    else if (file_type == "E")
    {
        gtk_combo_box_text_append_text(adicted_entrys, name.c_str());
    }
}

// Função que aciona o css
void style_use()
{
    GtkCssProvider *css_provider = gtk_css_provider_new();
    gtk_css_provider_load_from_path(css_provider, "../back_end/headers/colors.css", 0);
    gtk_style_context_add_provider_for_screen(
        gdk_screen_get_default(),
        GTK_STYLE_PROVIDER(css_provider),
        GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
}

// Função que reseta a lista de arquivos selecionados
void clear_choose_files()
{
    file_path = "ARQUIVOS SELECIONADOS :\n\n";
    gtk_label_set_text(choose_files, file_path.c_str());
    list_file_name.destroy_all();
    list_file_path.destroy_all();
}

// Função necessária para guardar os arquivos selecionados
void on_file_chooser_confirm_overwrite()
{
}

// Função que limpa a caixa de entradas
void clear_entry_box()
{
    gtk_combo_box_text_remove_all(adicted_entrys);
    gtk_entry_set_text(entry_box_entry, "");
}

// Função que limpa a caixa de codigos
void clear_codes_box()
{
    gtk_combo_box_text_remove_all(adicted_codes);
    gtk_entry_set_text(codes_entry_box, "");

    gtk_combo_box_text_remove_all(choose_codes_to_add_entry);
    gtk_entry_set_text(choose_codes_entry, "");
}

// Função que atualiza a lista de arquivos selecionados
void att_choose_files()
{
    file_path = "ARQUIVOS SELECIONADOS :\n\n";

    for (int i = 0; i < list_file_name.size(); i++)
    {
        file_path += list_file_name.current_node->value + '\n';
        list_file_name >> 1;
    }

    gtk_label_set_text(choose_files, file_path.c_str());
}

// Função que remove o último arquivo adicionado
void on_undo_buttom_clicked()
{

    if (list_file_name.size() == 0)
    {
        one_buttom_message("Erro", "Não há arquivos para remover!");
        return;
    }

    list_file_name << 1;
    list_file_path << 1;
    list_file_name.remove();
    list_file_path.remove();
    att_choose_files();
}

// Função para fechar a notificação da tela
void on_buttom_ok_clicked()
{
    gtk_widget_hide(GTK_WIDGET(message));
}

// Função que exibe uma notificação na tela com dois botões clicáveis
void two_buttons_message(string title, string text)
{
    message = GTK_MESSAGE_DIALOG(gtk_builder_get_object(builder, "two_buttons_message"));

    g_object_set(message, "text", title.c_str(), NULL);
    g_object_set(message, "secondary_text", text.c_str(), NULL);

    gtk_widget_show_all(GTK_WIDGET(message));
    gtk_dialog_run(GTK_DIALOG(message));
}

// Função que obtém o caminho dos arquivos
void on_file_chooser_file_activated()
{
    string ax = gtk_file_chooser_get_filename(file);
    string short_ax = get_file_name_from_path(ax);
    bool verify_2;

    if (file_type == "C")
    {
        if (check_duplicate_codes(short_ax) == 1)
        {
            one_buttom_message("Erro", "Esse algoritmo já foi adicionado!");
            return;
        }

        verify_2 = extension_file_check(short_ax);
    }
    else if (file_type == "E")
    {
        if (check_duplicate_entrys(short_ax) == 1)
        {
            one_buttom_message("Erro", "Essa entrada já foi adicionada!");
            return;
        }

        verify_2 = extension_file_check_entry(short_ax);
    }

    bool verify = duplicated_file_check(file_path, short_ax);

    if (verify == 1)
    {
        one_buttom_message("Erro", "Você já selecionou esse arquivo!");
        return;
    }
    if (verify_2 == 1)
    {
        one_buttom_message("Erro", "Extensão inválida!");
        return;
    }

    if (file_type == "C")
    {
        list_file_name.append(short_ax);
        list_file_path.append(ax);
    }
    if (file_type == "E")
    {
        list_file_name.append(short_ax);
        list_file_path.append(ax);
    }

    att_choose_files();
}

// Função que fecha o programa quando o "x" é clicado
void on_main_window_destroy()
{
    store_data(code_list); // Minhas
    gtk_main_quit();
}

// Função que leva para a tela de adicionar códigos
void on_add_code_buttom_clicked()
{
    gtk_stack_set_visible_child_name(stack, "tela_de_arquivo");
    file_type = "C";
}

// Função que leva para a tela de adicionar entradas
void on_add_entry_buttom_clicked()
{
    gtk_stack_set_visible_child_name(stack, "tela_de_escolha_de_codigo");
    file_type = "E";
}

// Função que pega o nome do código para adicionar as entradas
void on_confirm_buttom_choose_code_to_add_entry_clicked()
{
    code_to_add_entry = gtk_combo_box_text_get_active_text(choose_codes_to_add_entry);

    if (code_to_add_entry == "")
    {
        one_buttom_message("Erro", "Por favor, selecione um algoritmo primeiro!");
        return;
    }

    gtk_stack_set_visible_child_name(stack, "tela_de_arquivo");
}

// Função que atualiza as boxes dos algoritmos disponíveis no programa
void att_boxes_codes()
{

    file_type = "C";

    for (int i = 0; i < code_list.size(); i++)
    {
        add_to_list(code_list.executables.current_node->value.algorithm_name);
        code_list.executables >> 1;
    }
}

// Função que volta para a tela inicial, saindo da tela de adicionar arquivos
void on_back_buttom_clicked()
{
    gtk_stack_set_visible_child_name(stack, "pagina_inicial");
    clear_choose_files();
    one_buttom_message("Status", "Operação cancelada com sucesso!");
}

// Função que volta para a tela inicial
void on_back_buttom_choose_code_to_add_entry_clicked()
{
    on_back_buttom_clicked();
}

// Função que volta para a tela inicial, saindo da tela de execução de códigos
void on_back_buttom_2_clicked()
{
    gtk_stack_set_visible_child_name(stack, "pagina_inicial");
}

// Função que grava os códigos e entradas na lista
void on_confirmar_buttom_clicked()
{
    if (list_file_name.size() == 0)
    {
        one_buttom_message("Erro", "Por favor selecione pelo menos um arquivo!");
        return;
    }

    if (file_type == "C")
    {
        for (int i = 0; i < list_file_name.size(); i++)
        {
            code_list.append_exec(list_file_name.current_node->value, list_file_path.current_node->value);
            gtk_combo_box_text_append_text(adicted_codes, list_file_name.current_node->value.c_str());
            gtk_combo_box_text_append_text(choose_codes_to_add_entry, list_file_name.current_node->value.c_str());
            list_file_name >> 1;
            list_file_path >> 1;
        }
        list_file_name.destroy_all();
        list_file_path.destroy_all();
    }

    if (file_type == "E")
    {
        for (int j = 0; j < code_list.size(); j++)
        {
            if (code_list.executables.current_node->value.algorithm_name == code_to_add_entry)
            {
                code_list.executables.current_node->value.new_entries(list_file_name, list_file_path);
                break;
            }
            code_list.executables >> 1;
        }
    }

    gtk_stack_set_visible_child_name(stack, "pagina_inicial");
    one_buttom_message("Status", "Seus arquivos foram adicionados com sucesso!");
    clear_choose_files();
}

// Função que atualiza a caixa de entradas
void on_combo_box_codes_changed()
{
    current_algorithm_name = gtk_combo_box_text_get_active_text(adicted_codes);

    file_type = "E";

    for (int i = 0; i < code_list.size(); i++)
    {
        if (code_list.executables.current_node->value.algorithm_name == current_algorithm_name)
        {
            clear_entry_box();
            for (int j = 0; j < code_list.executables.current_node->value.entries_name.size(); j++)
            {
                add_to_list(code_list.executables.current_node->value.entries_name.current_node->value);
                code_list.executables.current_node->value.entries_name >> 1;
            }
            break;
        }
        code_list.executables >> 1;
    }
}


// Função que troca as entradas p/ o algoritmo selecionado
void on_combo_box_entrys_changed()
{
    current_entry = gtk_combo_box_text_get_active_text(adicted_entrys);
    string ax = main_time(get_exec_name_from_name(current_algorithm_name), get_exec_name_from_name(current_entry));

    if(current_algorithm_name == "" or current_entry == "") {
        return;
    }

    gtk_label_set_text(main_time_label, ax.c_str());
}


// Função que leva para a tela de execução dos códigos
void on_mycodes_buttom_clicked()
{
    gtk_stack_set_visible_child_name(stack, "tela_de_execucao");
    clear_entry_box();
    on_combo_box_codes_changed();
}

// Função que apaga o algoritmo atual na caixa da tela de execução
void on_delete_algorithm_buttom_clicked()
{

    if (current_algorithm_name == "")
    {
        one_buttom_message("Erro", "Nenhum algoritmo selecionado!");
        return;
    }

    for (int i = 0; i < code_list.size(); i++)
    {
        if (code_list.executables.current_node->value.algorithm_name == current_algorithm_name)
        {
            acess_dir("exits");
            command("rm -rf " + code_list.executables.current_node->value.executable_name + "_exits");
            acess_dir("..");
            code_list.executables.remove();
            break;
        }
        code_list.executables >> 1;
    }

    clear_codes_box();
    att_boxes_codes();
    clear_entry_box();
    on_combo_box_entrys_changed();
}

//Função que deleta a entrada selecionada
void on_clear_entry_buttom_clicked () 
{
    if(current_entry == "")
    {
        one_buttom_message("Erro", "Selecione a entrada a ser deletada!");
        return;
    }

    for (int j = 0; j < code_list.executables.current_node->value.entries_name.size(); j++)
    {
        if (code_list.executables.current_node->value.algorithm_name == current_algorithm_name)
        {
            for(int i = 0; i < code_list.executables.current_node->value.entries_name.size(); i++)
            {
                if (code_list.executables.current_node->value.entries_name.current_node->value == current_entry)
                {
                    code_list.executables.current_node->value.entries_name.remove();
                    code_list.executables.current_node->value.entries_path.remove();

                    clear_entry_box();
                    on_combo_box_codes_changed();

                    one_buttom_message("Status", "Entrada apagada com sucesso!");
                    gtk_label_set_text(main_time_label, "");
                    return;
                }
                code_list.executables.current_node->value.entries_name >> 1;
            }
        }
        code_list.executables >> 1;
    }
}

// Função que exibe os gráficos na tela quando o botão é clicado
void on_show_grafics_buttom_clicked()
{

    if (current_algorithm_name == "")
    {
        one_buttom_message("Erro", "Por favor, selecione um algoritmo primeiro!");
        return;
    }
    if (current_entry == "")
    {
        one_buttom_message("Erro", "Por favor, selecione a entrada desejada!");
        return;
    }

    for (int j = 0; j < code_list.size(); j++)
    {
        if (code_list.executables.current_node->value.algorithm_name == current_algorithm_name)
        {
            for (int i = 0; i < code_list.executables.current_node->value.entries_path.size(); i++)
            {
                if (code_list.executables.current_node->value.entries_name.current_node->value == current_entry)
                {
                    string n = get_exec_name_from_name(code_list.executables.current_node->value.entries_name.current_node->value);
                    string c = get_exec_name_from_name(code_list.executables.current_node->value.algorithm_name);
                    string a = "open exits/" + c + "_exits/" + c + n + ".png";

                    command(a);

                    return;
                }
                code_list.executables.current_node->value.entries_path >> 1;
                code_list.executables.current_node->value.entries_name >> 1;
            }
        }
        code_list.executables >> 1;
    }
}

// Função que executa apenas a entrada selecionada do algoritmo
void on_one_entry_exec_buttom_clicked()
{

    if (current_algorithm_name == "")
    {
        one_buttom_message("Erro", "Por favor, selecione um algoritmo primeiro!");
        return;
    }
    if (current_entry == "")
    {
        one_buttom_message("Erro", "Por favor, selecione a entrada desejada!");
        return;
    }

    for (int i = 0; i < code_list.size(); i++)
    {
        if (code_list.executables.current_node->value.algorithm_name == current_algorithm_name)
        {
            for (int j = 0; j < code_list.executables.current_node->value.entries_name.size(); j++)
            {
                if (code_list.executables.current_node->value.entries_name.current_node->value == current_entry)
                {
                    automatic_execution(code_list.executables.current_node->value, code_list.executables.current_node->value.entries_path.current_node->value, time_code_value);
                    break;
                }
                code_list.executables.current_node->value.entries_name >> 1;
                code_list.executables.current_node->value.entries_path >> 1;
            }
            break;
        }
        code_list.executables >> 1;
    }

    on_combo_box_entrys_changed();
}

// Função que executa todas as entradas do algoritmo selecionado
void on_one_code_all_entries_exec_buttom_clicked()
{

    if (current_algorithm_name == "")
    {
        one_buttom_message("Erro", "Por favor, selecione um algoritmo primeiro!");
        return;
    }

    for (int i = 0; i < code_list.size(); i++)
    {
        if (code_list.executables.current_node->value.algorithm_name == current_algorithm_name)
        {

            if (code_list.executables.current_node->value.entries_name.size() == 0)
            {
                one_buttom_message("Erro", "Adicione entradas primeiro!");
                return;
            }

            automatic_execution(code_list.executables.current_node->value, (time_code_value));
            break;
        }
        code_list.executables >> 1;
    }

    on_combo_box_entrys_changed();
}

// Função que executa todos os algoritmos presentes no programa
void all_codes_exec_buttom_clicked()
{

    if (code_list.size() == 0)
    {
        one_buttom_message("Erro", "Adicione os algoritmos primeiro!");
        return;
    }

    automatic_execution(code_list, (time_code_value));

    on_combo_box_entrys_changed();
}

/*Inicialização das variaveis globais*/

void init()
{

    data_base = fopen("data_base.txt", "r");

    char ax[1000];

    file_type = "C";

    stack = GTK_STACK(gtk_builder_get_object(builder, "stack_gtk"));

    file = GTK_FILE_CHOOSER(gtk_builder_get_object(builder, "file_chooser"));

    choose_files = GTK_LABEL(gtk_builder_get_object(builder, "choose_files"));

    time_code = GTK_ENTRY(gtk_builder_get_object(builder, "time_limit_code"));

    codes_entry_box = GTK_ENTRY(gtk_builder_get_object(builder, "codes_entry_box"));

    adicted_codes = GTK_COMBO_BOX_TEXT(gtk_builder_get_object(builder, "combo_box_codes"));

    adicted_entrys = GTK_COMBO_BOX_TEXT(gtk_builder_get_object(builder, "combo_box_entrys"));

    entry_box_entry = GTK_ENTRY(gtk_builder_get_object(builder, "entry_box_entry"));

    choose_codes_entry = GTK_ENTRY(gtk_builder_get_object(builder, "choose_codes_entry"));

    choose_codes_to_add_entry = GTK_COMBO_BOX_TEXT(gtk_builder_get_object(builder, "codigos_para_escolher"));

    main_time_label = GTK_LABEL(gtk_builder_get_object(builder, "main_time_label"));

    clear_choose_files();

    recover_data(code_list);

    att_boxes_codes();

    time_code_value = 5000;
}

// Programa principal
int main(int argc, char *argv[])
{

    gtk_init(&argc, &argv);

    builder = gtk_builder_new_from_file("Front_End.glade");

    init();

    gtk_builder_add_callback_symbols(builder,
                                     "on_main_window_destroy", G_CALLBACK(on_main_window_destroy),
                                     "on_add_code_buttom_clicked", G_CALLBACK(on_add_code_buttom_clicked),
                                     "on_add_entry_buttom_clicked", G_CALLBACK(on_add_entry_buttom_clicked),
                                     "on_mycodes_buttom_clicked", G_CALLBACK(on_mycodes_buttom_clicked),
                                     "on_back_buttom_clicked", G_CALLBACK(on_back_buttom_clicked),
                                     "on_back_buttom_2_clicked", G_CALLBACK(on_back_buttom_2_clicked),
                                     "on_confirmar_buttom_clicked", G_CALLBACK(on_confirmar_buttom_clicked),
                                     "on_buttom_ok_clicked", G_CALLBACK(on_buttom_ok_clicked),
                                     "on_file_chooser_file_activated", G_CALLBACK(on_file_chooser_file_activated),
                                     "on_file_chooser_confirm_overwrite", G_CALLBACK(on_file_chooser_confirm_overwrite),
                                     "on_time_define_buttom_clicked", G_CALLBACK(on_time_define_buttom_clicked),
                                     "on_back_buttom_choose_code_to_add_entry_clicked", G_CALLBACK(on_back_buttom_choose_code_to_add_entry_clicked),
                                     "on_confirm_buttom_choose_code_to_add_entry_clicked", G_CALLBACK(on_confirm_buttom_choose_code_to_add_entry_clicked),
                                     "on_combo_box_codes_changed", G_CALLBACK(on_combo_box_codes_changed),
                                     "on_combo_box_entrys_changed", G_CALLBACK(on_combo_box_entrys_changed),
                                     "on_undo_buttom_clicked", G_CALLBACK(on_undo_buttom_clicked),
                                     "on_one_code_all_entries_exec_buttom_clicked", G_CALLBACK(on_one_code_all_entries_exec_buttom_clicked),
                                     "on_one_entry_exec_buttom_clicked", G_CALLBACK(on_one_entry_exec_buttom_clicked),
                                     "on_delete_algorithm_buttom_clicked", G_CALLBACK(on_delete_algorithm_buttom_clicked),
                                     "all_codes_exec_buttom_clicked", G_CALLBACK(all_codes_exec_buttom_clicked),
                                     "on_show_grafics_buttom_clicked", G_CALLBACK(on_show_grafics_buttom_clicked),
                                     "on_clear_entry_buttom_clicked", G_CALLBACK(on_clear_entry_buttom_clicked),

                                     NULL);

    gtk_builder_connect_signals(builder, NULL);

    GtkWidget *window = GTK_WIDGET(gtk_builder_get_object(builder, "main_window"));
    style_use();
    gtk_widget_show_all(window);
    gtk_main();
}