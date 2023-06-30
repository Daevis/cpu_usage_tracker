struct threads_data{
    sem_t reader_send_ready;
    sem_t anazyler_read_ready;
    sem_t analyzer_write_ready;
    sem_t printer_read_ready;
    int reader_analyzer[2];
    int analyzer_printer[2];
    int number_of_cores;
};