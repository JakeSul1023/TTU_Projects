#include <stdio.h>

int main()
{
    int a = 0;
    int b = 0;
    int c = 0;

    //QUESTION 1
    printf("Please input two numbers seperated by a space: ");
    scanf("%d %d", &a, &b);

    c = a + b;

    printf("The sum is %d", c);
    
    return 0;
}