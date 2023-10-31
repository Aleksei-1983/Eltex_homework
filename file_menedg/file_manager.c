#include "file_manager.h"


int main()
{
  WINDOW* winMsg;
  WINDOW* wndDirectory;

  struct window* wnd1 = NULL;
  struct window* wnd2 = NULL;
    
  int cur_file = 0;
  int indexMsg = 0;
  
  int ch;
  int cur_in_wnd = 1;
  char bufDirectory[NEW_PATH_SIZE];
  

  // Инициализация библиотеки curse
  initscr(); // Инициализация curses и создание окна stdscr

  start_color();
  init_pair(1, COLOR_RED, COLOR_BLACK);
  init_pair(2, COLOR_CYAN, COLOR_BLACK);
  init_pair(3, COLOR_YELLOW, COLOR_BLACK);
  
  cbreak(); // Переключение в неканонический режим ввода
  noecho(); // Отключение эхо-режима
  curs_set(0);// скрываем курсов 
  keypad(stdscr, TRUE); // Включение обработки функциональных клавиш
  refresh();

  initWindow(&wnd1, LINES-8, COLS/2, 0, 0);
  initWindow(&wnd2, LINES-8, COLS/2, 0, COLS/2);
  winMsg = newwin(5, COLS, LINES-6, 0); // окно сообщений
  wndDirectory = newwin(1, COLS, LINES-7, 0);
  
  wbkgd(wnd1->wnd, COLOR_PAIR(1));
  wbkgd(wnd2->wnd, COLOR_PAIR(1));
  wbkgd(winMsg, COLOR_PAIR(2));
  wbkgd(wndDirectory, COLOR_PAIR(3));
  
  displayListFilnamesInWindow(wnd1->wnd, wnd1->ent, wnd1->numFils, cur_file, TRUTH);
  displayListFilnamesInWindow(wnd2->wnd, wnd2->ent, wnd2->numFils, cur_file, FALSE);
  messegeInWindow(winMsg, "Hi...", 0);

  if(cur_in_wnd == 1){

    sprintf(bufDirectory, PATH_TEMPLATE, wnd1->currentDirectory, "");
    mvwprintw(wndDirectory, 0, 0, "%s", bufDirectory);
     
  }

  
  wrefresh(wnd1->wnd);
  wrefresh(wnd2->wnd);
  wrefresh(wndDirectory);
  wrefresh(winMsg);

  while((ch = getch()) != 'q'){
    /*Набор пути к выбранному файлу или дериктории */
    if((ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z') || (ch == '/') || (ch == KEY_BACKSPACE)){
      formatDirectory(bufDirectory, ch);
      wclear(wndDirectory);
      mvwprintw(wndDirectory, 0, 0, "%s", bufDirectory);
      wrefresh(wndDirectory);
    }
    switch(ch){
    case KEY_UP:
      /* Перемещает курсор по списку файлов в верх в текущем окне
       * и отображает путь до выбранного файла в сторке */
      if (cur_file > 0) {
	cur_file--;
	bufDirectory[0] = '\0';
	
	if(cur_in_wnd == 1){
	  if( strcmp(wnd1->ent[cur_file]->d_name, "..") != 0){
	    //strcpy(bufDirectory, wnd1->currentDirectory);
	    sprintf(bufDirectory, PATH_TEMPLATE, wnd1->currentDirectory, wnd1->ent[cur_file]->d_name); // эта функция копирует в строку по шаблону строки
	  }
	  else{
	    sprintf(bufDirectory, PATH_TEMPLATE, wnd1->currentDirectory, "");
	  }
	  wclear(wndDirectory);
	  
	  mvwprintw(wndDirectory, 0, 0, "%s", bufDirectory);
	  displayListFilnamesInWindow(wnd1->wnd, wnd1->ent, wnd1->numFils, cur_file, TRUTH);
	  wrefresh(wnd1->wnd);
	  wrefresh(wndDirectory);
	}
	else if(cur_in_wnd == 2){
	  if( strcmp(wnd2->ent[cur_file]->d_name, "..") != 0){
	    //strcpy(bufDirectory, wnd1->currentDirectory);
	    sprintf(bufDirectory, PATH_TEMPLATE, wnd2->currentDirectory, wnd2->ent[cur_file]->d_name);
	  }
	  else{
	    sprintf(bufDirectory, PATH_TEMPLATE, wnd2->currentDirectory, "");
	  }
	  wclear(wndDirectory);
	  
	  mvwprintw(wndDirectory, 0, 0, "%s", bufDirectory);
	  displayListFilnamesInWindow(wnd2->wnd, wnd2->ent, wnd2->numFils, cur_file, TRUTH);
	  wrefresh(wnd2->wnd);
	  wrefresh(wndDirectory);
	  wrefresh(wnd2->wnd);
	}
      }
      break;
    case KEY_DOWN:
      /* Перемещает курсор по списку файлов вниз в текущем окне и отображает путь до выбранного файла в сторке */
      if (cur_file < (cur_in_wnd == 1 ? wnd1->numFils - 1 : wnd2->numFils  - 1)) {
	cur_file++;
	bufDirectory[0] = '\0';
	if(cur_in_wnd == 1){
	  if( strcmp(wnd1->ent[cur_file]->d_name, "..") != 0){
	    sprintf(bufDirectory, PATH_TEMPLATE, wnd1->currentDirectory, wnd1->ent[cur_file]->d_name); // эта функция копирует в строку по шаблону строки
	  }
	  else{
	    sprintf(bufDirectory, PATH_TEMPLATE, wnd1->currentDirectory, "");
	  }
	  wclear(wndDirectory);
	  
	  mvwprintw(wndDirectory, 0, 0, "%s", bufDirectory);
	  displayListFilnamesInWindow(wnd1->wnd, wnd1->ent, wnd1->numFils, cur_file, TRUTH);
	  wrefresh(wnd1->wnd);
	  wrefresh(wndDirectory);
	}
	else if(cur_in_wnd == 2){
	  if( strcmp(wnd2->ent[cur_file]->d_name, "..") != 0){
	    //strcpy(bufDirectory, wnd1->currentDirectory);
	    sprintf(bufDirectory, PATH_TEMPLATE, wnd2->currentDirectory, wnd2->ent[cur_file]->d_name);
	  }
	  else{
	    sprintf(bufDirectory, PATH_TEMPLATE, wnd2->currentDirectory, "");
	  }
	  wclear(wndDirectory);
	  
	  mvwprintw(wndDirectory, 0, 0, "%s", bufDirectory);
	  displayListFilnamesInWindow(wnd2->wnd, wnd2->ent, wnd2->numFils, cur_file, TRUTH);
	  wrefresh(wnd2->wnd);
	  wrefresh(wndDirectory);
	}
	
      }
      break;

    case KEY_BTAB:
      /* Переключается между окнами */
      if(cur_in_wnd == 1){
	cur_in_wnd = 2;
	cur_file = 0;
	displayListFilnamesInWindow(wnd1->wnd, wnd1->ent, wnd1->numFils, cur_file, FALSE);
	displayListFilnamesInWindow(wnd2->wnd, wnd2->ent, wnd2->numFils, cur_file, TRUTH);
		
      }
      else if(cur_in_wnd == 2){
	cur_in_wnd = 1;
	cur_file = 0;
	displayListFilnamesInWindow(wnd2->wnd, wnd2->ent, wnd2->numFils, cur_file, FALSE);
	displayListFilnamesInWindow(wnd1->wnd, wnd1->ent, wnd1->numFils, cur_file, TRUTH);
      }
      
      wrefresh(wnd1->wnd);
      wrefresh(wnd2->wnd); // Обновление второго окна
      
      break;

    case KEY_LEFT:
      /*ПРОБЛЕМА !!!!! когда в ручную изменяеш дерикторию и курсор стоит на ".." улетает на деректорию ниже*/
      /* Перкход в выбранную деректорию или если нажатию Tnter был ввод пути
       * какого либа файла или директории переходит туда  (в будующим запуск выбранного файла) */
      if(cur_in_wnd == 1){// если курсор в первом окне
	messegeInWindow(winMsg, "EMTER...", 0);

	/*заполняем содержанием новой деректории*/
	
	if(strcmp(wnd1->ent[cur_file]->d_name, "..") == 0){
	  /* Надо идти сканца дериктории и удалять текущую деректорию */
	  corectDir(bufDirectory);
	}
	
        freeMemory(&wnd1->ent, wnd1->numFils);
	wnd1->numFils = scandir(bufDirectory, &wnd1->ent, selection, compar);
	strcpy(wnd1->currentDirectory, bufDirectory);
	wclear(wndDirectory);
	  
        mvwprintw(wndDirectory, 0, 0, "%s", bufDirectory);
	cur_file = 0;
        displayListFilnamesInWindow(wnd1->wnd, wnd1->ent, wnd1->numFils, cur_file, TRUTH);
	wrefresh(wnd1->wnd);
	wrefresh(wndDirectory);

	
	
      }
      else if(cur_in_wnd == 2){ // если курсор во втором окне
	messegeInWindow(winMsg, "EMTER1...", 0);
	
	/*заполняем содержанием новой деректории*/
	
	if(strcmp(wnd2->ent[cur_file]->d_name, "..") == 0){
	  /* Надо идти сканца дериктории и удалять текущую деректорию */
	  corectDir(bufDirectory);
	}

	freeMemory(&wnd2->ent, wnd2->numFils);
	wnd2->numFils = scandir(bufDirectory, &wnd2->ent, selection, compar);
	strcpy(wnd2->currentDirectory, bufDirectory);
	wclear(wndDirectory);
	  
        mvwprintw(wndDirectory, 0, 0, "%s", bufDirectory);
	cur_file = 0;
        displayListFilnamesInWindow(wnd2->wnd, wnd2->ent, wnd2->numFils, cur_file, TRUTH);
	wrefresh(wnd2->wnd);
	wrefresh(wndDirectory);

      }
      break;
    }

  }

  refresh();
  //getch();
  endwin();
  return 0;
  
}
