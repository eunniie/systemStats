 #include "main.h"

void get_memusage(){
    struct rusage usage;
    if (getrusage(RUSAGE_SELF, &usage)== 0)
        printf(" Memory usage: %ld KB\n", usage.ru_maxrss); 
    else{
        perror("Error, issue with retreiving memory usage of the program");
        exit(EXIT_FAILURE);
    }
}

void printoutline(bool sys, int n, int t, int i, bool seq){
    if(seq){
        printf(">>>iteration: %d\n", i);
    }else{
        printf("Nbr of samples: %d -- every %d seconds\n", n, t);
    }
    get_memusage();
    printf("-------------------------------------\n");
    if(sys){
        printf("### Memory ### (Phys.Used/Tot -- Virtual Used/Tot)\n");
        printf("\033[%d;1H", n+5);
        printf("-------------------------------------\n");
        printf("\033[%d;1H", 5);
    }
}

void sys_info(bool sys){
    struct utsname info;
    if (uname(&info) < 0) {
        perror("Error retrieving system information");
        exit(EXIT_FAILURE);
    }
   if(sys){
        printf("-------------------------------------\n");
    }
    printf("System Name  = %s\n", info.sysname);
    printf("Machine Name = %s\n", info.nodename);
    printf("Version      = %s\n", info.version);
    printf("Release      = %s\n", info.release);
    printf("Architecture = %s\n", info.machine);
    printf("-------------------------------------\n");

}

void my_handler(int signum){
    if (signum == SIGUSR1){
        return;
    }
    if (signum == SIGINT){
        char input;
        while(1){
            printf("\033[s");
            printf("Are you sure you want to exit the program?(Y/N): ");
            scanf("%c", &input);
            if(input == 'Y' || input == 'y'){
                exit(EXIT_SUCCESS);
            }
            if(input == 'N' || input == 'n'){
                printf("\033[u");
                printf("\033[K");
                return;
            }
            printf("\033[u");
            printf("\033[K");
        }
        
    }
}

void cpu_graphics(float percent, int i){
    if(i>0){
        printf("\033[u");
    }
    if (percent >=0){
        printf("\t|||%.*s %.3f\n", (int)percent, 
        "||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||", percent);
    }else{
        printf("\t:::%.*s %.3f\n", (int)percent*-1, 
        "::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::", percent);
    }
    printf("\033[s");
    
}

int main(int argc, char **argv){
    int n = 10, t = 1;
    bool graphics = false, sys = true, user = true , seq = false;
    pid_t memory_process, user_process, cpu_process;
    int memory_pipe[2], user_pipe[2], cpu_pipe[2];
    long int ptotal = 0, pidle = 0;
    if (signal(SIGUSR1, my_handler) == SIG_ERR ){
        perror("Error setting signal handler");
        exit(EXIT_FAILURE);
    }
    if (signal(SIGTSTP, SIG_IGN) == SIG_ERR ){
        perror("Error setting signal handler");
        exit(EXIT_FAILURE);
    }
    set_current_cpu_usage(&pidle, &ptotal);
    printf("\033c"); //clears the screen
    fflush(stdout);

    //Checks if all CLA's are valid and sets them 
    if (argc > 6){
        printf("Too many command line arguments. Please execute the program again with valid arguments.\n");
        printf("The valid arguments are: --system --user --graphics --sequential --samples --tdealy\n");
        exit(EXIT_FAILURE);
    }else if (!set_cla(&argc, argv, &n, &t, &graphics, &sys, &user, &seq)){
        exit(EXIT_FAILURE);
    }
    if(pipe(user_pipe) == -1){
        perror("Error creating memory usage pipe");
        exit(EXIT_FAILURE);
    }
    user_process = fork();
        if (user_process == -1){
            perror("Error creating user process");
            exit(EXIT_FAILURE);
        }
    //Process to get user usage
    if(user_process == 0){
        close(user_pipe[0]); //close reading
        if (signal(SIGINT, SIG_IGN) == SIG_ERR ){
            perror("Error setting signal handler");
            exit(EXIT_FAILURE);
        }
        for(int i = 0; i<n; i++){
            user_usage(user_pipe[1]);
            pause();
        }
        close(user_pipe[1]);
        exit(EXIT_SUCCESS);
    }else{
        close(user_pipe[1]); //close writing
        if(pipe(memory_pipe) == -1){
            perror("Error creating user usage pipe");
            exit(EXIT_FAILURE);
        }
        memory_process = fork();
        if (memory_process == -1){
            perror("Error creating memory process");
            exit(EXIT_FAILURE);
        }
        //process to get memory usage
        if(memory_process == 0){
            close(memory_pipe[0]); //close reading
            if (signal(SIGINT, SIG_IGN) == SIG_ERR ){
                perror("Error setting signal handler");
                exit(EXIT_FAILURE);
            }
            double pre = 0.0;  //Previous sample's amount of virtually used memory in GB
            for(int i = 0; i<n; i++){
                mem_usage(graphics, &pre, memory_pipe[1]);
                pause();
            }
            close(memory_pipe[1]);
            exit(EXIT_SUCCESS);

        }else{
            close(memory_pipe[1]); //close writing
            if(pipe(cpu_pipe) == -1){
                perror("Error creating cpu usage pipe");
                exit(EXIT_FAILURE);
            }
            cpu_process = fork();
            if (cpu_process == -1){
                perror("Error creating cpu process");
                exit(EXIT_FAILURE);
            }
            
            //process to get cpu usage
            if(cpu_process == 0){
                close(cpu_pipe[0]); //close reading
            if (signal(SIGINT, SIG_IGN) == SIG_ERR ){
                perror("Error setting signal handler");
                exit(EXIT_FAILURE);
            }
                long int ptotal = 0, pidle = 0;
                for(int i = 0; i<n; i++){
                    cpu_usage(graphics, cpu_pipe[1], &pidle, &ptotal);
                    pause();
                }
                close(cpu_pipe[1]);
                exit(EXIT_SUCCESS);
            //main     
            }else{
                close(cpu_pipe[1]); //close writing
                struct sigaction act;
                act.sa_handler = my_handler;
                if(sigaction(SIGINT, &act, NULL) == -1){
                    perror("Error setting signal handler with sigaction()");
                    exit(EXIT_FAILURE);
                }
                char buf[220];
                printoutline(sys, n, t, 0, seq);
                for (int i = 0; i<n; i++){
                    //Displays memory usage
                    if (sys){
                        if(read(memory_pipe[0], buf, 220) == -1){
                            perror("Error reading memory usage from pipe");
                            exit(EXIT_FAILURE);
                        }
                        printf("%s",buf);
                        printf("\033[%d;1H", n+6);
                    }
                    //Displays user usage
                    if(user){
                        char temp;
                        printf("### Sessions/users ###\n");
                        if(read(user_pipe[0], &temp, 1) == -1){
                            perror("Error reading user usage from pipe");
                            exit(EXIT_FAILURE);
                        }
                        while (temp != '\0') {
                            printf("%c", temp);
                            if(read(user_pipe[0], &temp, 1) == -1){
                                perror("Error reading user usage from pipe");
                                exit(EXIT_FAILURE);
                            }
                        }
                        printf("-------------------------------------\n");
                    }
                    //Displays CPU usage
                    if (sys){
                        printf("Number of cores: %d\n", get_nprocs());
                        if(read(cpu_pipe[0], buf, 50) == -1){
                            perror("Error reading cpu usage from pipe");
                            exit(EXIT_FAILURE);
                        }
                        printf("%s",buf);
                        if (graphics){
                            strncpy(buf, buf+17, strlen(buf)+1);
                            cpu_graphics(strtof(buf, NULL), i);
                        }
                    }
                    sleep(t);
                    if(kill(user_process, SIGUSR1) == -1){
                        perror("Error sending signal to user process");
                        exit(EXIT_FAILURE);
                    }
                    if(kill(memory_process, SIGUSR1) == -1){
                        perror("Error sending signal to memory process");
                        exit(EXIT_FAILURE);
                    }
                    if(kill(cpu_process, SIGUSR1) == -1){
                        perror("Error sending signal to cpu process");
                        exit(EXIT_FAILURE);
                    }
                    if (i<n-1){
                        if(seq){
                            printf("\033[2J");
                            printf("\033[H");
                            fflush(stdout);
                            printoutline(sys, n, t, i+1, seq);
                        }
                        if(sys){
                            printf("\033[%d;1H", i+6);
                        }
                        if (!sys && ! seq){
                            printf("\033c");
                            printoutline(sys, n, t, i+1, seq);
                            printf("\033[4;1H");
                        }
                    }
                }
                sys_info(sys);
            }
        }
    }
}