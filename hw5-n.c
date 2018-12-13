/* 
A simple ventilation system
Manoosh Samiei
Student no. :94242070  
Microprocessor & computer structure course*/


#include <mega32.h>
#include <delay.h>
#include <alcd.h>
#include <stdio.h>
#include <stdlib.h>

unsigned char overflow;
unsigned char keypad[2];
char key;
char out,in1,in2,in3,sum,inside,dif1,dif2,dif3;


#define FIRST_ADC_INPUT 0
#define LAST_ADC_INPUT 3
unsigned int adc_data[LAST_ADC_INPUT-FIRST_ADC_INPUT+1];
#define ADC_VREF_TYPE 0x00

// ADC interrupt service routine
// with auto input scanning
interrupt [ADC_INT] void adc_isr(void)
{
static unsigned char input_index=0;
// Read the AD conversion result
adc_data[input_index]=ADCW;
// Select next ADC input
if (++input_index > (LAST_ADC_INPUT-FIRST_ADC_INPUT))
   input_index=0;
ADMUX=(FIRST_ADC_INPUT | (ADC_VREF_TYPE & 0xff))+input_index;
// Delay needed for the stabilization of the ADC input voltage
delay_us(10);
}


// Timer1 overflow interrupt service routine
interrupt [TIM1_OVF] void timer1_ovf_isr(void)
{
overflow++;
if(overflow==5){
      
out=adc_data[0]/4;
in1=adc_data[1]/4;
in2=adc_data[2]/4;
in3=adc_data[3]/4; }

}


void keyboard (void){   // keypad function
PORTC.0=0;
delay_ms(2);
if(PINC.4==0){key=7;}
if(PINC.5==0){key=8;}
if(PINC.6==0){key=9;}
PORTC.0=1;

PORTC.1=0;
delay_ms(2);
if(PINC.4==0){key=4;}
if(PINC.5==0){key=5;}
if(PINC.6==0){key=6;}
PORTC.1=1;

PORTC.2=0;
delay_ms(2);
if(PINC.4==0){key=1;}
if(PINC.5==0){key=2;}
if(PINC.6==0){key=3;}
if(PINC.7==0){key='-';}
PORTC.2=1;


PORTC.3=0;
delay_ms(2);
if(PINC.5==0){key=0;}
if(PINC.7==0){key='+';}
PORTC.3=1;
}

interrupt [EXT_INT2] void ext_int2_isr(void) //interrupt function
{
    #asm("cli");
    PORTD=0xFF;
    keyboard();
    PORTD=0xF0 ;
    #asm("sei");
}
       
void main(void)
{
char temp=23;
char key=20;
char j=0;
unsigned char str[20];
unsigned char str2[20];
char fan=1;
char heater=0;
// Declare your local variables here

// Input/Output Ports initialization
// Port A initialization
// Func7=In Func6=In Func5=In Func4=In Func3=In Func2=In Func1=In Func0=In 
// State7=T State6=T State5=T State4=T State3=T State2=T State1=T State0=T 
PORTA=0xFF;
DDRA=0x00;

// Port B initialization
// Func7=In Func6=In Func5=In Func4=In Func3=In Func2=In Func1=In Func0=In 
// State7=T State6=T State5=T State4=T State3=T State2=T State1=T State0=T 
PORTB=0x00;
DDRB=0x00;

// Port C initialization
// Func7=In Func6=In Func5=In Func4=In Func3=Out Func2=Out Func1=Out Func0=Out 
// State7=P State6=P State5=P State4=P State3=0 State2=0 State1=0 State0=0 
PORTC=0xF0;
DDRC=0x0F;

// Port D initialization
// Func7=Out Func6=Out Func5=Out Func4=Out Func3=Out Func2=Out Func1=Out Func0=Out 
// State7=0 State6=0 State5=0 State4=0 State3=0 State2=0 State1=0 State0=0 
PORTD=0x00;
DDRD=0xFF;

// Timer/Counter 0 initialization
// Clock source: System Clock
// Clock value: Timer 0 Stopped
// Mode: Normal top=0xFF
// OC0 output: Disconnected
TCCR0=0x00;
TCNT0=0x00;
OCR0=0x00;

// Timer/Counter 1 initialization
// Clock source: System Clock
// Clock value: 0.977 kHz
// Mode: Normal top=0xFFFF
// OC1A output: Discon.
// OC1B output: Discon.
// Noise Canceler: Off
// Input Capture on Rising Edge
// Timer1 Overflow Interrupt: On
// Input Capture Interrupt: Off
// Compare A Match Interrupt: Off
// Compare B Match Interrupt: Off
TCCR1A=0x00;
TCCR1B=0x45;
TCNT1H=0x00;
TCNT1L=0x00;
ICR1H=0x00;
ICR1L=0x00;
OCR1AH=0x00;
OCR1AL=0x00;
OCR1BH=0x00;
OCR1BL=0x00;

// Timer/Counter 2 initialization
// Clock source: System Clock
// Clock value: Timer2 Stopped
// Mode: Normal top=0xFF
// OC2 output: Disconnected
ASSR=0x00;
TCCR2=0x00;
TCNT2=0x00;
OCR2=0x00;

// External Interrupt(s) initialization
// INT0: Off
// INT1: Off
// INT2: On
// INT2 Mode: Rising Edge
GICR|=0x20;
MCUCR=0x00;
MCUCSR=0x40;
GIFR=0x20;
// Timer(s)/Counter(s) Interrupt(s) initialization
TIMSK=0x04;

// USART initialization
// USART disabled
UCSRB=0x00;


// ADC initialization
// ADC Clock frequency: 500.000 kHz
// ADC Voltage Reference: Int., cap. on AREF
ADMUX=FIRST_ADC_INPUT | (ADC_VREF_TYPE & 0xff);
ADCSRA=0xAB;
SFIOR&=0x1F;
SFIOR|=0xC0;
// SPI initialization
// SPI disabled
SPCR=0x00;

// TWI initialization
// TWI disabled
TWCR=0x00;

// Alphanumeric LCD initialization
// Connections are specified in the
// Project|Configure|C Compiler|Libraries|Alphanumeric LCD menu:
// RS - PORTD Bit 0
// RD - PORTD Bit 1
// EN - PORTD Bit 2
// D4 - PORTD Bit 4
// D5 - PORTD Bit 5
// D6 - PORTD Bit 6
// D7 - PORTD Bit 7
// Characters/line: 20
lcd_init(20);
lcd_clear();

#asm("sei")

while (1)
      {
      
      if(key!=20){
       
      if(key!='-' && key!='+'){
      keypad[j]=key;
      j++;
      if(j==2){
      j=0;
      temp=10*keypad[0]+keypad[1];}}
      
      else if(key=='+')
      temp++;
      
      else if(key=='-')
      temp--;}   


      sum=in1+in2+in3;
      inside=sum/3;
      if(temp-heater>=1){
      heater=1;}
      
      if(heater-temp<=1){
      heater=0;}
      
      dif1=cabs(in1-in2);
      dif2=cabs(in1-in3);
      dif3=cabs(in2-in3);
      
      if((dif1>2)||(dif2>2)||(dif3>2)){
      fan=2;}   
      
      lcd_gotoxy(0,0);
      sprintf(str,"outside:%d inside:%d",out , inside); 
      lcd_puts(str);
      
      lcd_gotoxy(0,1);
      sprintf(str2,"Fan:%d  temp:%d" ,fan , temp);
      lcd_puts(str2); 
      

        }   
        
}
