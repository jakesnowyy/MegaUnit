global add_asm
global sub_asm
global mul_asm
global div_asm

global rsb_asm
global sbl_asm

global inc_asm

section .text
bits 64
default rel

add_asm:
    sub rsp, 32
    ; num2_sz => RCX -- becames loop_1_counter
    ; num3_sz => RDX -- becames loop_2_counter
    ; num2*   => R8
    ; num3*   => R9
    ; R10 => num_idx
    ; R11 => temp_guy
    xor r10, r10 ; num_idx = 0; carry = 0;
    inc rdx
    inc rdx
    inc rcx
add_loop_1:
    dec rdx
    dec rcx
    jz add_loop_2 ; if zero goto 2nd loop
    mov r11, [r8+8*r10]
    adc [r9+8*r10], r11
    inc r10
    jmp add_loop_1
add_loop_2:
    dec rdx
    jz add_end
    adc [r9+8*r10], byte 0
    inc r10
    jmp add_loop_2
add_end:
    add rsp, 32
    ret

sub_asm:
    sub rsp, 32
    ; num2_sz => RCX -- becames loop_1_counter
    ; num3_sz => RDX -- becames loop_2_counter
    ; num2*   => R8
    ; num3*   => R9
    ; R10 => num_idx
    ; R11 => temp_guy
    xor r10, r10 ; num_idx = 0; carry = 0;
    inc rdx
    inc rdx
    inc rcx
sub_loop_1:
    dec rdx
    dec rcx
    jz sub_loop_2 ; if zero goto 2nd loop
    mov r11, [r8+8*r10]
    sbb [r9+8*r10], r11
    inc r10
    jmp sub_loop_1
sub_loop_2:
    dec rdx
    jz sub_end
    sbb [r9+8*r10], byte 0
    inc r10
    jmp sub_loop_2
sub_end:
    add rsp, 32
    ret

mul_asm:
    ; num1_sz => RCX
    ; num2_sz => RDX
    ; num2*   => R8
    ; num3*   => R9

    ret

div_asm:
    ; num1_sz => RCX
    ; num2_sz => RDX
    ; num2*   => R8
    ; num3*   => R9

    ret

rsb_asm:
    sub rsp, 32
    ; num_sz  => RCX
    ; num     => RDX
    ; R8 => 
    ; R9 => 
    ; R10 => num_idx
    ; R11 => temp_guy
    xor r10, r10 ; num_idx = 0; carry = 0;
    ; inc rcx
rsb_loop:
    dec rcx
    jz rsb_end ; if zero goto end
    rcr qword [rdx+8*rcx], 1
    jmp rsb_loop
rsb_end:
    rcr qword [rdx], 1
    add rsp, 32
    ret

inc_asm:
    sub rsp, 32
    ; num_sz  => RCX
    ; num     => RDX
    ; R8 => 
    ; R9 => 
    ; R10 => num_idx
    ; R11 => temp_guy
    xor r10, r10 ; num_idx = 0; carry = 0;
    stc
    ; inc rcx
inc_loop:
    jnc inc_end
    dec rcx
    jz inc_end ; if zero goto end
    adc [rdx+8*r10], byte 0
    inc r10
    jmp inc_loop
inc_end:
    add rsp, 32
    ret

sbl_asm:
    sub rsp, 32
    ; num_sz  => RCX
    ; num     => RDX
    ; R8 => 
    ; R9 => 
    ; R10 => num_idx
    ; R11 => temp_guy
    xor r10, r10 ; num_idx = 0; carry = 0;
    ; inc rcx
sbl_loop:
    dec rcx
    jz sbl_end ; if zero goto end
    rcl qword [rdx+8*r10], 1
    inc r10
    jmp sbl_loop
sbl_end:
    rcl qword [rdx+8*r10], 1
    add rsp, 32
    ret