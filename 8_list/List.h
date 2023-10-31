#ifndef __LIST__
#define __LIST__

struct human {
  char* _surname;
  char* _name;
  char* _phonNumber;
};

struct List{
  int _flag;
  struct human* _element;
  struct List* _nextList;
};

typedef enum {SURNAME = 1, NAME, PHON} TDirection;

int CompareStrings(char* str1, char* str2); 
char* InitPhon();
struct human* InitElement(char* surname, char* name, char* phon);
void PrintList(struct List** Ptr);
void Print(struct human* Ptr);

void FreeMemory(struct List* listPtr);

void AddEnd(struct List** list, char* surname, char* name, char* phon);
struct human* FindElement(struct List** list,char* surname);
struct List*  FindListElement(struct List** list, char* surname);
void DeleteElement(struct List** list, char* surname);
void DeleteListElement(struct List** list, struct List* listElement);
void DeleteListAll(struct List** list );


void Sort(struct List** list, TDirection sortElement);

#endif
