/*


*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef unsigned long long int u64;

typedef struct MegaUnit {
    u64* num;
    u64 sz;
} megaunit;

const char * ok_cmd[] = {
	"var", "eval", "quit"
};

const int n_cmds = (sizeof (ok_cmd) / sizeof (const char *));

int main(){

    #ifdef TEST
        freopen("test.in", "r" , stdin);
        freopen("test.out", "w", stdout);
    #endif

    char args[128];
    char cmd[128];
    int quit_ok = 0;
    int i = 0;
    do{
        fgets(args, 128, stdin);
        sscanf(args, "%s", cmd);
        for(i = 0; i < n_cmds; i++){
            if(!strcmp(cmd, ok_cmd[i])) break;
        }
        switch (i) {
			case 0:
				printf("Uninplemented\n");
				break;
			case 1:
				printf("Uninplemented\n");
				break;
			case 2:
				printf("OK\n");
				break;
			default:
				printf("Error");
				break;
		}
    } while (!quit_ok);


    return 0;
}