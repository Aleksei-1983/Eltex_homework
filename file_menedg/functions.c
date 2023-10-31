#include "file_manager.h"

int selection (const struct dirent* entry)
{
  if(strcmp(entry->d_name, ".") == 0)
    return 0;
  else
    return 1;
}

int compar(const struct dirent** a, const struct dirent** b)
{
  return strcmp((*a)->d_name, (*b)->d_name);
}
  
void initWindow(struct window** PtrWnd, int lines, int cols, int y, int x)
{
  if(*PtrWnd != NULL)
    return;

  *PtrWnd = malloc(sizeof(struct window));
  if(*PtrWnd == NULL){
    return; 
  }
  
  (*PtrWnd)->wnd = newwin(lines, cols, y, x);
  (*PtrWnd)->numFils = scandir(".", &(*PtrWnd)->ent, selection, compar);
  getcwd((*PtrWnd)->currentDirectory, SIZE_C_D);
}
  


void displayListFilnamesInWindow( WINDOW* win, struct dirent** nameList, int num_files, int cur_file, int cursor)
{

  wclear(win);
  
  // Отображение списка файлов в первом окне
  for (int i = 0 ; i < num_files; i++) {
    if(i == cur_file && cursor == TRUTH){
      wattron(win, A_REVERSE);
      //wprintw(win, "%s\n", filename[i] );
      mvwprintw(win, i, 0, "%s", nameList[i]->d_name); // Вывод имени файла в первое окно
      wattroff(win, A_REVERSE);
      continue;
      }
    //wprintw(win, "%s\n", filename[i] );
    mvwprintw(win, i, 0, "%s", nameList[i]->d_name); // Вывод имени файла в первое окно
  }
}

void messegeInWindow(WINDOW* win, char* msg, int i){
  wclear(win);
  mvwprintw(win, i, 0, "%s", msg);
  wrefresh(win);
  
}

void freeMemory(struct dirent*** nameList, int listLen){
  int index;

  for(index = 0; index < listLen; index++){
    free((*nameList)[index]);
  }
  free(*nameList);
  *nameList = NULL;

}
/* Удаляет символы в строке*/
void formatDirectory(char* bufDirectory, int  ch )
{
  int bufDirectoryLen = strlen(bufDirectory);
  
  if(ch == KEY_BACKSPACE){
    if(bufDirectoryLen <= 0){
      return;
    }
    bufDirectory[bufDirectoryLen -1] = '\0';
  }
  else{
    bufDirectory[bufDirectoryLen] = (char)ch;
    bufDirectory[bufDirectoryLen + 1] = '\0';
  }
}

void corectDir(char* dir){
  int i;
  int len = strlen(dir);

  for(i = len - 2; i > 0; i--){
    if(dir[i] == '/'){
      dir[i] = '\0';
      break;
    }
  }
}

