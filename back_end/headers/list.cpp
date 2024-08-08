#ifndef _LIST_
#define _LIST_

template <typename T>
struct Node{
    T value;
    Node<T>* next;
    Node<T>* before;

    Node(void){
        this->before = nullptr;
        this->next = nullptr;
    }

    Node(T value, Node<T> *next, Node<T> *before){
        this->value = value;
        this->next = next;
        this->before = next;
    }
};

template <typename T>
struct List{
    unsigned int size_of_list;
    Node<T> *start;
    Node<T> *end;
    Node<T> *current_node;

    List(void){
        this->size_of_list = 0;
        this->start = nullptr;
        this->end = nullptr;
        this->current_node = nullptr;
    }

    void append(T value){
        Node<T> *new_node = new Node<T>(value, nullptr, nullptr);
        if(this->size_of_list == 0){
            start = end = new_node;
            start->next = start->before = end;
            end->next = end->before = start;
            current_node = start;
        }else{
            new_node->next = start;
            new_node->before = end;
            end->next = new_node;
            start->before = new_node;
            end = new_node;
        }
        (this->size_of_list)++;
    }

    void remove(void){ 
        if(this->size_of_list == 0 || this->current_node == nullptr){
            return;
        }
        Node<T>* keep = current_node->next;

        if(this->size_of_list == 1){
            this->start = this->end = nullptr;
        }else{
            this->current_node->before->next = this->current_node->next;
            this->current_node->next->before = this->current_node->before;
            
            if(this->current_node == this->start){
                this->start = this->current_node->next;
            }
            
            if(this->current_node == this->end){
                this->end = this->current_node->before;
            }
        }

        delete this->current_node;
        (this->size_of_list)--;
        if(this->size_of_list == 0){
            current_node = nullptr;
        }else{
            current_node = keep;
        }
    }

    void operator >>(int move){
        if(this->size_of_list > 0){
            for(int c = 0; c < move; c++){
                this->current_node = this->current_node->next;
            }
        }
    }

    void operator <<(int move){
        if(this->size_of_list > 0){
            for(int c = 0; c < move; c++){
                this->current_node = this->current_node->before;
            }
        }
    }

    void restart(void){
        this->current_node = this->start;
    }
    
    unsigned int size(void){
        return this->size_of_list;
    }

    void destroy_all(void){
        while(this->size_of_list > 0){
            this->remove();
        }
    }

    void pop(void){
        if(this->current_node == nullptr || this->size_of_list == 0){
            return;
        }
        if(this->size_of_list == 1){
            this->remove();
        }else{
            if(this->current_node == this->end){
                this->current_node = this->current_node->before;
            }
            this->start->before = this->end->before;
            this->end->before->next = this->start;
            delete this->end;
            this->end = start->before;
            (this->size_of_list)--;
        }
    }

    ~List(){
        while(this->size_of_list > 0){
            this->remove();
        }
    }

};

#endif