/* Implement your shell here */
#include<sys/wait.h>
#include<stdio.h>
#include<stdlib.h>
#include<sysexits.h>
#include<string.h>
#include<err.h>
#include<unistd.h>
#include<sys/types.h>

#define MAX_DIR_SIZE 16
#define MAX_LEN 1024

int main(){
    int stack_top = -1;
    char *dir_stack[MAX_DIR_SIZE];
    char input[MAX_LEN + 1];


    printf("shell_jr: ");
    fflush(stdout);

    while(fgets(input, MAX_LEN + 1, stdin)){
        char cmd[MAX_LEN + 1], arg[MAX_LEN + 1];
        int input_len = strlen(input);
        
        if(input_len > 0 && input[input_len - 1] == '\n')
            input[input_len - 1] = '\0';

        sscanf(input, "%s %s", cmd, arg);
        
        if(!strcmp("exit", cmd) || !strcmp("goodbye", cmd)){
            printf("See you\n");
            fflush(stdout);
            exit(0);
        } else if(!strcmp("cd", cmd)){
            if(chdir(arg) == -1){
                printf("Cannot change to directory %s\n", arg);
                fflush(stdout);
            }
        } else if(!strcmp("pushd", cmd)){
            char curr_dir[MAX_LEN];
            if (stack_top >= MAX_DIR_SIZE - 1) {
                printf("Directory stack is full\n");
                fflush(stdout);
            } else{
                getcwd(curr_dir, MAX_LEN);
                dir_stack[++stack_top] = malloc(strlen(curr_dir) + 1);
                strcpy(dir_stack[stack_top], curr_dir);
                
                if(chdir(arg) == -1){
                    printf("Cannot change to directory %s\n", arg);
                    fflush(stdout);
                }
            }
        } else if(!strcmp("dirs", cmd)){
            int i;

            for(i = 0; i <= stack_top; i++){
                printf("%s\n", dir_stack[i]);
            }
        } else if(!strcmp("popd", cmd)){
            if (stack_top < 0) {
                printf("Directory stack is empty\n");
                fflush(stdout);
            } else{ 
                if(chdir(dir_stack[stack_top]) == -1){
                    printf("Cannot change to directory %s\n", dir_stack[stack_top]);
                    fflush(stdout);
                } else{
                free(dir_stack[stack_top]);
                stack_top--;
                }
            }
        } else {
            pid_t pid;
            if((pid = fork()) < 0) {
                err(EX_OSERR, "fork error");
            } else if(pid) {
                wait(NULL);
            } else {
                char *args[3];

                args[0] =  cmd;
                if(arg[0] == '\0')
                    args[1] = NULL;
                else
                    args[1] = arg;
                args[2] = NULL;

                execvp(cmd, args);
                printf("Failed to execute %s\n", cmd);
                fflush(stdout);
                exit(EX_OSERR);
            }
        }

        printf("shell_jr: ");
        fflush(stdout);
    }

    while(stack_top >= 0){
        free(dir_stack[stack_top--]);
    }

    return 0;
}