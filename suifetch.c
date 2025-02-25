#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "config.h"
#include <sys/time.h>

const char *ascii_cat = 
"\n"
" /\\_/\\  hello! \n"
"( o.o ) \n"
" > ^ <  \n";

const char *ascii_error_cat = 
"\n"
"\033[1;33mß/\\_/\\      \033[0;31mOops!\033[0m    \n"
"\033[1;33m ( o.o )    \033[0;31mError 404\033[0m  \n"
"\033[1;33m  > ^ <      \033[0;31mNot Found\033[0m \n";

char os_name[256] = ""; 

int OS_INFO(){
    FILE *opSysInfo; 
    opSysInfo = fopen("/System/Library/CoreServices/SystemVersion.plist", "r");

    if(opSysInfo == NULL){
        system("clear");        
        printf(" %s", ascii_error_cat);
        perror(">> Failed to retrieve OS name. ");
        return 1;
    }

    char line[256];
    int product_name_next = 0; 

    while(fgets(line, sizeof(line), opSysInfo)){
        if(strstr(line, "<key>ProductName</key>") != NULL){
            product_name_next = 1; 
            continue; 
        }

        if(product_name_next && strstr(line, "<string>") != NULL){
            char *start = strstr(line, "<string>") + 8;
            char *end = strstr(line, "</string>");

            if(start && end){
                int length = end - start;
                strncpy(os_name, start, length);
                os_name[length] = '\0';
                break;
            }
        }
    }
    fclose(opSysInfo);
    return 0;
}

int get_uptime() {
    FILE *fp;
    char buffer[256];

    fp = popen("uptime", "r");
    if (fp == NULL) {
        perror("Failed to run uptime command");
        return 1;
    }

    if (fgets(buffer, sizeof(buffer), fp) != NULL) {
        printf("\x1b[34muptime:\x1b[0m %s\n", buffer);
    }

    pclose(fp);
    return 0;
}

int main() { 
        
    printf("\e[33m%s\x1b[0m", ascii_cat); 
    
    if(OS_INFO() == 0 && strlen(os_name) > 0) {
        printf("\n\e[36mos:\e[0m %s", os_name);
    } else {
        printf("OS Information not available!\n");
    }
    
    if(SHOW_USER == 1) {
        char* username = getenv("USER");
        if(username != NULL)
            printf("\n\e[31muser:\e[0m %s\n", username);
        }else {
            perror("Failed to fetch data.");
        }
    if(SHOW_SHELL == 1) {
        char* shell = getenv("SHELL");
            if(shell != NULL){
                printf("\x1b[33msh:\x1b[0m %s \n", getenv("SHELL"));
            }
        }
     if(SHOW_TERM == 1){
        char* term = getenv("TERM");
        if(term != NULL) {
                printf("\x1b[32mterm:\x1b[0m %s \n", getenv("TERM"));
            }
       }

    get_uptime(); // temporary lol 

    /*
    wtf is this.
    boottime=`sysctl -n kern.boottime | awk '{print $4}' | sed 's/,//g'`
    unixtime=`date +%s`
    timeAgo=$(($unixtime - $boottime))
    uptime=`awk -v time=$timeAgo 'BEGIN { seconds = time % 60; minutes = int(time / 60 % 60); hours = int(time / 60 / 60 % 24); days = int(time / 60 / 60 / 24); printf("%.0f days, %.0f hours, %.0f minutes, %.0f seconds", days, hours, minutes, seconds); exit }'`
    echo $uptime

    someone help me convert this to up-time T-T
    */

    return 0;

}

//Ai were used to generate some of the code - will fully rewrite soon. 
