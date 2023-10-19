#include <msp430.h>

#define SEGMENT_A   BIT1
#define SEGMENT_B   BIT2
#define SEGMENT_C   BIT3
#define SEGMENT_D   BIT4
#define SEGMENT_E   BIT5
#define SEGMENT_F   BIT6
#define SEGMENT_G   BIT7

#define ROSU_M BIT0
#define GALBEN_M BIT1
#define VERDE_M BIT2
#define ROSU_P BIT3
#define VERDE_P BIT4

#define BUZZER  BIT1
volatile unsigned int i = 0;
volatile int LED_ROSU_APRINS = 0;
volatile int LED_GALBEN_APRINS = 0;
volatile int LED_VERDE_APRINS = 0;
unsigned char LED[4]={'L','E','D',':'};
unsigned char ROSU[5]={'R','O','S','U',';'};
unsigned char GALBEN[7]={'G','A','L','B','E','N',';'};
unsigned char VERDE[6]={'V','E','R','D','E',';'};
unsigned char numar[10] = {'-', '1', '2', '3', '4', '5', '6', '7', '8', '9'};

void delay_ms(unsigned int ms) {
    unsigned int i;
    for (i = 0; i < ms; i++) {
        __delay_cycles(8000);
    }
}

int main(void) {
    WDTCTL = WDTPW | WDTHOLD;

    CSCTL3 |= SELREF__XT1CLK;                         // Set REFO as FLL reference source
    CSCTL0 = 0;                                        // clear DCO and MOD registers
    CSCTL1 |= DCORSEL_3;                               // Set DCO = 8MHz
    CSCTL2 = FLLD_0 + 243;                             // DCOCLKDIV = 8MHz
    __delay_cycles(3);
    __bic_SR_register(SCG0);                           // enable FLL
    while(CSCTL7 & (FLLUNLOCK0 | FLLUNLOCK1));         // FLL locked

    CSCTL4 = SELMS__DCOCLKDIV | SELA__REFOCLK;        // set default REFO(~32768Hz) as ACLK source, ACLK = 32768Hz
                                                      // default DCOCLKDIV as MCLK and SMCLK source
    CSCTL5 = DIVS_1;        //SCCLK=4MHz

    P5DIR |= ROSU_M | GALBEN_M | VERDE_M | ROSU_P | VERDE_P;
    P5OUT &= ~ROSU_M & ~GALBEN_M & ~VERDE_M & ~ROSU_P & ~VERDE_P;

    P3DIR |= BUZZER;
    P3OUT &= ~BUZZER;

    P6DIR |= BIT6;
    P6OUT &= ~BIT6;

    P1DIR |= SEGMENT_A | SEGMENT_B | SEGMENT_C | SEGMENT_D | SEGMENT_E | SEGMENT_F | SEGMENT_G;

    P2DIR &= ~BIT3;
    P2OUT |= BIT3;                          // Configure P2.3 as pulled-up
    P2REN |= BIT3;                          // P2.3 pull-up register enable
    P2IES &= ~BIT3;                          // P2.3 Hi/Low edge
    P2IE |= BIT3;                           // P2.3 interrupt enabled
    P2IFG &= ~BIT3;


    PM5CTL0 &= ~LOCKLPM5;

    P4SEL0 |= BIT2 | BIT3;
    UCA1CTLW0 |= UCSWRST;
    UCA1CTLW0 |= UCSSEL_2;
    //Rata Transfer = 9600bps
    //4.000.000 / 9600 = 416.66
    //N > 16
    //OS16 = 1
    //UCBRx = INT(416.66/16) = 26.04125
    //UCBRx = 26
    //UCBRFX = INT[(26.0125-26)*16] = INT(0.0125 * 16) = INT(0.64) = 5
    //UCBRFx= 5;
    //0.7777 -> UCBRSx = 0x07861 -> UCBRSx = 0xED
    //OS16 = 1;
    UCA1BR0 = 26;                            // INT(4.000.000/9600)
    UCA1BR1 = 0x00;
    UCA1MCTLW = 0xB601;
    UCA1CTLW0 &= ~UCSWRST;
    UCA1IE |= UCRXIE;



    __bis_SR_register(GIE);


    while (1) {
        P1OUT |= SEGMENT_A | SEGMENT_B | SEGMENT_C | SEGMENT_D | SEGMENT_F | SEGMENT_G;
        P1OUT &= ~SEGMENT_E;
        P5OUT ^= VERDE_M;
        P5OUT ^= ROSU_P;
        UCA1TXBUF = numar[9];
        LED_VERDE_APRINS = 1;
        LED_ROSU_APRINS = 1;
        delay_ms(1000);
        P1OUT |= SEGMENT_A | SEGMENT_B | SEGMENT_C | SEGMENT_D | SEGMENT_E | SEGMENT_F | SEGMENT_G;
        UCA1TXBUF = numar[8];
        delay_ms(1000);
        P1OUT |= SEGMENT_A | SEGMENT_B | SEGMENT_C;
        P1OUT &= ~SEGMENT_D & ~SEGMENT_E & ~SEGMENT_F & ~SEGMENT_G ;
        UCA1TXBUF = numar[7];
        delay_ms(1000);
        P1OUT |= SEGMENT_A | SEGMENT_C | SEGMENT_D | SEGMENT_E | SEGMENT_F | SEGMENT_G;
        P1OUT &= ~SEGMENT_B;
        UCA1TXBUF = numar[6];
        delay_ms(1000);
        P1OUT |= SEGMENT_A | SEGMENT_C | SEGMENT_D | SEGMENT_F | SEGMENT_G;
        P1OUT &= ~SEGMENT_B & ~SEGMENT_E;
        UCA1TXBUF = numar[5];
        delay_ms(1000);
        P5OUT &= ~VERDE_M;


        P1OUT |= SEGMENT_B | SEGMENT_C | SEGMENT_F | SEGMENT_G;
        P1OUT &= ~SEGMENT_A & ~SEGMENT_D & ~SEGMENT_E;
        UCA1TXBUF = numar[4];
        P5OUT ^= GALBEN_M;
        LED_GALBEN_APRINS = 1;
        LED_ROSU_APRINS = 1;
        delay_ms(1000);
        P1OUT |= SEGMENT_A | SEGMENT_B | SEGMENT_C | SEGMENT_D | SEGMENT_G;
        P1OUT &= ~SEGMENT_E & ~SEGMENT_F;
        UCA1TXBUF = numar[3];
        delay_ms(1000);
        P1OUT |= SEGMENT_A | SEGMENT_B | SEGMENT_D | SEGMENT_E | SEGMENT_G;
        P1OUT &= ~SEGMENT_C & ~SEGMENT_F;
        UCA1TXBUF = numar[2];
        delay_ms(1000);
        P1OUT |= SEGMENT_B | SEGMENT_C;
        P1OUT &= ~SEGMENT_A & ~SEGMENT_D & ~SEGMENT_E & ~SEGMENT_F & ~SEGMENT_G;
        UCA1TXBUF = numar[1];
        delay_ms(1000);
        P5OUT &= ~GALBEN_M;
        P5OUT &= ~ROSU_P;
        P1OUT |= SEGMENT_B | SEGMENT_C;
        P1OUT &= ~SEGMENT_A & ~SEGMENT_D & ~SEGMENT_E & ~SEGMENT_F & ~SEGMENT_G;
        UCA1TXBUF = numar[0];
        delay_ms(1000);
        P5OUT ^= ROSU_M;
        P5OUT ^= VERDE_P;
        LED_ROSU_APRINS = 1;
        LED_VERDE_APRINS = 1;
        P3OUT |= BUZZER;
        P1OUT |= SEGMENT_G;
        UCA1TXBUF = numar[0];
        P1OUT &= ~SEGMENT_A & ~SEGMENT_B & ~SEGMENT_C & ~SEGMENT_D & ~SEGMENT_E & ~SEGMENT_F;
        delay_ms(9000);
        P5OUT &= ~ROSU_M;
        P5OUT &= ~VERDE_P;
        P3OUT &= ~BUZZER;

    }


}

#pragma vector=PORT2_VECTOR
__interrupt void Port_2(void)
{
    P4IFG &= ~BIT1;

    for (i = 0; i <= 3; i++) {
        while (!(UCA1IFG & UCTXIFG));
        UCA1TXBUF = LED[i];
    }
    if(LED_ROSU_APRINS==1){
        for (i = 0; i <= 4; i++) {
            while (!(UCA1IFG & UCTXIFG));
            UCA1TXBUF = ROSU[i];
        }
        LED_ROSU_APRINS=0;

    }
    if(LED_GALBEN_APRINS==1){
        for (i = 0; i <= 6; i++) {
            while (!(UCA1IFG & UCTXIFG));
            UCA1TXBUF = GALBEN[i];
        }
        LED_GALBEN_APRINS=0;

    }
    if(LED_VERDE_APRINS==1){
        for (i = 0; i <= 5; i++) {
            while (!(UCA1IFG & UCTXIFG));
            UCA1TXBUF = VERDE[i];
        }
        LED_VERDE_APRINS=0;
    }


}
