#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

void run(char *program, char** args){
    if(fork() == 0){
        exec(program,args);
        exit(0);
    }
    return;
}

int main(int argc, char** argv){
    char buf[2048];
    char* p = buf,* last_p = buf;
    char* argsbuf[128];
    char ** args = argsbuf;
    //将xargs的参数复制到argsbus中
    for(int i = 1; i < argc; i++){
        *args = argv[i];
        args++;
    }

    //记录当前位置
    char** pa = args;
    while(read(0,p,1) != 0){
        //使用p指针遍历缓冲区，遇到空格或换行，将其替换为字符‘\0’
        if(*p == ' ' || *p == '\n'){
            *p = '\0';

            //将参数添加到缓冲区argsbuf中
            *(pa++) = last_p;
            last_p = p + 1;
            if(*p == '\n'){
                *pa = 0;
                run(argv[1],argsbuf);
                pa =args;
            }
        }
        p++;
    }
    if(pa != args){
        *p ='\0';
        *(pa++) = last_p;
        *pa = 0;
        run(argv[1],argsbuf);
    }
    while(wait(0) != -1){};
    exit(0);
}