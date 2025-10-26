#include "kernel/types.h"
#include "user/user.h"

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        exit(-1);
    }
    int n = strlen(argv[1]);
    if (write(1, argv[1], n) != n)
    {
        exit(-1);
    }
    exit(0);
}