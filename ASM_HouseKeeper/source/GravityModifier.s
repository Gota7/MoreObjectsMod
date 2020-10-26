@ Runs when horzSpeed == 0_f
@ r1, r3, r4 and r5 are safe to use here
nsub_02010c74:
    ldr   r1,[r0,#0x9c]
    ldr   r3, =gravityMultiplier
    ldr   r3,[r3]
    mov   r4, #0x800
    mov   r5, #0
    smlal r4, r5, r1, r3
    mov   r4, r4, lsr #12
    orr   r1, r4, r5, lsl #20
    b     0x02010c78

@ Runs when horzSpeed != 0_f
@ r1, r4, r7 and r14 are safe to use here
nsub_02010cc4:
    ldr   r1,[r0,#0x9c]
    ldr   r7, =gravityMultiplier
    ldr   r7,[r7]
    mov   r4, #0x800
    mov   r14, #0
    smlal r4, r14, r1, r7
    mov   r4, r4, lsr #12
    orr   r4, r4, r14, lsl #20
    b     0x02010cc8