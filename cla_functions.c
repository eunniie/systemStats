#include "cla_functions.h"

char* get_num(char *str,int type){
	int length= strlen(str)-type;
	char *num = malloc(sizeof(char)*(length+1));
    strcpy(num,"\0");

	for(int i=type;i<strlen(str);i++){
        char cToStr[2];
        cToStr[0] = str[i];
        cToStr[1] = '\0';
		strncat(num, cToStr, 1);
	}
	return num;
}

int isvalid_cla(char *arg){
    if (strcmp(arg, "--system") == 0){
        return 1;
    }else if (strcmp(arg, "--user") == 0){
        return 2;
    }else if (strcmp(arg, "--graphics") == 0){
        return 3;
    }else if (strcmp(arg, "--sequential") == 0){
        return 4;
    }else if(strstr(arg, "--samples=") == &arg[0]){
        return 5;
    }else if(strstr(arg, "--tdelay=") == &arg[0]){
        return 6;
    }
    return -1;
}

bool isvalid_num(char *num){
    for (int i = 0; num[i]!= '\0'; i++){
        if (isdigit(num[i]) == 0){
             return false;
        }      
    }
    return true;
}

bool set_cla(int *argc, char **argv, int* n, int* t, bool *graphics, bool *sys, bool *user, bool *seq){
    int d = 0;//keep track if --user and --system were used together 
    int sample = 0, time = 0; //keeps track if multiple cla's try to change sample # or tdelay
    char *num;

    for (int i = 1; i< *argc; i++){

        if(i == 1 && isvalid_num(argv[i])){
            *n = atoi(argv[i]);
            sample++;

            if( *argc>2 && isvalid_num(argv[i+1])){
            *t = atoi(argv[i+1]);
            i+=1;
            time++;
            }
            continue;
        }

        switch (isvalid_cla(argv[i])){
            case 1:
                if (d == 0){
                    *sys = true;
                    *user = false;
                    d = 1;
                }else{
                    *sys = true;
                }
                break;

            case 2:
                if (d == 0){
                    d = 1;
                    *user = true;
                    *sys = false;
                }else{
                    *user = true;
                }
                break;

            case 3:
                *graphics = true;
                break;

            case 4:
                *seq = true;
                break;
            
            case 5:
                num = get_num(argv[i], 10);
                if(isvalid_num(num)){
                    if (sample != 0){
                        printf("ERROR: You've entered multiple arguments that change the number of samples, please only enter one.\n");
                        return false;
                    }
                    *n = atoi(num);
                    sample++;
                }else{
                    printf("ERROR: the --samples=N flag only accepts integers for N\n");
                    return false;
                }
                free(num);
                break;

            case 6:
                num = get_num(argv[i], 9);
                if(isvalid_num(num)){
                    if (time != 0){
                        printf("ERROR: You've entered multiple arguments that change the time delay, please only enter one.\n");
                        return false;
                    }
                    *t = atoi(num);
                    time++;
                }else{
                    printf("ERROR: the --tdelay=T flag only accepts integers for T\n");
                    return false;
                }
                free(num);
                break;

            default:
                printf("ERROR: Invalid command line argument(s)\n");
                printf("The valid flags are: --system --user --graphics --sequential --samples=N --tdelay=T\n");
                printf("The valid positional arguments are: N T\n");
                printf("Note: N and T are integers that appear in that EXACT order and must be the first arguements\n");
                return false;
        }
    }
    return true;
}