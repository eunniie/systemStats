#include "stats_functions.h"

void user_usage(int pipefd){
    struct utmp *user;
    setutent();
    user = getutent();
    char buffer[50];
    while(user != NULL) {
        if(user->ut_type==USER_PROCESS) {
            sprintf(buffer, " %-14s%-7s (%-s)\n", user->ut_user, user->ut_line, user->ut_host);
            write(pipefd, buffer, strlen(buffer));
        }
        user = getutent();
    }
    strcpy(buffer, "\0");
    write(pipefd, buffer, strlen(buffer)+1);
    endutent();
    
}

void mem_usage(bool graphics, double *previous, int pipefd){
    struct sysinfo info;
    double current;
    char msg[225];
    char buffer[110];
    if (sysinfo(&info) < 0) {
      perror("Error retrieving system usage");
      exit(EXIT_FAILURE);
    }

    sprintf(msg, "%.2f GB / %.2f GB -- ", info.mem_unit*(info.totalram-info.freeram)/1e+9, info.mem_unit*info.totalram/1e+9);
    current = info.mem_unit*(info.totalram+info.totalswap-info.freeram-info.freeswap)/1e+9;
    sprintf(buffer,"%.2f GB / %.2f GB", current,info.mem_unit*(info.totalram+info.totalswap)/1e+9);
    strcat(msg, buffer);

    if (graphics){
        float change;
        if (*previous == 0.0){
            change = current - current;
            sprintf(buffer, "\t|%.*so %.2f (%.2f)\n", (int)(change*10), 
            "####################################################################################################", change, current);
            strcat(msg, buffer);
        }else{
            change  = current - *previous;
            sprintf(buffer, "\t|%.*s@ %.2f (%.2f)\n", (int)(change*-10), 
            "::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::", change, current);
            strcat(msg, buffer);
        }
        *previous = current;
    }else{
        strcat(msg, "\n");
    }
    write(pipefd, msg, strlen(msg)+1);
}

void set_current_cpu_usage(long int *cidle, long int *ctotal){
    char *token = NULL;
    char line[100];
    FILE* cpuinfo = fopen("/proc/stat", "r");
    //error check fopen
    if(cpuinfo == NULL){
        perror("Failed to open /proc/stat");
        exit(EXIT_FAILURE);
    }
    //error check fgets
    if(fgets(line, 100, cpuinfo) == NULL){
        perror("Error getting cpuinfo in /proc/stat");
        exit(EXIT_FAILURE);
    }
    //error check fclose
    if(fclose(cpuinfo) == EOF){
        perror("Failed to close /proc/stat");
    }
    token = strtok(line, " ");

    for (int i = 0; i < 7;i++){
        token = strtok(NULL, " ");
        *ctotal += atoi(token);
        if( i == 3){
            *cidle = atoi(token);
        }
    }   
}

void cpu_usage(bool graphics, int pipefd, long int *pidle, long int *ptotal){ 
    long int ctotal = 0, cidle = 0;
    long double pusage = 0,  cusage = 0;
    long double usage = 0;
    char msg[50];
    set_current_cpu_usage(&cidle, &ctotal);
    pusage  = *ptotal - *pidle;
    cusage = ctotal - cidle;
    usage = ((cusage - pusage)/(ctotal - *ptotal))*100;
    sprintf(msg," total cpu use = %.3Lf%%\n", usage);
    write(pipefd, msg, strlen(msg)+1);
    *pidle = cidle;
    *ptotal = ctotal;
}


