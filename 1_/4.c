#include <stdio.h>
#define N 5

void ulitka(int array[][N], int bigin, int siz, int namber);

int main()
{
    int array3[N][N];
   
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {

            array3[i][j] = 0;
        }
        printf("\n");
    }

   
    ulitka(array3, 0 , N , 1);


    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {

            printf("%3d", array3[i][j]);
        }
        printf("\n");
    }
    
}

void ulitka(int array[][N],int startIndex, int size, int date) {
   
    if (startIndex == size) {
        return;
    }
    else if(size - startIndex == 1){
        array[startIndex][size - 1] = date;
        return;
    }
    
    int pozishn = 1;
    int x = 0;

    for (int i = 0; i < 4; i++) {
        for (int j =  + x; j < size; j++) {
            if (pozishn == 1) {
                array[startIndex][j] = date++;
            }

            if (pozishn == 2) {
                array[j][size - 1] = date++;
            }

            if (pozishn == 3) {
                array[size - 1][(size - 1) - j + startIndex] = date++;
            }

            if (pozishn == 4) {
                if (j != size - 1)
                    array[(size - 1) - j + startIndex][startIndex] = date++;
            }
        }
        if (x < 1) x++;
        pozishn++;
    }
    ulitka(array, startIndex + 1, size - 1, date);

}
