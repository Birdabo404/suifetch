#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "config.h"
#include <sys/time.h>

int main() {
	const char* ascii_cat = 
        " /\\_/\\  \n"
        "( o.o ) \n"
        " > ^ <  \n";

    const char* ascii_error_cat = 
        "\033[1;33mß/\\_/\\      \033[0;31mOops!\033[0m    \n"
        "\033[1;33m ( o.o )    \033[0;31mError 404\033[0m  \n"
        "\033[1;33m  > ^ <      \033[0;31mNot Found\033[0m \n";
       
        
    printf("\e[33m%s\x1b[0m", ascii_cat); 

    // Open the /etc/os-release file to read the Distro name of your sys.
    // edit to work in mac
    FILE *OS_INFO = fopen("/System/Library/CoreServices/SystemVersion.plist", "r");
    if( OS_INFO == NULL){
        system("clear");
        printf(" %s", ascii_error_cat);
	    perror(">> Failed to retrieve OS name. ");
	    return 1;
	}

    char line[256];
    char os_name[256] = "";

    //Reading the file line x line.
    while (fgets(line, sizeof(line), OS_INFO)) {
	    if(strncmp(line, "PRETTY_NAME=", 12) == 0) {
		    sscanf(line, "PRETTY_NAME=\"%[^\"]\"", os_name);
    } 
}
    // Close the file
    fclose(OS_INFO);


    //print the OS and version INFO.
    if(strlen(os_name) > 0 ) {
		printf("\n\e[36mos:\e[0m %s", os_name);
	} else{
		printf("OS Information not available!");
	}

    // Fetching the username of your sys.
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
    // Fetching the uptime of your sys. Please let me know if there's a better way to calculate the uptime detail.
    FILE* uptime_file = fopen("/proc/uptime", "r");
    if (uptime_file == NULL) {
        perror("Failed to open /proc/uptime");
        printf("\n>> Uptime needs updating, currently learning macos.");
        return 1;
    }

    int uptime_seconds;
    if (fscanf(uptime_file, "%d", &uptime_seconds) != 1) {
        perror("Failed to read uptime from /proc/uptime");
        fclose(uptime_file);
        return 1;
    }

    fclose(uptime_file);

    int hours = (int)(uptime_seconds / 3600);
    int minutes = ((int)uptime_seconds % 3600) / 60;
    printf("\x1b[34muptime:\x1b[0m %d hours, %d mins\n", hours, minutes);
    return 0;
}
