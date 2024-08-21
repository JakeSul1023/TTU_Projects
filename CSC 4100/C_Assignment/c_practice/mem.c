#include <stdio.h>
#include <stdlib.h>

int main()
{
    int *ptr;
    int i = 0; 
    int n = 0;
    int sum = 0;

    printf("Input the number of integers: ");
    scanf("%d", &n);

    ptr = (int*) malloc(n*sizeof(int));

    if(ptr == NULL)
    {
        printf("Error! Memory cannot be allocated.");
        exit(0);
    }
    
    for(i = 0; i < n; i++)
    {
        printf("Enter number %d: ", i + 1);
        scanf("%d", ptr + i);
        sum += *(ptr + i);
    }
   
    printf("Sum = %d\n", sum);

    free(ptr);

    return 0;
}