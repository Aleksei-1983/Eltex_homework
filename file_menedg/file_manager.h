#ifndef __FILE_MANAGER__
#define __FILE_MANAGER__

#include <stdio.h>
#include <termios.h>
#include <sys/ioctl.h>
#include <signal.h>
#include <stdlib.h>
#include <curses.h>
#include <string.h>
#include <dirent.h>
#include <unistd.h>


//#define MAX_FILES 100
#define FALSE 0
#define TRUTH 1
#define SIZE_C_D 500
#define NEW_PATH_SIZE 1000
#define PATH_TEMPLATE "%s/%s"

struct window
{
  WINDOW* wnd;
  struct dirent** ent;
  int numFils;
  char currentDirectory[SIZE_C_D];

};

int selection (const struct dirent* entry); /* отбор*/

int compar(const struct dirent** a, const struct dirent** b); /* сортеровка*/

void initWindow(struct window** PtrWnd, int lines, int cols, int y, int x); /*первичное иницилиализация окна*/

void displayListFilnamesInWindow( WINDOW* win, struct dirent** nameList, int num_files, int cur_file, int cursor); /* отображкния названия файлов в окне*/

void messegeInWindow(WINDOW* win, char* msg, int i);  /*отобразить сообщение*/

void freeMemory(struct dirent*** nameList, int listLen); /*освобождает память*/

void formatDirectory(char* bufDirectory, int  ch ); /* Удаляет символы в строке*/

void corectDir(char* dir);/* формерует путь к деректории*/



#endif //__FILE_MANAGER__
