#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "List.h"

#define SU 15
#define NA 15
#define SIZ 20


int main (char argc, char* argv){

  int flag = 1;
  int index = 0;
  int chooseSort;
  char surnameIn[20];
  char nameIn[20];
  char phonIn[20];
  
  struct List* startList = NULL;
  
  char surname[SU][20] = {"Ivanov", "Cmernov", "Cidorod", "Fdorov", "Petrov",
    "Volkov", "Morozov", "Alekseev","Kozlov", "Orlov", "Nikolaev", "Romanov", "Grigopyev",
    "Kuzmin", "Zaycev"};

  char name[NA][20] = {"Artem", "Maksim", "Poman", "Volodya", "Matvey",
    "Timofey", "Yaroslav", "Danil","Stepan", "Timur", "Ilya", "Egor", "Pyotr",
    "Kiril", "Ivan"};

  for(int i = 0; i < SIZ; i++){
    AddEnd(&startList, surname[rand()%SU], name[rand()%NA], InitPhon());
  }

  while(flag){
    printf("\n1. add element\n2. fand element to surname\n3. delete element to surname\n4. delete all elements\n5. print all\n6. sort\n7. exit\n");
    scanf("%d", &index);
    switch(index){
    case 1:
      printf("\nEnter Surname Nema Phon: ");
      scanf("%s", surnameIn);
      scanf("%s", nameIn);
      scanf("%s", phonIn);
      AddEnd(&startList, surnameIn, nameIn, phonIn);
      break;
    case 2:
      printf("\nEnter Surname: ");
      scanf("%s", surnameIn);
      struct human* temp = FindElement(&startList, surnameIn);
      if(temp != NULL)
	Print(temp);
      else
	printf("Element do not fond\n");
      break;
    case 3:
      printf("\nEnter Surname: ");
      scanf("%s", surnameIn);
      DeleteElement(&startList, surnameIn);
      break;
    case 4:
      DeleteListAll(&startList);
      break;
    case 5:
      PrintList(&startList);
      break;
    case 6:
      printf("\nChoose what to sort by;\n1 = surname, 2 = name: ");
      scanf("%d", &chooseSort);
      if(chooseSort == 1)
	Sort(&startList, SURNAME);
      else if(chooseSort == 2)
	Sort(&startList, NAME);
      break;
    case 7:
      flag = 0;
      break;
    }
  }
  

  DeleteListAll(&startList);
  return 0;
}

