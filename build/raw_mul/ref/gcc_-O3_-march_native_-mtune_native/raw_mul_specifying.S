.intel_syntax noprefix
.section .text
.global word3_muladd
.align 8


/;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
//;
//; void word3_muladd(uint64_t* c2, uint64_t* c1, uint64_t* c0, uint64_t a, uint64_t b);
//; c2->rdi, c1->rsi, c0->rdx, a->rcx, b->r8
//;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
word3_muladd:
	endbr64
	push   rbp
	mov    rbp,rsp
	push r9
	push r11
	push r12
	push r13
	push r14
	push r15

	
	mov r9, rdx
	mov r15, qword ptr [rdi]
	mov r14, qword ptr [rsi]
	mov r13, qword ptr [r9]

	mov rdx, rcx 
	mulx r12, r11, r8
	add r13, r11
	adcx r14, r12
	adc r15, 0

	mov qword ptr [rdi], r15
	mov qword ptr [rsi], r14
	mov qword ptr [r9], r13

	pop r15
	pop r14
	pop r13
	pop r12
	pop r11
	pop r9
	leave
	ret
