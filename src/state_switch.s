	.arch msp430g2553
	.p2align 1, 0

	.data
	.extern state


jt:
	.word case0
	.word case1
	.word case2
	.word case3

	.text
	.global change_state

change_state:
	cmp	#4, r12
	jc	default
	add	r12, r12
	mov	jt(r12), r0
case0:
	mov #0, &state
	jmp default
case1:
	mov #1, &state
	jmp default
case2:
	mov #2, &state
	jmp default
case3:
	mov #3, &state
	jmp default
default:
	ret
