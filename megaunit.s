global add_asm
global sub_asm
global mul_asm
; global div_asm

global rsb_asm
global sbl_asm

global inc_asm
global dec_asm

;return register
%define r_ret rax

;scratch registers
%define r_scr0 r10
%define r_scr1 r11

;semi_scratch registers
%define r_ssc0 r12
%define r_ssc1 r13
%define r_ssc2 r14
%define r_ssc3 r15

%define r_nsc0 rbx
%define r_mul1 rdx

;argument registers
%ifdef linux
    %define r_arg0 rdi
    %define r_arg1 rsi
    %define r_arg2 rdx
    %define r_arg3 rcx
%endif

%ifdef windwos
    %define r_arg0 rcx
    %define r_arg1 rdx
    %define r_arg2 r8
    %define r_arg3 r9
%endif


section .text
bits 64
default rel

add_asm:
    sub rsp, 32
    ; num2_sz => r_arg0 RCX -- becames loop_1_counter
    ; num3_sz => r_arg1 RDX -- becames loop_2_counter
    ; num2*   => r_arg2 R8
    ; num3*   => r_arg3 R9
    ; r_scr0 R10 => num_idx
    ; r_scr1 R11 => temp_guy
    xor r_scr0, r_scr0 ; num_idx = 0; carry = 0;
    inc r_arg1
    inc r_arg1
    inc r_arg0
add_loop_1:
    dec r_arg1
    dec r_arg0
    jz add_loop_2 ; if zero goto 2nd loop
    mov r_scr1, [r_arg2+8*r_scr0]
    adc [r_arg3+8*r_scr0], r_scr1
    inc r_scr0
    jmp add_loop_1
add_loop_2:
    dec r_arg1
    jz add_end
    adc qword[r_arg3+8*r_scr0], 0
    inc r_scr0
    jmp add_loop_2
add_end:
    add rsp, 32
    ret

sub_asm:
    sub rsp, 32
    ; num2_sz => r_arg0 RCX -- becames loop_1_counter
    ; num3_sz => r_arg1 RDX -- becames loop_2_counter
    ; num2*   => r_arg2 R8
    ; num3*   => r_arg3 R9
    ; r_scr0 R10 => num_idx
    ; r_scr1 R11 => temp_guy
    xor r_scr0, r_scr0 ; num_idx = 0; carry = 0;
    inc r_arg1
    inc r_arg1
    inc r_arg0
sub_loop_1:
    dec r_arg1
    dec r_arg0
    jz sub_loop_2 ; if zero goto 2nd loop
    mov r_scr1, [r_arg2+8*r_scr0]
    sbb [r_arg3+8*r_scr0], r_scr1
    inc r_scr0
    jmp sub_loop_1
sub_loop_2:
    dec r_arg1
    jz sub_end
    sbb qword [r_arg3+8*r_scr0], 0
    inc r_scr0
    jmp sub_loop_2
sub_end:
    add rsp, 32
    ret

mul_asm:
    sub rsp, 32
    push r_ssc0
    push r_ssc1
    push r_ssc2
    push r_ssc3
    push r_nsc0
    ; vec     => r_arg0 RCX
    ; num1_sz => r_arg1 RDX
    ; num2_sz => r_arg2 R8
    ; r_arg3 R9 => num1_sz
    ; r_scr0 R10 => num_idx -- num2_sz
    ; r_scr1 R11 => num_idx_2 -- num1_sz
    ; r_ssc0 R12 => num1*
    ; r_ssc1 R13 => num2*
    ; r_ssc2 R14 => tmp
    ; r_ssc3 R15 => tmp_2 -- idx+idx_2
    ; r_ret RAX => tmp_and_mul
    ; this function uses r_mul1, so it cant use 
    ; r_arg1 nor r_arg2
    mov r_arg3, r_arg1
    mov r_ssc0, [r_arg0]
    mov r_ssc1, [r_arg0+8]
    mov r_scr0, r_arg2
loop_out:
    mov r_scr1, r_arg3
    dec r_scr0
    js mul_end
loop_in:
    dec r_scr1
    js loop_out
    mov r_ssc3, r_scr0
    add r_ssc3, r_scr1
    mov r_ret, [r_ssc1+8*r_scr0]
    mul qword [r_ssc0+8*r_scr1]
    jnc jump
    mov r_ssc2, [r_arg0+8*r_scr0+16]
    mov [r_ssc2+8*r_ssc3+8], r_mul1
jump:
    mov r_ssc2, [r_arg0+8*r_scr0+24]
    mov [r_ssc2+8*r_ssc3], r_ret
    jmp loop_in
mul_end:
    pop r_nsc0
    pop r_ssc3
    pop r_ssc2
    pop r_ssc1
    pop r_ssc0
    add rsp, 32
    ret

; div_asm:
;     ; num1_sz => RCX
;     ; num2_sz => RDX
;     ; num2*   => R8
;     ; num3*   => R9

;     ret

rsb_asm:
    sub rsp, 32
    ; num_sz  => r_arg0 RCX
    ; num     => r_arg1 RDX
    ; R8 => 
    ; R9 => 
    ; r_scr0 R10 => num_idx
    xor r_scr0, r_scr0 ; num_idx = 0; carry = 0;
    ; inc rcx
rsb_loop:
    dec r_arg0
    jz rsb_end ; if zero goto end
    rcr qword [r_arg1+8*r_arg0], 1
    jmp rsb_loop
rsb_end:
    rcr qword [r_arg1], 1
    add rsp, 32
    ret

inc_asm:
    sub rsp, 32
    ; num_sz  => r_arg0 RCX
    ; num     => r_arg1 RDX
    ; R8 => 
    ; R9 => 
    ; r_scr0 R10 => num_idx
    ; R11 => temp_guy
    xor r_scr0, r_scr0 ; num_idx = 0; carry = 0;
    stc
    inc r_arg0
inc_loop:
    jnc inc_end
    dec r_arg0
    jz inc_end ; if zero goto end
    adc qword [r_arg1+8*r_scr0], 0
    inc r_scr0
    jmp inc_loop
inc_end:
    add rsp, 32
    ret

dec_asm:
    sub rsp, 32
    ; num_sz  => r_arg0 RCX
    ; num     => r_arg1 RDX
    ; R8 => 
    ; R9 => 
    ; r_scr0 R10 => num_idx
    ; R11 => temp_guy
    xor r_scr0, r_scr0 ; num_idx = 0; carry = 0;
    stc
    inc r_arg0
dec_loop:
    jnc dec_end
    dec r_arg0
    jz dec_end ; if zero goto end
    sbb qword [r_arg1+8*r_scr0], 0
    inc r_scr0
    jmp dec_loop
dec_end:
    add rsp, 32
    ret

sbl_asm:
    sub rsp, 32
    ; num_sz  => r_arg0 RCX
    ; num     => r_arg1 RDX
    ; R8 => 
    ; R9 => 
    ; r_scr0 R10 => num_idx
    ; R11 => temp_guy
    xor r_scr0, r_scr0 ; num_idx = 0; carry = 0;
    ; inc rcx
sbl_loop:
    dec r_arg0
    jz sbl_end ; if zero goto end
    rcl qword [r_arg1+8*r_scr0], 1
    inc r_scr0
    jmp sbl_loop
sbl_end:
    rcl qword [r_arg1+8*r_scr0], 1
    add rsp, 32
    ret