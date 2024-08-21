#include <stdio.h>

void print_message();

int main()
{
    for(int i = 0; i < 5; i++)
    {
        print_message();
    }

    return 0;
}

void print_message()
{
    printf("Hello, World!\n");
}