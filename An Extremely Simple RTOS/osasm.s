	.syntax unified
	.macro             FUNCTION name                /* this macro makes life less tedious. =) */
	.func              \name,\name                  /* this tells a debugger that the function starts here */
	.type              \name,%function              /* when a function is pointed to from a table, this is mandatory */
	.thumb_func                                     /* when a function is called by using 'bx' or 'blx' this is mandatory */
	.align                                          /* make sure the address is aligned for code output */
\name\():                                                           /* this defines the label. the \() is necessary to separate the colon from the label */
	.endm
 
	.macro             ENDFUNC name                 /* FUNCTION and ENDFUNC must always be paired */
	.size              \name, . - \name             /* tells the linker how big the code block for the function is */
	.pool                                           /* let the assembler place constants here */
	.endfunc                                        /* mark the end of the function, so a debugger can display it better */
	.endm
                    

.global EnableInterrupts
.global DisableInterrupts
.global SysTick_Handler
.global StartOS
.global StartCritical
.global EndCritical


FUNCTION DisableInterrupts
        CPSID   I
        BX      LR
ENDFUNC DisableInterrupts


FUNCTION EnableInterrupts
        CPSIE   I
        BX      LR
ENDFUNC EnableInterrupts


FUNCTION		SysTick_Handler    // 1) Saves R0-R3,R12,LR,PC,PSR
	CPSID   I                  // 2) Prevent interrupt during switch
    PUSH    {R4-R11}           // 3) Save remaining regs r4-11
    LDR     R0, =RunPt         // 4) R0=pointer to RunPt, old thread
    LDR     R1, [R0]           //    R1 = RunPt
    STR     SP, [R1]           // 5) Save SP into TCB
    LDR     R1, [R1,#4]        // 6) R1 = RunPt->next
    STR     R1, [R0]           //    RunPt = R1
    LDR     SP, [R1]           // 7) new thread SP; SP = RunPt->sp;
    POP     {R4-R11}           // 8) restore regs r4-11
    CPSIE   I                  // 9) tasks run with interrupts enabled
    BX      LR                 // 10) restore R0-R3,R12,LR,PC,PSR
ENDFUNC SysTick_Handler


FUNCTION StartOS
    LDR     R0, =RunPt         //; currently running thread
    LDR     R2, [R0]           //; R2 = value of RunPt
    LDR     SP, [R2]           //; new thread SP; SP = RunPt->stackPointer;
    POP     {R4-R11}           //; restore regs r4-11
    POP     {R0-R3}            //; restore regs r0-3
    POP     {R12}
    POP     {LR}               //; discard LR from initial stack
    POP     {LR}               //; start location
    POP     {R1}               //; discard PSR
    CPSIE   I                  //; Enable interrupts at processor level
    BX      LR                 //; start first thread
ENDFUNC StartOS


FUNCTION StartCritical
        MRS    R0, PRIMASK  //; save old status
        CPSID  I            //; mask all (except faults)
        BX     LR
ENDFUNC StartCritical


FUNCTION EndCritical
        MSR    PRIMASK, R0
        BX     LR
ENDFUNC EndCritical

    .align
