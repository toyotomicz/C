            .text
            .global @swap@8
@swap@8:
            mov     (%ecx), %eax
            mov     (%edx), %ebx
            mov     %ebx, (%ecx)
            mov     %eax, (%edx)
            ret
