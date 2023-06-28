struct threads_data{
   // pthread_mutex_t lock;
    sem_t reader_send_ready;
    sem_t anazyler_read_ready;
    int reader_analyzer[2];
    int analyzer_printer[2];
    int number_of_cores;
};