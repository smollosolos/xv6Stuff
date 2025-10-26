#include "kernel/types.h"
#include "user/user.h"

// char **argv means same thing pointer means array
// so *int is array of ints
// **char is array of array if char (strings)

int main(int argc, char *argv[])
{
    char *args[3];
    args[0] = "puts";
    args[1] = "Hello, World!\n";
    args[2] = 0;
    exec("puts", args);
}