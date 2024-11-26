            .text
            .global swap
swap:
            mov     (%rcx), %eax
            mov     (%rdx), %ebx
            mov     %ebx, (%rcx)
            mov     %eax, (%rdx)
            ret
