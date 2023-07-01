#include <unistd.h>
#include <ncurses.h>

#include "../header/globals.h"
#include "../header/printer.h"


void * printer_gui(void* thread_dataPtr)
{
  struct threads_data *thread_data = (struct threads_data*)thread_dataPtr;
  unsigned int core_numbers = (unsigned int)thread_data->number_of_cores;
  float read1_buffer[core_numbers];
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

    thread_data->watch(2);
    sem_wait(&thread_data->analyzer_write_ready);
    sem_post(&thread_data->printer_read_ready);
    

    float cpu=0;
    for(unsigned int core = 0; core < core_numbers; core++){
     read(thread_data->analyzer_printer[0],&read1_buffer[core],4);

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
    
    cpu = cpu/(float)core_numbers;
    
    mvwprintw(gui, 0, 1, "Average cpu usage = ");
    mvwprintw(gui, 0, 21, "%.2f",(double)cpu);
    mvwprintw(gui, 0, 26, " %% ");
    
    #endif
    wrefresh(gui);
  }
  //endwin();
}
