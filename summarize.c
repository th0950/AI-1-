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
    printf("hello\n");

    printf("command list = cd / ls \n");


    char command[MAX_LEN_LINE];  //command 라는 배열 생성
    char* args[] = { command, NULL }; //args라는 포인터배열 생성, 배열 길이는 100이고, 내용은 null이다
    int ret, status; //int형 변수 ret / status 생성 아마도 pid와 child pid의 줄임말인 듯 함
    pid_t pid, cpid; // pid를 저장할 변수 선언
                     // 프로세스 아이디를 정의하기위해 프로세스 아이디 / c프로세스 아이디를 생성

    while (true) {
        char* s;
        int len;

        printf("MyShell $ ");   
        s = fgets(command, MAX_LEN_LINE, stdin); // command라는 char배열에 최대길이 100인 커멘드를 입력시킨다.
        if (s == NULL) { //만약 command에 아무입력값이 없다면 밑의 조건문을 실행한다.
            fprintf(stderr, "fgets failed\n"); //fprintf 는 파일 출력을의미하는데, 여기선 명목상 사용한 것으로 보면 되고 그냥 문구출력한다음 에러코드띄우며 종료하라는거임
                                               //stderr은 표준에러출력장치를 의미, 보통은 모니터임
            exit(1); //exit(1)은 위의 fprintf 에 써진 fgets failed를 출력한 후 프로그램 종료하라는 의미
            // exit은 프로그램의 종료를 가능케하는 함수 exit(숫자)에 해당하는 숫자는 에러코드를 의미함 exit(0)은 정상종료 / exit(1)은 에러코드 종료
        }

        len = strlen(command); //len이라는 int형 변수의 값은 command의 길이임
        printf("%d\n", len);
        if (command[len - 1] == '\n') { //command가 예를들어 alpha를 입력했을 경우 alpha+엔터키이므로, 맨 마지막 엔터키는
            command[len - 1] = '\0';    // 이 조건문을 통해 null값으로 바꾼다 는 말임
        }

        printf("[%s]\n", command); // alpha입력시 alpha 5글자 + 엔터키로 총 6글자임

        pid = fork(); // pid의 자식 프로세스를 실행함. 자식프로세스는 위의 문장들을 가지고 있음

        if (pid < 0) { // pid가 0 미만일 수는 없음. 따라서 밑의 문장은 무조건 에러메시지 출력 후 종료
                       // 구글링하니 임의의 자식 프로세스를 기다린다고 하네
            fprintf(stderr, "fork failed\n");
            exit(1);
        }
        if (pid != 0) {  /* parent */  //pid가 0이 아니면 아래의 문장 실행. 즉, parent process에서만 밑의 메시지 실행
            cpid = waitpid(pid, &status, 0); // 1. 첫번째 인자에는 자식프로세스의 id를 넣으며, 그 프로세스가 종료될 까지 기다림
                                             // 2. status에는 프로세스가 종료 될 때의 상태 정보가 저장됨
                                             // 3. 세번째 인자는 신경쓸 거 없음 0이므로 그냥 wait함수와 같다고 보면 됨
                                             // wait함수로 부모프로세스는 자식프로세스가 종료될 때 까지 아무일도 하지 않고 기다리게 됨
                                             // waitpid 함수와 wait함수가 존재하는데, 여기서 waitpid의 세번째 인자에 0을 넣었으므로, 그냥 wait함수와 같이 동작함
            if (cpid != pid) {
                fprintf(stderr, "waitpid failed\n");
            }
            printf("Child process terminated\n");
            if (WIFEXITED(status)) { //wifexited(status)의 의미는 1,0으로만 보면 되며, 자식프로세스가 정상적으로 종료되었다면 1, 아니면 0임
                printf("Exit status is %d\n", WEXITSTATUS(status)); //WEXITSTATUS(status) 자식프로세스가 정상적으로 종료되었다면 자식프로세스의 종료코드를 리턴
            }
        }
        else {  /* child */ //pid가 0이면 아래의 문장 실행. 즉, child process에서만 밑의 메시지 실행
            ret = execve(args[0], args, NULL);   // int형 변수 ret의 값은
                                                // exeve(args[0], args, NULL); 의 뜻은 실행코드 args[0]를 메모리에 넣고 기존에 실행되던 프로세스를 없에는 것임
                                               // argv 와 envp를 두번째, 3번째 인자로 실행한다고 함
                                              // argv[0]에
            if (ret < 0) {
                fprintf(stderr, "execve failed\n");
                return 1;
            }
        }
    }
    return 0;
}
