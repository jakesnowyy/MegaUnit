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
void dec_asm(u64 num_sz, u64 num);

//C functions

//First, two functions that create new megaunits
//Very self-explanatory
megaunit* new_unit_from_val(u64 val);
megaunit* new_from_size(u64 size);

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

//Arithmetic operations
void add_2nd_in_1st(megaunit* first, megaunit* second);
void sub_2nd_from_1st(megaunit* first, megaunit* second);
void mul_2nd_by_1st(megaunit* first, megaunit* second);
void div_1st_by_2nd(megaunit* first, megaunit* second);

//Factorial and power
void factorial(megaunit* num);

//Resize (or fit)
//function previously named 'resize_fit'
void recheck_size(megaunit* num);
void increase_size(megaunit* num, u64 size);

//Destructor and move
void destroy(megaunit* num);
void move(megaunit* destination, megaunit* origin);

//Print
void print_megaunit(megaunit* num);

//Read
megaunit* read_megaunit();


//main function
int main(int argc, char* argv[]){
    megaunit* test1;
    if(argc == 2){
        u64 n = 0;
        sscanf(argv[1], "%d", &n);
        test1 = new_unit_from_val(n);
    } else
        test1 = read_megaunit();
    // megaunit* test2 = read_megaunit();

    // add_2nd_in_1st(test1, test2);
    print_megaunit(test1);
    // sub_2nd_from_1st(test1, test2);
    // print_megaunit(test1);
    // mul_2nd_by_1st(test1, test2);
    // print_megaunit(test1);
    // div_1st_by_2nd(test1, test2);
    // print_megaunit(test1);
    // print_megaunit(test2);

    factorial(test1);
    print_megaunit(test1);

    destroy(test1);
    // destroy(test2);
    return 0;
}

//
//Creation section start
//
megaunit* new_unit_from_val(u64 val){
/**
 * This function returns the address of 
 * a newly allocated megaunit object
 * with size *1* and num[0] = val
 */
    megaunit* new = calloc(1, sizeof(megaunit));
    new->sz = 1;
    new->num = calloc(new->sz, sizeof(u64));
    new->num[0] = val;
    return new;
}

megaunit* new_from_size(u64 size){
/**
 * This function returns the address of
 * a newly allocated megaunit object
 * with size *size* and num[0:sz-1] = 0
 */
    megaunit* new = calloc(1, sizeof(megaunit));
    new->sz = size;
    new->num = calloc(new->sz, sizeof(u64));
    // calloc already initialized with zeroes
    return new;
}
//
//Creation section end
//

//
//Comparison section start
//
int gt(megaunit* num1, megaunit* num2){
/**
 * This function returns 1 if num1 > num2
 * and returns 0 otherwise
 */
    //First, we check the size
    if(num1->sz > num2->sz)
        return 1;
    if(num1->sz == num2->sz){
        /**
         * If their size is the same,
         * we check every num[x]
         * startig with x = sz-1
         */
        long long int i = num1->sz-1;
        while(i >= 0){
            if(num1->num[i] > num2->num[i])
                return 1;
            if(num1->num[i] < num2->num[i])
                return 0;
            i--;
        }
    }
    /**
     * If they're equal, the second if end without
     * returning, so we return 0. If we have not entered
     * in none of the two if's, then num2_sz > num1_sz
     * and we return 0.
     */
    return 0;
}

int lt(megaunit* num1, megaunit* num2){
/**
 * This function returns 1 if num1 < num2
 * and returns 0 otherwise
 */
    //First, we check the size
    if(num1->sz < num2->sz)
        return 1;
    if(num1->sz == num2->sz){
        /**
         * If their size is the same,
         * we check every num[x]
         * startig with x = sz-1
         */
        long long int i = num1->sz-1;
        while(i >= 0){
            if(num1->num[i] < num2->num[i])
                return 1;
            if(num1->num[i] > num2->num[i])
                return 0;
            i--;
        }
    }
    /**
     * If they're equal, the second if end without
     * returning, so we return 0. If we have not entered
     * in none of the two if's, then num2_sz < num1_sz
     * and we return 0.
     */
    return 0;
}

int eq(megaunit* num1, megaunit* num2){
/**
 * This function returns 1 if num1 == num2
 * and returns 0 otherwise
 */
    /**
     * If their sizes are different, they can't
     * be equal (that's because even though the sz
     * var represents how many blocks of sizeof(u64)
     * have been allocated for a specific megaunit,
     * whenever a megaunit gets smaller, the memory
     * for it will be reallocated to fit the current
     * number)
     */
    if(num1->sz != num2->sz)
        return 0;
    long long int i = num1->sz-1;
    //If at some point they aren't equal, return 0
    while(i >= 0){
        if(num1->num[i] != num2->num[i])
            return 0;
        i--;
    }
    return 1;
}

int ne(megaunit* num1, megaunit* num2){
    //Simply returns the opposite of 'eq'
    return !(eq(num1, num2));
}
//
//Comparison section end
//

//
//Shifts section start
//
void shift_bits_right(megaunit* num, u64 bits){
/**
 * This function shifts all the bits of the megaunit
 * to the right by 'bits' bits.
 * The argument 'bits' is unsigned because only
 * shitfs to the right will happen in this function
 * It's highly recommended that the 'bits' argument
 * be between 1 and 63, because this function may
 * became very inefficient with a higher number of
 * bit shifts (and also because there is a function
 * that is specially made for qword shifts...)
 */
    /**
     * This function will first **make changes**
     * and then **recheck** the megaunit size
     * mainly because this operation will only 
     * make a megaunit smaller
     */
    while(bits--)
        //This uses the assembly function 'rsb_asm'
        rsb_asm(num->sz, (u64)num->num);
    recheck_size(num);
}

void shift_bits_left(megaunit* num, u64 bits){
/**
 * This function shifts all the bits of the megaunit
 * to the left by 'bits' bits.
 * The argument 'bits' is unsigned because only
 * shitfs to the left will happen in this function
 * It's highly recommended that the 'bits' argument
 * be between 1 and 63, because this function may
 * became very inefficient with a higher number of
 * bit shifts (and also because there is a function
 * that is specially made for qword shifts...)
 */
    /**
     * This function will first **increase**
     * the megaunit size, so it can **make changes**
     * and then **recheck** the megaunit size
     */
    increase_size(num, bits%64);
    while(bits--)
        //This uses the assembly function 'rsb_asm'
        sbl_asm(num->sz, (u64)num->num);
    recheck_size(num);
}

void shift_qwords_right(megaunit* num, u64 qwords){
/**
 * This function shifts all the qwords of the megaunit
 * to the right by 'qwords' qwords.
 * The argument 'qwords' is unsigned because only
 * shitfs to the right will happen in this function
 */
    /**
     * This function will first **make changes**
     * and then **recheck** the megaunit size
     * mainly because this operation will only 
     * make a megaunit smaller
     */
    long long int i = 0;
    for(; i < num->sz-qwords; i++)
        num->num[i] = num->num[i+qwords];
    while(i < num->sz)
        num->num[i++] = 0;
    recheck_size(num);
}

void shift_qwords_left(megaunit* num, u64 qwords){
/**
 * This function shifts all the qwords of the megaunit
 * to the right by 'qwords' qwords.
 * The argument 'qwords' is unsigned because only
 * shitfs to the right will happen in this function
 */
    /**
     * This function will first **increase**
     * the megaunit size, so it can **make changes**
     * and then **recheck** the megaunit size
     */
    increase_size(num, qwords);
    long long int i = num->sz-1;
    for(; i >= qwords; i--)
        num->num[i] = num->num[i-qwords];
    while(i >= 0)
        num->num[i--] = 0;
    recheck_size(num);
}
//
//Shifts section end
//

//
//Increment and decrement section start
//
void inc(megaunit* num){
/**
 * This function adds one to the megaunit arg
 */
    /**
     * This function will first **increase**
     * the megaunit size, so it can **make changes**
     * and then **recheck** the megaunit size
     */
    increase_size(num, 1);
    //This function uses the assembly function inc_asm
    inc_asm(num->sz, (u64)num->num);
    recheck_size(num);
}

void dec(megaunit* num){
/**
 * This function subtracts one from the arg
 */
    /**
     * This function will first **make changes**
     * and then **recheck** the megaunit size
     */
    dec_asm(num->sz, (u64)num->num);
    recheck_size(num);
}
//
//Increment and decrement section end
//

//
//Arithmetic operations section start
//
void add_2nd_in_1st(megaunit* first, megaunit* second){
/**
 * This function adds the second megaunit into
 * the first. The second remains unchanged and
 * the first becames the second plus the first
 */
    /**
     * The add_asm function only works by adding
     * the smallest to the greatest megaunit
     */
    megaunit* greatest = first;
    megaunit* smallest = second;
    if(gt(smallest, greatest)){
        greatest = second;
        smallest = first;
    }
    /**
     * The result megaunit is created based on
     * the greatest, but with size +1
     */
    megaunit* result = new_from_size(greatest->sz + 1);
    memcpy(result->num, greatest->num, sizeof(u64)*greatest->sz);
    //This function uses the assembly function add_asm
    add_asm(smallest->sz, result->sz, (u64)smallest->num, (u64)result->num);
    recheck_size(result);
    /**
     * The result megaunit is going to be moved to
     * the first megaunit
     */
    move(first, result);
    //This should do the trick
    //These comments will be removed if it works properly
}

void sub_2nd_from_1st(megaunit* first, megaunit* second){
/**
 * This function subtracts the second megaunit of
 * the first. The second remains unchanged and
 * the first becames the second subtracted by the first
 */
    /**
     * The sub_asm function only works by subtracting
     * the greatest by the smallest megaunit
     */
    megaunit* greatest = first;
    megaunit* smallest = second;
    if(gt(smallest, greatest)){
        greatest = second;
        smallest = first;
    }
    /**
     * The result megaunit is created based on
     * the greatest and with the same size
     */
    megaunit* result = new_from_size(greatest->sz);
    memcpy(result->num, greatest->num, sizeof(u64)*greatest->sz);
    //This function uses the assembly function sub_asm
    sub_asm(smallest->sz, result->sz, (u64)smallest->num, (u64)result->num);
    recheck_size(result);
    /**
     * The result megaunit is going to be moved to
     * the first megaunit
     */
    move(first, result);
}

void mul_2nd_by_1st(megaunit* first, megaunit* second){
/**
 * This function multiplies the two megaunits and
 * puts the result in the first megaunit. The 
 * second megaunit remains unchanged
 */
    /**
     * The mul_asm function works by multiplying
     * every qword of the second by the first
     * and placing the results on several megaunits
     * It is more efficient (probably) if you give
     * it an array with 2*smallest->sz megaunits
     * with smallest_sz + greatest_sz qwords in each
     */
    megaunit* greatest = first;
    megaunit* smallest = second;
    if(gt(smallest, greatest)){
        greatest = second;
        smallest = first;
    }
    long long int array_size = smallest->sz*2;
    long long int megaunits_size = smallest->sz + greatest->sz;

    //This creates the array of 2*smallest->sz (megaunit*)s
    megaunit** nums = calloc(array_size, sizeof(megaunit*));

    //This creates each individual megaunit and zero initializes 
    long long int i = 0;
    for(i = 0; i < array_size; i++){
        nums[i] = new_from_size(megaunits_size);
    }
    u64* vec = calloc(array_size + 2, sizeof(u64));
    vec[0] = (u64)greatest->num;
    vec[1] = (u64)smallest->num;
    for(i = 0; i < array_size; i++){
        vec[i+2] = (u64)nums[i]->num;
    }
    mul_asm((u64)vec, greatest->sz, smallest->sz);
    for(i = 1; i < array_size; i++){
        add_2nd_in_1st(nums[0], nums[i]);
        destroy(nums[i]);
    }
    move(first, nums[0]);
    free(nums);
    free(vec);
    //Maybe its better to set them all to NULL before freeing
}

void div_1st_by_2nd(megaunit* first, megaunit* second){
/**
 * This function divides the first megaunit
 * by the second and stores the remainder in
 * the first megaunit and the quocient in the
 * second megaunit. Both are changed
 */
    /**
     * This var will keep track of how many 
     * bits the second megaunit is shifted
     */
    long long int shifted = 0;
    // long long int bits = second->sz - first->sz;
    megaunit* quo = new_from_size(first->sz);
    do{
        shift_qwords_left(second, 1);
        shifted += 64;
    } while(!lt(first, second));
    //While first >= second
    shift_qwords_right(second, 1);
    shifted -= 64;
    do{
        shift_bits_left(second, 1);
        shifted++;
    } while(!lt(first, second));
    //While first >= second
    while(shifted > 0){
        shift_bits_left(quo, 1);
        shift_bits_right(second, 1);
        shifted--;

        //While first >= second
        while(!lt(first, second)){
            sub_2nd_from_1st(first, second);
            inc(quo);
        }
    }
    move(second, quo);
}
//
//Arithmetic operations section end
//

//
//Factorial and power section start
//
void factorial(megaunit* num){
/**
 * This function calculates the factorial
 * of the megaunit 'num' and put it into itself
 */
    megaunit* one = new_unit_from_val(1);
    megaunit* temp = new_from_size(num->sz);
    memcpy(temp->num, num->num, num->sz*sizeof(u64));
    dec(temp);
    while(gt(temp, one)){
        mul_2nd_by_1st(num, temp);
        dec(temp);
    }
    destroy(temp);
    destroy(one);
}
//
//Factorial and power section end
//

//
//Resize and fit section start
//
void recheck_size(megaunit* num){
/**
 * This function checks if the megaunit
 * needs all the current allocated size
 * If it doesn't, it will be allocated
 * only the needed size
 */
    long long int i = num->sz;
    while(--i > 0 && num->num[i] == 0)
        num->sz--;
    num->num = realloc(num->num, num->sz * sizeof(u64));
}

void increase_size(megaunit* num, u64 size){
/**
 * This function increases the size of the
 * megaunit by 'size' qwords
 */
    long long int old_size = num->sz;
    num->sz += size;
    num->num = realloc(num->num, num->sz * sizeof(u64));
    long long int i = 0;
    for(; i < size; i++)
        num->num[old_size+i] = 0;
}
//
//Resize and fit section end
//

//
//Destructor and move section start
//
void destroy(megaunit* num){
/**
 * This function frees all the allocated qwords
 * for the megaunit and frees the megaunit itself
 */
    free(num->num);
    num->num = NULL;
    free(num);
    num = NULL;
}

void move(megaunit* destination, megaunit* origin){
/**
 * This function frees the 'num' of destination,
 * change it so it points to the origin's 'num'
 * and frees the origin megaunit
 */
    free(destination->num);
    destination->num = origin->num;
    destination->sz = origin->sz;
    origin->num = NULL;
    // origin->sz = 0; Maybe unnecessary
    free(origin);
    origin = NULL;
}
//
//Destructor and move section end
//

//
//Print section start
//
void print_megaunit(megaunit* num){
    megaunit* ten = new_unit_from_val(10);
    megaunit* zero = new_unit_from_val(0);

    char* s = calloc(num->sz*22, sizeof(char));
    s[0] = '0';
    long long int i = 0;
    
    megaunit* copy = new_from_size(num->sz);
    memcpy(copy->num, num->num, num->sz*sizeof(u64));

    while(gt(copy, zero)){
        div_1st_by_2nd(copy, ten);
        s[i] = (char)copy->num[0]+'0';
        i++;
        move(copy, ten);
        ten = new_unit_from_val(10);
    }
    while(--i>0){
        putchar(s[i]);
    }
    putchar(s[0]);
    putchar('\n');
    free(s);
    destroy(zero);
    // destroy(ten);
    ten = NULL;
    destroy(copy);    
}
//
//Print section end
//

//
//Read section start
//
megaunit* read_megaunit(){
    megaunit* read = new_from_size(1);
    megaunit* ten = new_unit_from_val(10);
    megaunit* digit;
    char in = getchar();
    while(in != '\n'){
        mul_2nd_by_1st(read, ten);
        digit = new_unit_from_val(in-'0');
        add_2nd_in_1st(read, digit);
        destroy(digit);
        in = getchar();
    }
    destroy(ten);
    return read;
}
//
//Read section end
//

// //NOT refactored code

// //kk
// void increase_size(megaunit* num, u64 size){
// }

// megaunit* new_from_megaunit(megaunit* num);
// megaunit* new_from_megaunit(megaunit* num){
// /**
//  * This function returns the address of
//  * a newly allocated megaunit object
//  * with the same contents as the 'num' arg
//  */
//     megaunit* new = calloc(1, sizeof(megaunit));
//     new->sz = num->sz;
//     new->num = calloc(new->sz, sizeof(u64));
//     memcpy(new->num, num->num, sizeof(u64)*new->sz);
// }

// megaunit* cpx(megaunit* num, u64 bigger, int copy){
//     megaunit* n = ct();
//     n->sz = num->sz + bigger;
//     n->num = calloc(n->sz, sizeof(u64));
//     if(n->num == NULL){
//         //This gave me an error, but I don't care right now
//         printf("Error\n");
//         return NULL;
//     }
//     if(copy)
//         memcpy(n->num, num->num, sizeof(u64)*num->sz);
//     return n;
// }

// megaunit* add(megaunit* num1, megaunit* num2){
//     megaunit *n1 = num1, *n2 = num2;
//     if(gt(num2, num1)){
//         n1 = num2;
//         n2 = num1;
//     }
//     megaunit* num3 = cpx(n1, 1, 1);
//     add_asm(n2->sz, num3->sz, (u64)n2->num, (u64)num3->num);
//     shrk(num3);
//     return num3;
// }

// megaunit* add_2(megaunit* num1, megaunit* num2){
//     megaunit *n1 = num1, *n2 = num2;
//     if(gt(num2, num1)){
//         n1 = num2;
//         n2 = num1;
//     }
//     n1->sz++;
//     realloc(n1->num, sizeof(u64)*n1->sz);
//     n1->num[n1->sz-1] = 0;
//     add_asm(n2->sz, n1->sz, (u64)n2->num, (u64)n1->num);
//     if(n1->num[n1->sz-1] == 0){
//         n1->sz--;
//         realloc(n1->num, sizeof(u64)*n1->sz);
//     }
//     return n1;
// }

// megaunit* sub(megaunit* num1, megaunit* num2){
//     megaunit *n1 = num1, *n2 = num2;
//     if(gt(num2, num1)){
//         n1 = num2;
//         n2 = num1;
//     }
//     megaunit* num3 = cpx(n1, 0, 1);
//     sub_asm(n2->sz, num3->sz, (u64)n2->num, (u64)num3->num);
//     shrk(num3);
//     return num3;
// }

// megaunit* sub_2(megaunit* num1, megaunit* num2){
//     megaunit *n1 = num1, *n2 = num2;
//     if(gt(num2, num1)){
//         n1 = num2;
//         n2 = num1;
//     }
//     sub_asm(n2->sz, n1->sz, (u64)n2->num, (u64)n1->num);
//     shrk(n1);
//     return n1;
// }

// megaunit* mul(megaunit* num1, megaunit* num2){
//     megaunit *n1 = num1, *n2 = num2;
//     if(gt(num2, num1)){
//         n1 = num2;
//         n2 = num1;
//     }
//     long long int tam = n2->sz*2;
//     megaunit** nums = calloc(tam, sizeof(megaunit*));
//     long long int i = 0;
//     for(i = 0; i < tam; i++){
//         nums[i] = ct();
//     }
//     u64* vec = calloc(tam+2, sizeof(u64));
//     vec[0] = (u64)n1->num;
//     vec[1] = (u64)n2->num;
//     for(i = 0; i < tam; i++){
//         nums[i]->num = calloc(n2->sz+n1->sz, sizeof(u64));
//         nums[i]->sz = n2->sz+n1->sz;
//         vec[i+2] = (u64)nums[i]->num;
//     }
//     mul_asm((u64)vec, n1->sz, n2->sz);
//     megaunit* num3 = ct();
//     num3->sz = 1; num3->num = calloc(1, sizeof(u64));
//     megaunit* tmp;
//     // megaunit* num3 = ct();
//     // num3->sz = n2->sz+n1->sz;
//     // num3->num = calloc(num3->sz, sizeof(u64));
//     // for(i = 0; i < tam; i++){
//     //     pr(nums[i]);
//     // }
//     pr(num3);
//     for(i = 0; i < tam; i++){
//         tmp = add(num3, nums[i]);
//         dt(nums[i]);
//         dt(num3);
//         num3 = tmp;
//     }
//     shrk(num3);
//     return num3;
// }

// megaunit* dvr(megaunit* num1, megaunit* num2){
//     megaunit *n1 = num1, *n2 = num2;
//     if(gt(num2, num1)){
//         megaunit* ret = ct();
//         ret->sz = 1;
//         ret->num = calloc(1, sizeof(u64));
//         return ret;
//     }
//     megaunit* num3 = ct();
//     num3->sz = 1;
//     num3->num = calloc(1, sizeof(u64));
//     long long int i = 0;
//     while(gt(n1, n2)){
//         ls(n2);
//         i += 64;
//     }
//     if(i == 0)
//         num3->num[0] = 1;
//     else{
//         while(gt(n2, n1)){
//             rsb(n2);
//             i--;
//         }
//         sbl(n2);
//         i++;
//         while(i > 0){
//             rsb(n2);
//             i--;
//             //shift num3 to the left 1 bit
//             sbl(num3);
//             if(!lt(n1, n2)){
//                 sub_2(n1, n2);
//                 inc(num3);
//             }
//         }
//         // if(!lt(n1, n2)){
//         //     sub_2(n1, n2);
//         //     inc(num3);
//         // }
//     }
//     shrk(num3);
//     return num3;
// }

// void inc(megaunit* num){
//     num->sz++;
//     realloc(num->num, sizeof(u64)*num->sz);
//     num->num[num->sz-1] = 0;
//     inc_asm(num->sz, (u64)num->num);
//     //shrk
//     if(num->num[num->sz-1] == 0){
//         num->sz--;
//         realloc(num->num, sizeof(u64)*num->sz);
//     }
// }

// megaunit* cnv(u64 n){
//     if(n == 0)
//         return ct();
//     megaunit* num = ct();
//     num->sz = 1;
//     num->num = calloc(1, sizeof(u64));
//     num->num[0] = n;
//     return num;
// }

// void ls(megaunit* num){
//     num->sz++;
//     realloc(num->num, sizeof(u64)*num->sz);
//     if(num->num == NULL){
//         printf("Error\n");
//         num->sz--;
//         return;
//     }
//     long long int i = num->sz-1;
//     while(i > 0){
//         num->num[i] = num->num[i-1];
//         i--;
//     }
//     num->num[i] = 0;
// }

// void rs(megaunit* num){
//     if(num->sz > 0){
//         num->sz--;
//         if(num->sz > 0){
//             long long int i = 0;
//             while(i < num->sz){
//                 num->num[i] = num->num[i+1];
//                 i++;
//             }
//         }
//         realloc(num->num, sizeof(u64)*num->sz);
//         if(num->num == NULL && num->sz > 0){
//             printf("Error\n");
//             num->sz++;
//             return;
//         }
//     }
// }

// void pr(megaunit* num){
//     // if(FULL){
//         megaunit* p = cpx(num, 0, 1);
//         char* s = calloc(p->sz*22, sizeof(char));
//         long long int i = 0;
//         megaunit* ten = ct();
//         ten->sz = 1;
//         ten->num = calloc(1, sizeof(u64));
//         ten->num[0] = 10;
//         megaunit* zero = ct();
//         zero->sz = 1;
//         zero->num = calloc(1, sizeof(u64));
//         megaunit* tmp;
//         while(gt(p, zero)){
//             tmp = dvr(p, ten);
//             s[i] = (char)p->num[0]+'0';
//             i++;
//             dt(p);
//             p = tmp;
//         }
//         while(--i>0){
//             putchar(s[i]);
//         }
//         putchar(s[i]);
//         dt(zero);
//         dt(ten);
//         dt(p);
//         free(s);
//         putchar('\n');

//     // } else{
//     //     printf("sz=%llu\n", num->sz);
//     //     long long int i = num->sz - 1;
//     //     long long int exponent = i;
//     //     if(num->sz == 0) 
//     //         printf("0");
//     //     while(i >= 0)
//     //         printf("%llu * 18446744073709551616^%lld ", num->num[i--], exponent--);
//     //     printf("\n");
//     // }
// }

// megaunit* rd(char* s){
//     if(FULL){

//     } else{
//         long long int sz = 0;
//         char* c = NULL;
//         while((c = strchr(s, ' '))){
//             *(c) = ',';
//             sz++;
//         }
//         sz+=2;
//         megaunit* num = ct();
//         if(num == NULL){
//             printf("Error\n");
//             return NULL;
//         }
//         num->sz = sz;
//         num->num = calloc(sz, sizeof(u64));
//         if(num->num == NULL){
//             printf("Error\n");
//             return NULL;
//         }
//         char* word = strtok(s, ",");
//         sz--;
//         while(word != NULL){
//             sscanf(word, "%llu", &(num->num[sz]));
//             sz--;
//             word = strtok(NULL, ",");
//         }
//         while(sz >= 0){
//             rs(num);
//             sz--;
//         }
//         return num;
//     }
// }

// void rsb(megaunit* num){
//     if(num->sz > 0){
//         rsb_asm(num->sz, (u64)num->num);
//         shrk(num);
//     }
// }

// void sbl(megaunit* num){
//     if(num->num[num->sz-1]&~(1ull<<63)){
//         num->sz++;
//         realloc(num->num, sizeof(u64)*num->sz);
//         num->num[num->sz-1] = 0;
//     }
//     sbl_asm(num->sz, (u64)num->num); //TODO
// }

// megaunit* ct(){
//     megaunit* num = (megaunit*)calloc(1, sizeof(megaunit));
//     if(num == NULL){
//         printf("Error\n");
//         return NULL;
//     }
//     return num;
// }

// void shrk(megaunit* num){
//     long long int i = num->sz-1;
//     while(num->num[i] == 0){
//         if(i > 0){
//             i--;
//         } else{
//             // realloc(num->num, sizeof(u64)*1);
//             // num->sz = 1;
//             // num->num[0] = 0;
//             return;
//         }
//     }
//     num->sz = i+1;
//     realloc(num->num, sizeof(u64)*num->sz);
// }

// void dt(megaunit* num){
//     if(num->num != NULL){
//         free(num->num);
//         num->num = NULL;
//     }
//     free(num);
//     num = NULL;
// }