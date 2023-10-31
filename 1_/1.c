/* Заполнение одномерного массива и вывод его как двумерного*/
#include <stdio.h>
#define N 5

int main()
{
    
    int array[N * N];

    for (int i = 0; i < (N * N); i++) {
        *(array + i) = i + 1;
    }
    

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            printf("%4d ", *(array+ (i * N) + j));
        }
        printf("\n");
    }
    
}
