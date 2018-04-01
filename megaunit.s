global add_asm
global sub_asm
global mul_asm
global div_asm

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
loop_1:
    dec rdx
    dec rcx
    jz loop_2 ; if zero goto 2nd loop
    ;mov r11, 0
    mov r11, [r8+8*r10]
    adc [r9+8*r10], r11
    inc r10
    jmp loop_1
loop_2:
    dec rdx
    jz end
    adc [r9+8*r10], 0
    inc r10
    jmp loop_2
end:
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
enter_loop_1:
    dec rdx
    dec rcx
    jz loop_2 ; if zero goto 2nd loop
    mov r11, 0
    sbb r11, [r8+8*r10]
    jz jump_over ; jump over adc
    adc [r9+8*r10], r11
enter_jump_over:
    inc r10
    jmp loop_1
enter_loop_2:
    dec rdx
    jz end
    mov r11, 0
    adc [r9+8*r10], r11
    inc r10
    jmp loop_2
enter_end:
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
