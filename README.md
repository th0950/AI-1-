#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MAX_LEN_LINE    100

int main(void)
{
    char command[MAX_LEN_LINE];
    char *args[] = {command, NULL};
    int ret, status;
    pid_t pid, cpid;
    
    printf("안녕하세요 제가만든 shell을 사용해주셔서 감사합니다. \n");
    printf("종료키는 'exit_this_program' 입니다\n");
    printf("명령어 목록키는 'command_list 입니다.\n");
    
    while (true) {
        char *s;
        int len;
        
        printf("MyShell $ ");
        s = fgets(command, MAX_LEN_LINE, stdin);
        
        
        if (s == NULL) {
            fprintf(stderr, "fgets failed\n");
            exit(1);
        }
        
        len = strlen(command);
        printf("%d\n", len);
        if (command[len - 1] == '\n') {
            command[len - 1] = '\0'; 
        }

        printf("value of s is ");

        printf("[%s]", s);

        int len_of_s = strlen(s);

        printf("\nlength of s is ");

        printf("%d\n", len_of_s);

        if (strcmp(s, "exit_this_program") == 0) {
            printf("프로그램 종료합니다.");
            exit(0);
        }

        if (strcmp(s, "command_list") == 0) {
            printf("명령어 : cd / ls \n");
        }
        
        printf("[%s]\n", command);
      
        pid = fork();
        if (pid < 0) {
            fprintf(stderr, "fork failed\n");
            exit(1);
        } 
        if (pid != 0) {  /* parent */
            cpid = waitpid(pid, &status, 0);
            if (cpid != pid) {
                fprintf(stderr, "waitpid failed\n");        
            }
            printf("Child process terminated\n");
            if (WIFEXITED(status)) {
                printf("Exit status is %d\n", WEXITSTATUS(status)); 
            }
        }
        else {  /* child */
            ret = execve(args[0], args, NULL);
            if (ret < 0) {
                fprintf(stderr, "execve failed\n");   
                return 1;
            }
        } 
    }
    return 0;
}
