#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "List.h"

int CompareStrings(char* str1, char* str2){
  int i;
  for(i = 0; str1[i] != '\0' && str2[i] != '\0';i++){
    if(str1[i] != str2[i])
      return 0;
  }

  if(str1[i] == '\0' && str2[i] == '\0')
    return 1;
  
}

char* InitPhon(){
  char* phon = malloc(sizeof(char) * 16);
    
  if(phon != NULL){
    
    for(int i = 0; i < 16; i++){
      if(i == 1 || i == 5 || i == 9 || i == 12){
	phon[i] = '-';
      }else{
	phon[i] = (rand() % 9) + '1';
      }
    }
    phon[16] = '\0';
  }
  return phon;
}

struct human* InitElement(char* surname, char* name, char* phon){

  struct human* Ptr = malloc(sizeof(struct human));
  
  Ptr->_surname = malloc(sizeof(char) * (strlen(surname) + 1));
  Ptr->_surname = strcpy(Ptr->_surname, surname);
  Ptr->_name = malloc(sizeof(char) * (strlen(name) + 1));
  Ptr->_name = strcpy(Ptr->_name, name);
  Ptr->_phonNumber = malloc(sizeof(char) * (strlen(phon) + 1));
  Ptr->_phonNumber = strcpy(Ptr->_phonNumber, phon);

  return Ptr;
}

void Print(struct human* Ptr){
  printf("-------------\n%s\n%s\n%s\n------------", Ptr->_surname, Ptr->_name, Ptr->_phonNumber );
}

void PrintList(struct List** Ptr){
  
  if(*Ptr != NULL){
    struct List* elementList;
    elementList = *Ptr;
    Print(elementList->_element);
    
    while(elementList->_nextList != NULL){
      elementList = elementList->_nextList;
      Print(elementList->_element);      
    }
  }
  else{
    printf("\nContact list is empty\n");
  }
  
}

void FreeMemory(struct List* listPtr)
{
   free(listPtr->_element->_surname);
   free(listPtr->_element->_name);
   free(listPtr->_element->_phonNumber);
   free(listPtr->_element);
   free(listPtr);
}

void AddEnd(struct List** list, char* surname, char* name, char* phon)
{
  struct human* newElement = InitElement(surname, name, phon);
  struct List* newElementList = malloc(sizeof(struct List));
  struct List* elementList; 
  
  if(newElementList != NULL && newElement != NULL){
    newElementList->_flag = 1;
    newElementList->_element = newElement;
    newElementList->_nextList = NULL;
    
    if(*list != NULL){
      
      elementList = *list;
      
      while(elementList->_nextList != NULL){
	elementList = elementList->_nextList;
      }
      elementList->_nextList = newElementList;
      
    }else{
      *list = newElementList;
    }
  }
}

struct human* FindElement(struct List** list,char* surname)
{

  struct List* elementLst = *list;
  
  if(*list != NULL){
    while((strcmp(elementLst->_element->_surname, surname) != 0) && elementLst->_nextList != NULL){
      elementLst = elementLst->_nextList;
    }

    if(strcmp(elementLst->_element->_surname, surname) == 0){
      return elementLst->_element;
    }
    else{
      return NULL;
    }
    
  }
}

struct List* FindListElement(struct List** list, char* surname)
{
  
  if(*list != NULL){

    struct List* elementLst = *list;
    
    while((strcmp(elementLst->_element->_surname, surname) != 0) && elementLst->_nextList != NULL){
      elementLst = elementLst->_nextList;
    }

    if(strcmp(elementLst->_element->_surname, surname) == 0){
      return elementLst;
    }
    else{
      return NULL;
    }
    
  }
}

void DeleteElement(struct List** list, char* surname)
{

  if(list != NULL){
    struct List* current;
    struct List*  previous;
    current = *list;
    
    if(CompareStrings(current->_element->_surname, surname)){
      *list = current->_nextList;
      FreeMemory(current);
    }
    else{
      previous = current;
      current = current->_nextList;
      while(!(CompareStrings(current->_element->_surname, surname)) && current->_nextList != NULL){
	previous = current;
	current = current->_nextList;
      }

      if(CompareStrings(current->_element->_surname, surname)){
	 previous->_nextList = current->_nextList;
	 FreeMemory(current);
      }
    }
  }
  
}

void DeleteListElement(struct List** list, struct List* listElement)
{
  
  if(list != NULL && listElement != NULL){
    struct List* current;
    struct List*  previous;
    current = *list;
    
    if(current == listElement){
      *list = current->_nextList;
    }
    else{
      previous = current;
      current = current->_nextList;
      while(current != listElement  && current->_nextList != NULL){
	previous = current;
	current = current->_nextList;
      }

      if(current == listElement){
	previous->_nextList = current->_nextList;
	FreeMemory(current);
      }
    }
  }  
}

void DeleteListAll(struct List** list )
{
  if(*list != NULL){
    struct List* current;
    struct List*  previous;
    current = *list;

    while(current != NULL){
      previous = current;
      current = current->_nextList;
      FreeMemory(previous);
    }
    *list = NULL;
  }
  else{
    printf("\nContact list is empty\n");
  }
}


void Sort(struct List** list, TDirection sortElement)
{
  if(*list != NULL ){
    //int c1 = 0;
    //int c2 = 0;

    int sizeList = 0;
    
    struct List* current;
    struct List* previous0;
    struct List* previous;
    
    struct List* current_0;
    struct List* temp;

    current = *list;
    while(current != NULL){
      current = current->_nextList;
      sizeList++;
    }
    
    int flagFirst;
    current_0 = *list;
    while(sizeList--){
      //c1++;
      current_0 = current_0->_nextList;
      flagFirst = 1;

      current = *list;
      
      while( current->_nextList != NULL){
	//c2++;
	previous0 = flagFirst ? *list: previous;  
	previous = current;
	current = current->_nextList;
	
	switch (sortElement){
	case SURNAME:
	  if(strcmp(previous->_element->_surname, current->_element->_surname) > 0){
	    if(flagFirst){
	      *list = current;
	      previous->_nextList =  current->_nextList;
	      current->_nextList = previous;
	    }
	    else{
	      previous0->_nextList = current;
	      previous->_nextList =  current->_nextList;
	      current->_nextList = previous;
	    }
	    temp = current;
	    current =  previous;
	    previous = temp;
	  }
	  
	  break;
	case NAME:
	  if(strcmp(previous->_element->_name, current->_element->_name) > 0){
	   
	    if(flagFirst){
	      *list = current;
	      previous->_nextList =  current->_nextList;
	      current->_nextList = previous;
	    }
	    else{
	      previous0->_nextList = current;
	      previous->_nextList =  current->_nextList;
	      current->_nextList = previous;
	    }
	    temp = current;
	    current =  previous;
	    previous = temp;
	  }
	  break;
	
	}
      
	flagFirst = 0;
	
      }
    }
  }
   else{
    printf("\nContact list is empty\n");
  }
}
