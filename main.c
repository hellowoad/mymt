#include <stdio.h>
#include <sys/time.h>


int get_tick_msec(){
    struct timeval time_base;
    gettimeofday(&time_base, NULL);
    return time_base.tv_sec * 1000 + time_base.tv_usec / 1000;

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
        head = NULL;
        return;
    }
    /* target t at other position */
    for(struct MultiTimer *idx = head; idx; idx = idx->next){
        if(idx->next == t){
            idx->next = t->next;
        }
    }

}
void sorted_insert(struct MultiTimer *t){
    /* if already exist, delete first */
    delete(t);
    t->ddl += get_tick_msec();
    t->next = NULL;
    /* empty list */
    if(head==NULL){
        head = t;
        return;
    }
    int inserted = 0;
    for(struct MultiTimer *idx = head;idx;idx = idx->next){
        if(t->ddl > idx->ddl){
            inserted = 1;
            /* insert to middle */
            if(idx->next != NULL){
                t->next = idx->next;
            }
            /* else insert to tail, t->next = NULL */
            idx->next = t;
        }
    }
    /* insert to head */
    if(!inserted){
       t->next = head;
       head = t;
    }
}

void yield(void){
    for(;head;head = head->next){
        if(get_tick_msec() > head->ddl){
            struct MultiTimer *expired_timer = head;
            head = head->next;
            expired_timer->callback_func();
        }

    }


}

void print_1();
struct MultiTimer t1={100000, print_1};
void print_1()
{
    printf("1111111\n");
    sorted_insert(&t1);
}
void print_2();
struct MultiTimer t2={200000, print_2};
void print_2()
{
    printf("2222222\n");
    sorted_insert(&t2);
}

int main() {

    printf("Hello, World!\n");
    printf("%d\n", get_tick_msec());
    sorted_insert(&t1);
    sorted_insert(&t2);

    while(1){
        yield();
    }
    return 0;
}
