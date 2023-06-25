
void analyze(int cpu_cores,int array[*][15]);

void *analyzeThread(void *arg);
typedef struct matrix {
    int rows;
    int cols;
    int **data;
} matrix;

enum cpu_data {
    user=1,
    cpu_nice=2,
    cpu_system=3,
    idle=4,
    iowait=5,
    irq=6,
    softir=7,
    steal=8,
    guest=9,
    guest_nice=10
    //char id;
    };
enum cpu_usage_calc{
  prev_idle=0,
  prev_total=1,
} ;

