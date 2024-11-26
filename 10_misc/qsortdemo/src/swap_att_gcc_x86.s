            .text
            .global swap
swap:
            mov     (%edx), %eax
            mov     (%ecx), %ebx
            mov     %ebx, (%edx)
            mov     %eax, (%ecx)
            ret
