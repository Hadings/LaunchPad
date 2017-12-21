void Timer2A_countCapture_init(void)
{
    SYSCTL->RCGCTIMER |= 4; /* enable clock to Timer Block 2 */
    SYSCTL->RCGCGPIO |= 0x20; /* enable clock to PORTF */
    GPIOF->DIR &= ~0x10; /* make PF4 an input pin */
    GPIOF->DEN |= 0x18; /* make PF4 a digital pin */
    GPIOF->PUR = 0x10;
    GPIOF->AFSEL |= 0x10; /* enable alternate function on PF4 */
    GPIOF->PCTL &= ~0x000F0000; /* configure PF4 as T2CCP0 pin */
    GPIOF->PCTL |= 0x00070000;
    TIMER2->CTL &= ~1; /* disable TIMER2A during setup */
    TIMER2->CFG = 4; /* configure as 16-bit timer mode */
    TIMER2->TAMR = 0x13; /* up-count, edge-count, capture mode */
    TIMER2->TAMATCHR = 0xFFFF; /* set the count limit */
    TIMER2->TAPMR = 0xFF; /* to 0xFFFFFF with prescaler */
    TIMER2->CTL &= ~0xC; /* capture the rising edge */
    TIMER2->CTL |= 1; /* enable timer2A */
}
int Timer2A_countCapture(void)
{
    return TIMER2->TAR;
}
