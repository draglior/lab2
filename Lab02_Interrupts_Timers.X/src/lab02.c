#include "lab02.h"

#include <xc.h>
#include <p33Fxxxx.h>
//do not change the order of the following 2 definitions
#define FCY 12800000UL
#include <libpic30.h>

#include "types.h"
#include "lcd.h"
#include "led.h"

#define FCY_EXT 32768


uint16_t seconds = 0;
uint16_t milliseconds = 0;
uint16_t minutes = 0;

void initialize_timer()
{
    // Enable RTC Oscillator -> this effectively does OSCCONbits.LPOSCEN = 1
    // but the OSCCON register is lock protected. That means you would have to 
    // write a specific sequence of numbers to the register OSCCONL. After that 
    // the write access to OSCCONL will be enabled for one instruction cycle.
    // The function __builtin_write_OSCCONL(val) does the unlocking sequence and
    // afterwards writes the value val to that register. (OSCCONL represents the
    // lower 8 bits of the register OSCCON)
    __builtin_write_OSCCONL(OSCCONL | 2);
    
    //Timer1
    // Disable the Timers
    CLEARBIT(T1CONbits.TON);
    // Set Prescaler
    T1CONbits.TCKPS=0b11;
    // Set Clock Source
    T1CONbits.TCS=1; //external 32kHz
    // Set Gated Timer Mode -> don't use gating
    CLEARBIT(T1CONbits.TGATE);
    // T1: Set External Clock Input Synchronization -> no sync
    //T1CONbits.TSYNC=0;
    // Load Timer Periods
    PR1=125;
    // Reset Timer Values
    TMR1=0x00;
    // Set Interrupt Priority
    IPC0bits.T1IP = 0x01;
    // Clear Interrupt Flags
    IFS0bits.T1IF = 0;
    // Enable Interrupts
    SETBIT(IEC0bits.T1IE);
    // Enable the Timers
    SETBIT(T1CONbits.TON);
    
    //================================
    
    //Timer 2
    // Disable the Timers
    CLEARBIT(T2CONbits.TON);
    // Set Prescaler
    T2CONbits.TCKPS=0b11;
    // Set Clock Source
    T2CONbits.TCS=0; //internal 12.8MHz
    // Set Gated Timer Mode -> don't use gating
    CLEARBIT(T2CONbits.TGATE);
    // T1: Set External Clock Input Synchronization -> no sync
    //T1CONbits.TSYNC=0;
    // Load Timer Periods
    PR2=100;
    // Reset Timer Values
    TMR2=0x00;
    // Set Interrupt Priority
    IPC1bits.T2IP = 0x01;
    // Clear Interrupt Flags
    IFS0bits.T2IF = 0;
    // Enable Interrupts
    SETBIT(IEC0bits.T2IE);
    // Enable the Timers
    SETBIT(T2CONbits.TON);
    
    //================================
    
    //Timer 3
    // Disable the Timers
    CLEARBIT(T3CONbits.TON);
    // Set Prescaler
    T3CONbits.TCKPS=0b00;
    // Set Clock Source
    T3CONbits.TCS=0; //internal 12.8MHz
    // Set Gated Timer Mode -> don't use gating
    CLEARBIT(T3CONbits.TGATE);
    // T1: Set External Clock Input Synchronization -> no sync
    //T1CONbits.TSYNC=0;
    // Load Timer Periods
    PR3=65536; //2^16
    // Reset Timer Values
    TMR3=0x00;
    // Set Interrupt Priority
    IPC2bits.T3IP = 0x01;
    // Clear Interrupt Flags
    IFS0bits.T3IF = 0;
    // Enable Interrupts
    SETBIT(IEC0bits.T3IE);
    // Enable the Timers
    SETBIT(T3CONbits.TON);
    
}

void timer_loop()
{
    
    // print assignment information
    lcd_printf("Lab02: Int & Timer");
    lcd_locate(0, 1);
    lcd_printf("Group: BluePinneapple");
    while(TRUE)
    {
        lcd_locate(0, 5);    
        lcd_printf("%02u:%02u.%03u\r\n" , minutes, seconds, milliseconds);  
    }
}

void __attribute__((__interrupt__, __shadow__, __auto_psv__)) _T1Interrupt(void)
{ // invoked every ??
    TOGGLEBIT(LED2_PORT);
    seconds++;
    if (seconds > 60){ 
            seconds = 0;
            minutes++;}
    
    IFS0bits.T1IF = 0;

}

void __attribute__((__interrupt__, __shadow__, __auto_psv__)) _T2Interrupt(void)
{
    TOGGLEBIT(LED1_PORT);
    milliseconds = milliseconds + 2; //considering the 2ms
    if (milliseconds > 999) 
            milliseconds = 0;
    
    IFS0bits.T2IF = 0;
}

void __attribute__((__interrupt__, __shadow__, __auto_psv__)) _T3Interrupt(void)
{
    IFS0bits.T3IF = 0;
}