@Disable drawing distances

repl_020110ec:
	ble		0x0201111c
	ldr		r0, =twoDeeOn
	ldrb	r0, [r0]
	cmp		r0, #0
	bne		0x0201111c
	bx		lr

@Ignore the Z coordinate of camSpacePos in 4 sound player functions:

@PlayCharVoice
nsub_02012164:
	ldr		r2, =camSpacePos2
	add		r1, r1, r3
	b		0x02012168

@PlayLong
nsub_02012330:
	ldr		r3, =camSpacePos2
	mov		r5, r3
	b		0x02012334


@Play
nsub_02012598:
	ldr		r2, =camSpacePos2
	mov		r7, r0
	b		0x0201259c


@PlayBank0
nsub_02012654:
	ldr	r2, =camSpacePos2
	mov	r1, r0
	b		0x02012658

@ Fix the glitch that would happen when pressing L in 2D

nsub_0200a5b4:
	beq		0x0200a5b8
	ldr 	r0, =twoDeeOn
	ldrb	r0, [r0]
	cmp 	r0, #0
	beq		0x0200A5F8
	b 		0x0200a5b8

nsub_0200A618:
	beq		0x0200A61c
	ldr 	r0, =twoDeeOn
	ldrb	r0, [r0]
	cmp 	r0, #0
	beq		0x0200A628
	b 		0x0200A61c

nsub_0200A62c:
	beq		0x0200A630
	ldr 	r0, =twoDeeOn
	ldrb	r0, [r0]
	cmp 	r0, #0
	beq 	0x0200A640
	b 		0x0200A630
