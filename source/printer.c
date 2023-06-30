
#include <unistd.h>
#include <semaphore.h>
#include <stdio.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <stdint.h>
#include <sys/time.h>
#include <ncurses.h>

#include "../header/globals.h"
#include "../header/reader.h"


void * printer_gui(void* thread_dataPtr)
{
  struct threads_data *thread_data = (struct threads_data*)thread_dataPtr;
  unsigned int core_numbers =(uintptr_t)thread_data->number_of_cores;
  float read1_buffer[core_numbers];
  
  struct timeval begin, end;
  double sum = 0;
  double add = 1; 
  double cpu_time_used;

  int parent_x, parent_y, new_x, new_y;

  while(1){

    initscr();
    noecho();
    curs_set(FALSE);

    getmaxyx(stdscr, parent_y, parent_x);

    WINDOW *gui = newwin(parent_y, parent_x, 0, 0);
    getmaxyx(stdscr, new_y, new_x);

      if (new_y != parent_y || new_x != parent_x) {
        parent_x = new_x;
        parent_y = new_y;
        wresize(gui, new_y, new_x);
        wclear(stdscr);
        wclear(gui);
      }
  
    sem_wait(&thread_data->analyzer_write_ready);
    gettimeofday(&begin, 0);
    sem_post(&thread_data->printer_read_ready);
    
    float cpu=0;
    for(int core = 0; core < core_numbers; core++){
      int result = read(thread_data->analyzer_printer[0],&read1_buffer[core],4);

      #ifdef PRINT_CORES_USAGE
      mvwprintw(gui, core, 1, "core = ");
      mvwprintw(gui, core, 8, "%d",core);
      mvwprintw(gui, core, 10, "is used in = ");
      mvwprintw(gui, core, 23, "%.2f",read1_buffer[core]);
      mvwprintw(gui, core, 28, " %% ");
      #endif

      cpu+=read1_buffer[core];
    }

    #ifndef PRINT_CORES_USAGE
    cpu = cpu/core_numbers;
    mvwprintw(gui, 0, 1, "Average cpu usage = ");
    mvwprintw(gui, 0, 21, "%.2f",cpu);
    mvwprintw(gui, 0, 26, " %% ");
    #endif

    gettimeofday(&end, 0);
    long seconds = end.tv_sec - begin.tv_sec;
    long microseconds = end.tv_usec - begin.tv_usec;
    double elapsed = seconds + microseconds*1e-6;

   // mvwprintw(gui, 1, 4, "%f",elapsed);

    wrefresh(gui);
  }
  endwin();
}