 #include "stats_functions.h"
 #include "cla_functions.h"
 #include <signal.h>
 #include <fcntl.h> //?
 #include <sys/types.h> //?
 #include <unistd.h> //?
 #include <time.h> //?

/* Function: get_memusage()
 * ----------------------------
 *  Prints to stdout the memory usage of the program in kilobytes
 */
 void get_memusage();

/* Function: printoutline(bool sys, int n, int t, int i, bool seq)
 * ----------------------------
 *  Prints to stdout the header of the program
 * 
 * sys:     indicates if program is printing system usage
 * n:       the number of samples taken
 * t:       the amount of tdelay 
 * i:       the iteration number
 * seq:     indicates if output is sequential
 */
void printoutline(bool sys, int n, int t, int i, bool seq);

/* Function: sys_info()
 * ----------------------------
 *  Prints the system information of the machine
 *  Exits with no output on failure of retrieving system information using uname()
 * 
 * sys:     indicates if program is printing system usage
 */
void sys_info(bool sys);

/* Function: my_handler()
 * ----------------------------
 *  When signum is SIGUSR1, program returns. When signum is SIGINT, the program 
 *  will ask the user whether it wants to quit the program or not. 
 * 
 * signum:     the signal being handled
 */
void my_handler(int signum);

/* Function: cpu_graphics(float percent, int i);
 * ----------------------------
 *   Prints a graphical representation of cpu usage.
 *      
 *   percent:   the percent of CPU usage of the current sample
 *   i:         the current sample number
 */
void cpu_graphics(float percent, int i);
