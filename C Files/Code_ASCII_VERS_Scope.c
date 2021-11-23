#include <18f452.h>
#use delay (crystal=20MHz) // preprocessor directive (specifies clock type and speed)
#use rs232 (baud=9600,parity=N,xmit=PIN_C6,rcv=PIN_C7,bits=8) 


unsigned int16 TEMPO=200;
 unsigned int8 a=64;


// PROGRAMME PRINCIPAL //
void main()

{

   while (true)
   {
      printf("%d",a);
      //printf("%u",a);
      delay_ms(TEMPO);
   }

}
