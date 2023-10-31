#include <stdio.h>
#define N 9

int main()
{
    int array1[N];
    int tmp;

    for (int i = 0; i < N; i++) {
        array1[i] = i;
    }

    for (int i = 0; i < N; i++) {
        printf("%d ", array1[i]);
    }

    printf("\n\n\n");

    for (int i = 0; i < N / 2; i++) {
        tmp = array1[i];
        array1[i] = array1[(N -1) - i];
        array1[(N - 1) - i] = tmp;
    }

    for (int i = 0; i < N; i++) {
         printf("%d ", array1[i]);
    }
    
}
