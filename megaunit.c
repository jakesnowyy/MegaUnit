/*


*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FULL 0

typedef unsigned long long int u64;

typedef struct MegaUnit {
    u64* num;
    u64 sz;
} megaunit;

const char * ok_cmd[] = {
	"var", "eval", "quit"
};

const int n_cmds = (sizeof (ok_cmd) / sizeof (const char *));

megaunit* add(megaunit* num1, megaunit* num2);
megaunit* sub(megaunit* num1, megaunit* num2);
megaunit* mul(megaunit* num1, megaunit* num2);
megaunit* dvv(megaunit* num1, megaunit* num2);

int gt(megaunit* num1, megaunit* num2);
int lt(megaunit* num1, megaunit* num2);
int eq(megaunit* num1, megaunit* num2);
int ne(megaunit* num1, megaunit* num2);

void ls(megaunit* num);
void rs(megaunit* num);
void pr(megaunit* num);
megaunit* rd(char* s);

megaunit* ct();
void dt(megaunit* num);

int main(){

    char s[255];
    fgets(s, 255, stdin);
    megaunit* num1 = rd(s);
    pr(num1);
    fgets(s, 255, stdin);
    megaunit* num2 = rd(s);
    pr(num2);
    printf("num1 > num2 == %d\n", gt(num1, num2));
    printf("num1 < num2 == %d\n", lt(num1, num2));
    printf("num1 == num2 == %d\n", eq(num1, num2));
    printf("num1 != num2 == %d\n", ne(num1, num2));
    printf("num1 >> 2^64:\n");
    rs(num1);
    pr(num1);
    printf("num2 << 2^64:\n");
    ls(num2);
    pr(num2);
    dt(num1);
    dt(num2);



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
                quit_ok = 1;
				break;
			default:
				printf("Error\n");
                quit_ok = 1;
				break;
		}
    } while (!quit_ok);


    return 0;
}

// megaunit* add(megaunit* num1, megaunit* num2){

// }

// megaunit* sub(megaunit* num1, megaunit* num2){

// }

// megaunit* mul(megaunit* num1, megaunit* num2){

// }

// megaunit* dvv(megaunit* num1, megaunit* num2){

// }

int gt(megaunit* num1, megaunit* num2){
    if(num1->sz > num2->sz)
        return 1;
    if(num1->sz == num2->sz){
        long long int i = num1->sz-1;
        while(i >= 0){
            if(num1->num[i] > num2->num[i])
                return 1;
            if(num1->num[i] < num2->num[i])
                return 0;
            i--;
        }
    }
    return 0;
}

int lt(megaunit* num1, megaunit* num2){
    if(num1->sz < num2->sz)
        return 1;
    if(num1->sz == num2->sz){
        long long int i = num1->sz-1;
        while(i >= 0){
            if(num1->num[i] < num2->num[i])
                return 1;
            if(num1->num[i] > num2->num[i])
                return 0;
            i--;
        }
    }
    return 0;
}

int eq(megaunit* num1, megaunit* num2){
    if(num1->sz != num2->sz)
        return 0;
    long long int i = num1->sz-1;
    while(i >= 0){
        if(num1->num[i] != num2->num[i])
            return 0;
        i--;
    }
    return 1;
}

int ne(megaunit* num1, megaunit* num2){
    return !(eq(num1, num2));
}

void ls(megaunit* num){
    num->sz++;
    realloc(num->num, sizeof(u64)*num->sz);
    if(num->num == NULL){
        printf("Error\n");
        num->sz--;
        return;
    }
    long long int i = num->sz-1;
    while(i > 0){
        num->num[i] = num->num[i-1];
        i--;
    }
    num->num[i] = 0;
}

void rs(megaunit* num){
    if(num->sz > 0){
        num->sz--;
        if(num->sz > 0){
            long long int i = 0;
            while(i < num->sz){
                num->num[i] = num->num[i+1];
                i++;
            }
        }
        realloc(num->num, sizeof(u64)*num->sz);
        if(num->num == NULL && num->sz > 0){
            printf("Error\n");
            num->sz++;
            return;
        }
    }
}

void pr(megaunit* num){
    if(FULL){

    } else{
        printf("sz=%I64d\n", num->sz);
        long long int i = num->sz - 1;
        long long int exponent = i;
        if(num->sz == 0) 
            printf("0");
        while(i >= 0)
            printf("%I64d * 18446744073709551616^%I64d ", num->num[i--], exponent--);
        printf("\n");
    }
}

megaunit* rd(char* s){
    if(FULL){

    } else{
        long long int sz = 0;
        char* c = NULL;
        while((c = strchr(s, ' '))){
            *(c) = ',';
            sz++;
        }
        sz+=2;
        megaunit* num = ct();
        if(num == NULL){
            printf("Error\n");
            return NULL;
        }
        num->sz = sz;
        num->num = calloc(sz, sizeof(u64));
        if(num->num == NULL){
            printf("Error\n");
            return NULL;
        }
        char* word = strtok(s, ",");
        sz--;
        while(word != NULL){
            sscanf(word, "%I64d", &(num->num[sz]));
            sz--;
            word = strtok(NULL, ",");
        }
        while(sz >= 0){
            rs(num);
            sz--;
        }
        return num;
    }
}

megaunit* ct(){
    megaunit* num = (megaunit*)calloc(1, sizeof(megaunit));
    if(num == NULL){
        printf("Error\n");
        return NULL;
    }
    return num;
}

void dt(megaunit* num){
    if(num->num != NULL){
        free(num->num);
        num->num = NULL;
    }
    free(num);
    num = NULL;
}