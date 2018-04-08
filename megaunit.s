global add_asm
global sub_asm
global mul_asm
; global div_asm

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
    sub rsp, 32
    push r12
    push r13
    push r14
    push r15
    ; vec     => RCX
    ; num1_sz => RDX
    ; num2_sz => R8
    ; R9 => num1_sz
    ; R10 => num_idx -- num2_sz
    ; R11 => num_idx_2 -- num1_sz
    ; R12 => num1*
    ; R13 => num2*
    ; RAX => tmp_and_mul
    ; R14 => tmp
    ; R15 => tmp_2 -- idx+idx_2
    mov r9, rdx
    mov r12, [rcx]
    mov r13, [rcx+8]
    mov r10, r8
loop_out:
    mov r11, r9
    dec r10
    js mul_end
loop_in:
    dec r11
    js loop_out
    mov r15, r10
    add r15, r11
    mov rax, [r13+8*r10]
    mul qword [r12+8*r11]
    jnc jump
    mov r14, [rcx+8*r10+16]
    mov [r14+8*r15+8], rdx
jump:
    mov r14, [rcx+8*r10+24]
    mov [r14+8*r15], rax
    jmp loop_in
mul_end:
    pop r15
    pop r14
    pop r13
    pop r12
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

sub_asm:
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
sub_loop:
    jnc sub_end
    dec rcx
    jz sub_end ; if zero goto end
    sbb [rdx+8*r10], byte 0
    inc r10
    jmp sub_loop
sub_end:
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