            .text
            .global swap
swap:
            mov     (%rdi),%eax
            mov     (%rsi),%edx
            mov     %edx,(%rdi)
            mov     %eax,(%rsi)
            ret
