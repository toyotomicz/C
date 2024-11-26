.386
_TEXT		SEGMENT BYTE PUBLIC 'CODE'
			ASSUME CS:_TEXT

			public  @swap@8

@swap@8		proc    near
			jmp		@L1
			db		'XXXXX'
@L1:
			mov		eax, [edx]
			mov		ebx, [ecx]
			mov		[edx], ebx
			mov		[ecx], eax
			ret
@swap@8		endp

_TEXT		ends

END
