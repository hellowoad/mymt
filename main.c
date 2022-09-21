#include <stdio.h>
#include <windows.h>
#include <stdint.h>


int  get_tick_msec(void){
    return GetTickCount();
}

struct MultiTimer{
    int ddl;
    void (*callback_func) (void);
    struct MultiTimer * next;
};
struct MultiTimer *head = NULL;
void delete(struct MultiTimer *t){
    /* target t at head */
    if(head == t){
        head = head->next;
        return;
    }
    /* target t at other position */
    for(struct MultiTimer *idx = head; idx; idx = idx->next){
        if(idx->next == t){
            idx->next = t->next;
        }
    }

}
void sorted_insert(struct MultiTimer *t, int delay_time){
    /* if already exist, delete first */
    delete(t);
    t->ddl = get_tick_msec() + delay_time;
    t->next = NULL;
    /* empty list */
    if(head==NULL){
        head = t;
        return;
    }
    /* insert to head */
    if(t->ddl < head->ddl){
        t->next = head;
        head = t;
        return;
    }
    struct MultiTimer *idx;
    for(idx = head;idx->next;idx = idx->next){
        if(t->ddl < idx->next->ddl){
            /* insert to middle */
            t->next = idx->next;
            idx->next = t;
            break;
        }
    }
    /* insert to tail */
    if(!t->next){
        idx->next = t;
    }
}

void yield(void){
    if(head){
        int now = get_tick_msec();
        if(now > head->ddl){
            struct MultiTimer *expired_timer = head;
            head = head->next;
            expired_timer->callback_func();
        }
    }
}

void print_1();
struct MultiTimer t1 = {.callback_func = print_1};
void print_1()
{
    printf("1111111\n");
    sorted_insert(&t1, 5);
}
void print_2();
struct MultiTimer t2 = {.callback_func = print_2};
void print_2()
{
    printf("2222222\n");
    sorted_insert(&t2, 1);
}
void print_3();
struct MultiTimer t3 = {.callback_func = print_3};
void print_3()
{
    printf("33333\n");
    sorted_insert(&t3, 1);
}
void print_4();
struct MultiTimer t4 = {.callback_func = print_4};
void print_4()
{
    printf("4444444\n");
    sorted_insert(&t4, 10);
}
void print_5();
struct MultiTimer t5 = {.callback_func = print_5};
void print_5()
{
    printf("5555555\n");
    sorted_insert(&t5, 1);
}



int main() {

    printf("Hello, World!\n");
    printf("%d\n", get_tick_msec());
    sorted_insert(&t1, 5);
    sorted_insert(&t2, 1);
    sorted_insert(&t3, 1);
    sorted_insert(&t4, 10);
    sorted_insert(&t5, 1);
    int a = 1;
    a++;
    if(a < 10){
        printf("aaaaa\n");
    }else if(a < 100){
        printf("bbbbb\n");
    }else{
        printf("cccc\n");
    }
    while(1){
        yield();
    }
    return 0;
}

