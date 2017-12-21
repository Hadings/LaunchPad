/*PF1 - red LED, PF2 - blue LED, PF3 - green LED */
/* PORTF data register */
#define PORTFDAT (*((volatile unsigned int*)0x400253FC))
/* PORTF data direction register */
#define PORTFDIR (*((volatile unsigned int*)0x40025400))
/* PORTF digital enable register */
#define PORTFDEN (*((volatile unsigned int*)0x4002551C))
/* run mode clock gating register */
#define RCGCGPIO (*((volatile unsigned int*)0x400FE608))
void delayMs(int n); /* function prototype for delay */

int main(void)
{
 RCGCGPIO |= 0x20; /* enable clock to GPIOF at clock gating register */
 PORTFDIR = 0x0E; /* set PORTF pin3-1 as output pins */
 PORTFDEN = 0x0E; /* set PORTF pin3-1 as digital pins */
 while(1)
 {
 PORTFDAT = 0x0E; /* write PORTF to turn on all LEDs */
 delayMs(1000000);
 PORTFDAT = 0; /* write PORTF to turn off all LEDs */
 delayMs(1000000);
 }
}
void delayMs(int n)
{
 volatile int i;
 for(i = 0 ; i < n; i++) {}
}
