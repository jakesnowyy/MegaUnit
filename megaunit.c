/*
  megaunit.c
  **Code being refactored**

*/

//includes
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//defines
#define FULL 0
typedef unsigned long long int u64;

//structs
typedef struct MegaUnit {
    u64* num;
    u64 sz;
} megaunit;

//assembly functions
void add_asm(u64 num2_sz, u64 num3_sz, u64 num2, u64 num3);
void sub_asm(u64 num2_sz, u64 num3_sz, u64 num2, u64 num3);
void mul_asm(u64 vec, u64 num1_sz, u64 num2_sz);
void rsb_asm(u64 num_sz, u64 num);
void sbl_asm(u64 num_sz, u64 num);
void inc_asm(u64 num_sz, u64 num);

//C functions

//First, three functions that create new megaunits
//Very self-explanatory
megaunit* new();
megaunit* new_from_size(u64 size);
megaunit* new_from_u64(u64 val);

//Then we can compare with:
int gt(megaunit* num1, megaunit* num2);
int lt(megaunit* num1, megaunit* num2);
int eq(megaunit* num1, megaunit* num2);
int ne(megaunit* num1, megaunit* num2);

//Shifts
void shift_bits_right(megaunit* num, u64 bits);
void shift_bits_left(megaunit* num, u64 bits);
void shift_qwords_right(megaunit* num, u64 qwords);
void shift_qwords_left(megaunit* num, u64 qwords);

//Increment and decrement
void inc(megaunit* num);
void dec(megaunit* num);

megaunit* cpx(megaunit* num, u64 bigger, int copy);
megaunit* add(megaunit* num1, megaunit* num2);
megaunit* add_2(megaunit* num1, megaunit* num2);
megaunit* sub(megaunit* num1, megaunit* num2);
megaunit* sub_2(megaunit* num1, megaunit* num2);
megaunit* mul(megaunit* num1, megaunit* num2);
megaunit* dvr(megaunit* num1, megaunit* num2);
megaunit* cnv(u64 n);
void ls(megaunit* num);
void rs(megaunit* num);
void pr(megaunit* num);
megaunit* rd(char* s);
void rsb(megaunit* num);
void sbl(megaunit* num);
megaunit* ct();
void shrk(megaunit* num);
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
    megaunit* result = mul(num1, num2);
    pr(result);
    dt(num1);
    dt(num2);
    dt(result);
    return 0;
}

megaunit* cpx(megaunit* num, u64 bigger, int copy){
    megaunit* n = ct();
    n->sz = num->sz + bigger;
    n->num = calloc(n->sz, sizeof(u64));
    if(n->num == NULL){
        //This gave me an error, but I don't care right now
        printf("Error\n");
        return NULL;
    }
    if(copy)
        memcpy(n->num, num->num, sizeof(u64)*num->sz);
    return n;
}

megaunit* add(megaunit* num1, megaunit* num2){
    megaunit *n1 = num1, *n2 = num2;
    if(gt(num2, num1)){
        n1 = num2;
        n2 = num1;
    }
    megaunit* num3 = cpx(n1, 1, 1);
    add_asm(n2->sz, num3->sz, (u64)n2->num, (u64)num3->num);
    shrk(num3);
    return num3;
}

megaunit* add_2(megaunit* num1, megaunit* num2){
    megaunit *n1 = num1, *n2 = num2;
    if(gt(num2, num1)){
        n1 = num2;
        n2 = num1;
    }
    n1->sz++;
    realloc(n1->num, sizeof(u64)*n1->sz);
    n1->num[n1->sz-1] = 0;
    add_asm(n2->sz, n1->sz, (u64)n2->num, (u64)n1->num);
    if(n1->num[n1->sz-1] == 0){
        n1->sz--;
        realloc(n1->num, sizeof(u64)*n1->sz);
    }
    return n1;
}

megaunit* sub(megaunit* num1, megaunit* num2){
    megaunit *n1 = num1, *n2 = num2;
    if(gt(num2, num1)){
        n1 = num2;
        n2 = num1;
    }
    megaunit* num3 = cpx(n1, 0, 1);
    sub_asm(n2->sz, num3->sz, (u64)n2->num, (u64)num3->num);
    shrk(num3);
    return num3;
}

megaunit* sub_2(megaunit* num1, megaunit* num2){
    megaunit *n1 = num1, *n2 = num2;
    if(gt(num2, num1)){
        n1 = num2;
        n2 = num1;
    }
    sub_asm(n2->sz, n1->sz, (u64)n2->num, (u64)n1->num);
    shrk(n1);
    return n1;
}

megaunit* mul(megaunit* num1, megaunit* num2){
    megaunit *n1 = num1, *n2 = num2;
    if(gt(num2, num1)){
        n1 = num2;
        n2 = num1;
    }
    long long int tam = n2->sz*2;
    megaunit** nums = calloc(tam, sizeof(megaunit*));
    long long int i = 0;
    for(i = 0; i < tam; i++){
        nums[i] = ct();
    }
    u64* vec = calloc(tam+2, sizeof(u64));
    vec[0] = (u64)n1->num;
    vec[1] = (u64)n2->num;
    for(i = 0; i < tam; i++){
        nums[i]->num = calloc(n2->sz+n1->sz, sizeof(u64));
        nums[i]->sz = n2->sz+n1->sz;
        vec[i+2] = (u64)nums[i]->num;
    }
    mul_asm((u64)vec, n1->sz, n2->sz);
    megaunit* num3 = ct();
    num3->sz = 1; num3->num = calloc(1, sizeof(u64));
    megaunit* tmp;
    // megaunit* num3 = ct();
    // num3->sz = n2->sz+n1->sz;
    // num3->num = calloc(num3->sz, sizeof(u64));
    // for(i = 0; i < tam; i++){
    //     pr(nums[i]);
    // }
    pr(num3);
    for(i = 0; i < tam; i++){
        tmp = add(num3, nums[i]);
        dt(nums[i]);
        dt(num3);
        num3 = tmp;
    }
    shrk(num3);
    return num3;
}

megaunit* dvr(megaunit* num1, megaunit* num2){
    megaunit *n1 = num1, *n2 = num2;
    if(gt(num2, num1)){
        megaunit* ret = ct();
        ret->sz = 1;
        ret->num = calloc(1, sizeof(u64));
        return ret;
    }
    megaunit* num3 = ct();
    num3->sz = 1;
    num3->num = calloc(1, sizeof(u64));
    long long int i = 0;
    while(gt(n1, n2)){
        ls(n2);
        i += 64;
    }
    if(i == 0)
        num3->num[0] = 1;
    else{
        while(gt(n2, n1)){
            rsb(n2);
            i--;
        }
        sbl(n2);
        i++;
        while(i > 0){
            rsb(n2);
            i--;
            //shift num3 to the left 1 bit
            sbl(num3);
            if(!lt(n1, n2)){
                sub_2(n1, n2);
                inc(num3);
            }
        }
        // if(!lt(n1, n2)){
        //     sub_2(n1, n2);
        //     inc(num3);
        // }
    }
    shrk(num3);
    return num3;
}

void inc(megaunit* num){
    num->sz++;
    realloc(num->num, sizeof(u64)*num->sz);
    num->num[num->sz-1] = 0;
    inc_asm(num->sz, (u64)num->num);
    //shrk
    if(num->num[num->sz-1] == 0){
        num->sz--;
        realloc(num->num, sizeof(u64)*num->sz);
    }
}

megaunit* cnv(u64 n){
    if(n == 0)
        return ct();
    megaunit* num = ct();
    num->sz = 1;
    num->num = calloc(1, sizeof(u64));
    num->num[0] = n;
    return num;
}

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
    // if(FULL){
        megaunit* p = cpx(num, 0, 1);
        char* s = calloc(p->sz*22, sizeof(char));
        long long int i = 0;
        megaunit* ten = ct();
        ten->sz = 1;
        ten->num = calloc(1, sizeof(u64));
        ten->num[0] = 10;
        megaunit* zero = ct();
        zero->sz = 1;
        zero->num = calloc(1, sizeof(u64));
        megaunit* tmp;
        while(gt(p, zero)){
            tmp = dvr(p, ten);
            s[i] = (char)p->num[0]+'0';
            i++;
            dt(p);
            p = tmp;
        }
        while(--i>0){
            putchar(s[i]);
        }
        putchar(s[i]);
        dt(zero);
        dt(ten);
        dt(p);
        free(s);
        putchar('\n');

    // } else{
    //     printf("sz=%llu\n", num->sz);
    //     long long int i = num->sz - 1;
    //     long long int exponent = i;
    //     if(num->sz == 0) 
    //         printf("0");
    //     while(i >= 0)
    //         printf("%llu * 18446744073709551616^%lld ", num->num[i--], exponent--);
    //     printf("\n");
    // }
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
            sscanf(word, "%llu", &(num->num[sz]));
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

void rsb(megaunit* num){
    if(num->sz > 0){
        rsb_asm(num->sz, (u64)num->num);
        shrk(num);
    }
}

void sbl(megaunit* num){
    if(num->num[num->sz-1]&~(1ull<<63)){
        num->sz++;
        realloc(num->num, sizeof(u64)*num->sz);
        num->num[num->sz-1] = 0;
    }
    sbl_asm(num->sz, (u64)num->num); //TODO
}

megaunit* ct(){
    megaunit* num = (megaunit*)calloc(1, sizeof(megaunit));
    if(num == NULL){
        printf("Error\n");
        return NULL;
    }
    return num;
}

void shrk(megaunit* num){
    long long int i = num->sz-1;
    while(num->num[i] == 0){
        if(i > 0){
            i--;
        } else{
            // realloc(num->num, sizeof(u64)*1);
            // num->sz = 1;
            // num->num[0] = 0;
            return;
        }
    }
    num->sz = i+1;
    realloc(num->num, sizeof(u64)*num->sz);
}

void dt(megaunit* num){
    if(num->num != NULL){
        free(num->num);
        num->num = NULL;
    }
    free(num);
    num = NULL;
}