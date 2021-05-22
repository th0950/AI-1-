//fork함수에 대해 더 깊게알기위해 shell.c을 더 자세히 분석해보았다.

#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <dirent.h> // chdir 을 위한 헤더파일 (cd 명령어를 만들기 위함)
#include <sys/wait.h>

#define MAX_LEN_LINE    100

int main(void)
{
    char command[MAX_LEN_LINE];
    char *args[] = {command, NULL};
    int ret, status;
    pid_t pid, cpid;
    
    printf("안녕하세요 제가만든 shell을 사용해주셔서 감사합니다. \n");
    printf("종료키는 'exit' 입니다\n");
    printf("명령어 목록키는 'help' 입니다.\n\n\n\n\n");
    
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
        
        if (command[len - 1] == '\n') {
            command[len - 1] = '\0'; 
        }

        
      
        pid = fork();

        printf("%d", pid);
        printf("이 pid의 값임 \n");


        if (pid < 0) { //pid 는 음수
            printf("pid<0인 구문 실행");
            fprintf(stderr, "fork failed\n");
            exit(1);
        }

        if (pid != 0) {  /* parent */ // pid 는 양수
            printf("부모 프로세스 실행되었음\n");
            
            cpid = waitpid(pid, &status, 0);

            if (cpid != pid) {
                fprintf(stderr, "waitpid failed\n");        
            }

            printf("Child process terminated\n");

            if (WIFEXITED(status)) {
                printf("Exit status is %d\n", WEXITSTATUS(status)); 
            }
        }



        else {  /* child */ //pid 는 0
            printf("자식 프로세스 실행되었음\n");

            if (strcmp(s, "exit") == 0) {
                printf("프로그램 종료합니다.");
                return 1;
            }


            if (strcmp(s, "help") == 0) {
                printf("명령어 : 오늘의 메뉴 / 오늘의 날씨 / 미세먼지 정보 / 눈뽕\n");
                exit(0);
            }

            printf("자식프로세스 종료되어가고있는 것 같네요\n");



            ret = execve(args[0], args, NULL);
            if (ret < 0) {
                fprintf(stderr, "execve failed\n");   
                return 1;
            }

            printf("자식프로세스 종료되어버렸네요\n");
        }

        printf("while문의 종료\n"); 
    }
    return 0;
}
