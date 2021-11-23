#include <18f452.h>
#use delay (crystal=20MHz) // preprocessor directive (specifies clock type and speed)
#use rs232 (baud=9600,parity=N,xmit=PIN_C6,rcv=PIN_C7,bits=8) 

unsigned int16 TEMPO=1000;
unsigned int8 autobaud=0x55;//Letter U = 0x55 (Autobaud)
unsigned int8 clearScreen=0x45;//Clear the screen

// PROGRAMME PRINCIPAL //
void main()

{
   output_LOW(PIN_C5); //Mise à 0 de la pin Reset
   delay_ms(500);// Delay d'initialisation minimum à respecter
   output_HIGH(PIN_C5);// Mise à 1 de la pin Reset
   delay_ms(1500);// Attente de fin d'initialisation
   
   printf("%c",autobaud);//Envoi de la commande Autobaud (Set the rate)
   delay_ms(1000);//
   
   printf("%c",clearScreen); //Efface l'écran 
   delay_ms(1000);
   
   while(True)
   {
   
   printf("B%c%c",0b11111111,0b11111111); //Affichage Ecran blanc 
   }
   
}
