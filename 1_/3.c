#include <stdio.h>
#define N 9

int main()
{
    int array2[N][N];

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            if (j < (N - 1 - i)) {
                array2[i][j] = 0;
            }
            else {
                array2[i][j] = 1;
            }
                    
        }
    }

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {

            printf("%4d", array2[i][j]);
        }
        printf("\n");
    }
    
}
