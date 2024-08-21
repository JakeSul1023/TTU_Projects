#include <stdio.h>

int main()
{
    //QUESTION 4
    int var = 0;
    int fact = 1;
    
    printf("Please input a number: ");
    scanf("%d", &var);

    while(var < 0)
    {
        printf("\nPlease input a number greater than 0: ");
        scanf("%d", &var);
    }

    for(int i = 1; i <= var; i++)
    {
        fact *= i; 
    }
    if(var == 0)
    {
        fact = 1;
    }
    
    printf("The facorial the given number: %d\n", fact);

    return 0;
}