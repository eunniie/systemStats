 #include <stdio.h>
 #include <stdlib.h>
 #include <stdbool.h>
 #include <unistd.h>
 #include <sys/sysinfo.h>
 #include <sys/resource.h>
 #include <utmp.h>
 #include <sys/utsname.h>
 #include <string.h>

/* Function: user_usage(int pipefd)
 * ----------------------------
 *  Writes all the users that are currently connected to the machine
 *  into the pipe, 'pipefd'
 *  
 *  pipefd:     the pipe to write to 
 */
void user_usage(int pipefd);



/* Function: mem_usage(bool graphics, double *previous, int pipefd)
 * ----------------------------
 *  Writes the current memory usage information of the machine to the pipe, 'pipefd'
 *  Graphical representation of the memory will be written when graphics is true.
 *  
 *      The usage is printed in the following format:
 *      PHYSICAL_USED/PHYSICAL_TOTAL --   VIRTUAL_USED/VIRTUAL_TOTAL   GRAPHIC       
 *      
 *      All memory is displayed in gigabytes.
 *      Physical memory is the actual physical RAM memory of the machine.
 *      Virtual memory accounts for the physical memory and swap space.
 *
 *  graphics:      toggle to indicate if graphics are on or off
 *  previous:      the previous sample's amount of virtually used memory in GB
 *  pipefd:        the pipe to write to
 */
void mem_usage(bool graphics, double *previous, int pipefd);

/* Function: set_current_cpu_usage(long int *cidle, long int *ctotal)
 * ----------------------------
 *  Sets the values of cidle and ctotal
 *      All time units are in USER_HZ
 *
 *  cidle:      sum of the amount of time every CPU in the machine spent idle
 *  ctotal:     sum of the amount of time every CPU in the machine spent working and idle
 */
void set_current_cpu_usage(long int *cidle, long int *ctotal);

/* Function: cpu_usage(bool graphics, int pipefd, long int *pidle, long int *ptotal)
 * ----------------------------
 *  Writes the current cpu usage percentage of the machine to the pipe, 'pipefd'
 *
 *  graphics:   toggle to indicate if graphics are on or off
 *  pipefd:     the pipe to write to 
 *  pidle:      previous sample's sum of the amount of time every CPU in the machine spent idle
 *  ptotal      previous sample's sum of the amount of time every CPU in the machine spent working and idle
 * 
 *   All time units are in USER_HZ
 */
void cpu_usage(bool graphics, int pipefd, long int *pidle, long int *ptotal);