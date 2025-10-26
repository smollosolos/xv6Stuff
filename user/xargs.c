#include "kernel/types.h"
#include "user/user.h"

#define MAX_LENGTH 512

int main(int argc, char *argv[])
{

    if (argc <= 1)
        exit(0);

    char buf = 0;
    char input[MAX_LENGTH];

    int character = 0;

    while (read(0, &buf, sizeof(char)) > 0)
    {

        if (buf == '\n' || character == MAX_LENGTH - 1)
        {
            input[character] = '\0';
            character = 0;
            
            int f = fork();
            
            if (f == 0)
            {
                char *args[argc + 1];

                for (int i = 1; i < argc; i++)

                {
                    args[i - 1] = argv[i];
                }

                args[argc - 1] = input;
                args[argc] = 0;

                exec(args[0], args);
            }

            else if (f == -1) {
                fprintf(2, "xargs: fork error\n");
                exit(-1);
            }

            else
                wait(0);

        }
        else
        {
            input[character] = buf;
            character++;
        }
        
    }
    exit(0);
}