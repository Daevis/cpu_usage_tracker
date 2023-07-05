#include <unistd.h>
#include <ncurses.h>

#include "globals.h"
#include "printer.h"

/**
 * @brief Print data thread
 * 
 * This function is a thread to print average cpu usage.
 * It can be configured to print all cpus & usage.
 *
 * @param thread_dataPtr struct of shared variables between threads
 * @return void* 
 */
void* printer_gui(void* thread_dataPtr){

  struct threads_data *thread_data = (struct threads_data*)thread_dataPtr;
  unsigned int core_numbers = (unsigned int)thread_data->number_of_cores - 1;
  float print_read_buffer[core_numbers];

  while(thread_data->kill != 1){

    #ifdef PRINT_CORES_USAGE
    int parent_x, parent_y, new_x, new_y;
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
    #endif
    
    sem_post(&thread_data->printer_read_ready);
    sem_wait(&thread_data->analyzer_write_ready);
    float cpu = 0;

    for(unsigned int core = 0; core < core_numbers; core++){
      if(thread_data->kill != 1){
      long result = read(thread_data->analyzer_printer[0],&print_read_buffer[core],sizeof(cpu));
      
      if(result == -1){
        sem_post(&thread_data->send_log);
        thread_data->message = "Error with reading data from analyzer";
      }
      }
     
      #ifdef PRINT_CORES_USAGE
      mvwprintw(gui, core, 1, "core = ");
      mvwprintw(gui, core, 8, "%d",core);
      mvwprintw(gui, core, 10, "is used in = ");
      mvwprintw(gui, core, 23, "%.2f",print_read_buffer[core]);
      mvwprintw(gui, core, 28, " %% ");
      #endif

      cpu += print_read_buffer[core];
    }

    cpu = cpu/(float)core_numbers;
    thread_data->alive_sign(2);
    printf("\rAverage cpu usage = %.2f %%", (double)cpu);
    fflush(stdout);
    sem_post(&thread_data->send_log);
    thread_data->message = "No errors";

    #ifdef PRINT_CORES_USAGE
    mvwprintw(gui, 0, 1, "Average cpu usage = ");
    mvwprintw(gui, 0, 21, "%.2f",(double)cpu);
    mvwprintw(gui, 0, 26, " %% ");
    wrefresh(gui);
    #endif
  }
  return 0;
}
