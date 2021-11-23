#include <18f452.h>
#use delay (crystal=20MHz) // preprocessor directive (specifies clock type and speed)
#use rs232 (baud=9600,parity=N,xmit=PIN_C6,rcv=PIN_C7,bits=8) 

unsigned int8 ACK=0;
unsigned int8 dataRX=10;
unsigned int8 autobaud=0x55;//Letter U = 0x55 (Autobaud)
unsigned int8 clearScreen=0x45;//Clear the screen
//Background Color
unsigned int16 ReplaceBackgroundColor=0x42;//B(ascii) Value to replace background color
//Red
unsigned int16 red1=0xf0;
unsigned int16 red2=0x00;
//Green 
unsigned int16 green1=0xff;
unsigned int16 green2=0xff;
//Blue
unsigned int16 blue1=0x0c;
unsigned int16 blue2=0xff;


/*
   %c => print a Value 
   %d,%i... => print a Character
*/





// INTERUPTIONS 
#INT_RDA
void interupt()
{
   if(kbhit()) //Return true when a character is received in the buffer (Buffer is full)
   {
      dataRX=getc();
      
      if (dataRX==0x06)//ACK = good return 
      {
         ACK=1;  
      }
      
   }
}


// PROGRAMME PRINCIPAL //
void main()

{

   enable_interrupts(GLOBAL);
   enable_interrupts(INT_RDA);
   
   
   output_LOW(PIN_C5); //Mise à 0 de la pin Reset
   delay_ms(500);// Delay d'initialisation minimum à respecter
   output_HIGH(PIN_C5);// Mise à 1 de la pin Reset
   delay_ms(1500);// Attente de fin d'initialisation
   
   printf("%c",autobaud);//Envoi de la commande Autobaud (Set the rate)
   delay_ms(1000);
   while(ACK!=1); ACK = 0; // To check if the buffer is in the next step
   
      
   printf("%c",clearScreen); //Efface l'écran 
    while(ACK!=1); ACK = 0; // To check if the buffer is in the next step
   
   
   
   while(True)
   {
      printf("%c%c%c",replaceBackgroundColor,red1,red2);
       while(ACK!=1); ACK = 0; //Interupts
      
      delay_ms(1000);
      
      printf("%c%c%c",replaceBackgroundColor,green1,green2);
       while(ACK!=1); ACK = 0; // Interupts
        delay_ms(1000);

      printf("%c%c%c",replaceBackgroundColor,blue1,blue2);
       while(ACK!=1); ACK = 0; // Interupts
        delay_ms(1000);   
   }
   
}
