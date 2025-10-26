#include "kernel/types.h"
#include "user/user.h"

int global_int = 0;

struct memlayout {
    void* text;
    int* data;
    int* stack;
    int* heap;
};

struct memvalues {
    int data;
    int stack;
    int heap; 
};

struct memcombo {
    struct memlayout memlayout;
    struct memvalues memvalues;
};


void print_mem(const char* who, struct memlayout* layout, struct memvalues* values) {
    printf("%s:stack:%p:%d\n", who, layout->stack, values->stack);
    printf("%s:heap:%p:%d\n", who, layout->heap, values->heap);
    printf("%s:data:%p:%d\n", who, layout->data, values->data);
    printf("%s:text:%p\n", who, layout->text);
}

int main( int argc, char* argv[]){
    global_int = 3;
    int local_int = 4;
    void* function_address = (void*)print_mem;
    int* pbrk = (int*) sbrk(sizeof(int));
    pbrk[0] = 5;
    struct memlayout layout = {function_address, &global_int, &local_int, pbrk};
    int p[2];
    pipe(p);
    int f = fork();
    
    if (f == -1) exit(-1);
    
    else if (f == 0){
        global_int = 10;
        local_int = 11;
        function_address = (void*)main;
        int* pbrk2 = (int*) sbrk(sizeof(int));
        pbrk2[0] = 12;

        struct memlayout layout2 = {function_address, &global_int, &local_int,  pbrk2};
        struct memvalues values = {global_int, local_int, *pbrk2};
        struct memcombo combo = {layout2, values};

        write(p[1], &combo, sizeof(struct memcombo));
    }

    else{
        struct memcombo* received = (struct memcombo*)sbrk(sizeof(struct memcombo));
        if (read(p[0],received, sizeof(struct memcombo)) != sizeof(struct memcombo)){
            exit(-1);
        }
        print_mem("child", &received->memlayout, &received->memvalues);
        struct memvalues values = {global_int, local_int, *pbrk};
        print_mem("parent", &layout, &values);
    }
    exit(0);
}
