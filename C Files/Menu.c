#include <18f4620.h>
#use delay(crystal=20Mhz)
#fuses NOPROTECT

#use rs232(baud=9600, PARITY=N, BITS=8, xmit=PIN_C6, rcv=PIN_C7,stream=TRAME)


unsigned int8 ACK = 0;
unsigned int16 dataRX=0;
unsigned int8 detectSc1=0;
unsigned int8 detectSc2=0;
unsigned int8 detectSc3=0;
unsigned int8 ledOFF=0;
unsigned int16 x,y;
unsigned int8 autobaud=0x55;//Letter U = 0x55 (Autobaud)
unsigned int8 clearScreen=0x45;//Clear screen command
unsigned int8 command=0x59;
unsigned int8 touchControl=0x05;
unsigned int8 enable=0x00;
int8 CORD[5]={0,0,0,0,0};

// INTERUPT
#INT_RDA                
void RS232_ACK()         
{                       
   if (kbhit())         //Return true when a character is received in the buffer (Buffer is full)
   {
      dataRX= getc();      //ACK = good return
      if (dataRX==0x06)
      {
         ACK=1;         //ACK devient l'image du port C en réponse booléenne
      }
      if (dataRX>21)//> à 0x15
      {
         int nbyte = 0;                 
         
            WHILE(nbyte<5)
            {               // réception des 4 octets de coordonnées
               CORD[nbyte] = getc();
               while(!kbhit());        // atente de nouvelle donnée
               nbyte++;
            }
         
      y = make16(CORD[0],CORD[1]);  // met les coordonnées dans un mot de 16 bits
      x = make16(CORD[2],CORD[3]);
         
         if  (y>64 && y<144) //COORD Button 1 
         {
            detectSc1=1;
            dataRX=0;
         }
         if (y>147 && y<223)//COORD Button 2
         {
            detectSc2=1;
            dataRX=0;
         }
         if (y>228 && y<304)//COORD Button 3
         {
            detectSc3=1;
            dataRX=0;
         }
         if (y<64)//COORD Black Rectangle
         {
            ledOFF=1;
            dataRX=0;
         }
      }
   }
}

//Main Program
/*
   %c => print a Value 
   %d,%i... => print a Character
*/

void main()
{
/*----------------------ENABLE INTERUPTS-------------------------*/
      enable_interrupts(GLOBAL);
      enable_interrupts(INT_RDA);
     
      /*----------------------INIT-------------------------*/   
      output_LOW(PIN_C5); //Set 0 on reset PIN
      delay_ms(500);// Init delay minimum to respect
      output_HIGH(PIN_C5);// Set 1 on reset PIN 
      delay_ms(1500);// Waiting of the end of the init
      printf("%c",autobaud);//Send Autobaud command (Set the rate)
      delay_ms(1000);
      while(ACK!=1); ACK = 0; // To check if the buffer is in the next step
      printf("%c%c%c",command,touchControl,enable);//Enable touch screen command - (Doc p.14)
      while(ACK!=1); ACK = 0; // To check if the buffer is in the next step
      printf("%c",clearScreen); //clear the screen
      while(ACK!=1); ACK = 0; // To check if the buffer is in the next step
  
   /*-----------------------BACKGROUND-------------------------*/
   printf("s%c%c%c%c%c%s%c",7,9,3,255,255,"WELCOME",0);//Welcome init
   while(ACK!=1); ACK = 0; // To check if the buffer is in the next step
   delay_ms(2000);
   printf("B%c%c",255,255); //changement de couleur
   while (ACK!=1);
   ACK=0;
   /*-----------------------DRAWINGS-------------------------*/   
   printf("C%c%c%c%c%c%c%c%c",0,126,0,105,0,35,240,0);  //Circle 1 
   while (ACK!=1);
   ACK=0;
   printf("C%c%c%c%c%c%c%c%c",0,126,0,185,0,35,15,0);  //Circle 2 
   while (ACK!=1);
   ACK=0;
   printf("C%c%c%c%c%c%c%c%c",0,126,1,9,0,35,0,240);  //Circle 3 
   while (ACK!=1);
   ACK=0;
   printf("r%c%c%c%c%c%c%c%c%c%c",0,0,0,0,0,239,0,48,0,0);  //Black rectangle 
   while (ACK!=1);
   ACK=0;
   /*-----------------------TEXT-------------------------*/
   //Select the scene
   printf("s%c%c%c%c%c%s%c",2,1,3,255,255,"Select the scene",0);
      while(ACK!=1); ACK = 0; // Interupts 
   //Scene 1
   printf("s%c%c%c%c%c%s%c",10,6,3,255,255,"1",0);
      while(ACK!=1); ACK = 0; // Interupts 
   //Scene 2
   printf("s%c%c%c%c%c%s%c",10,11,3,255,255,"2",0);
      while(ACK!=1); ACK = 0; // Interupts  
   //Scene 3
   printf("s%c%c%c%c%c%s%c",10,16,3,255,255,"3",0);
      while(ACK!=1); ACK = 0; // Interupts   
    
   
// TouchScreen
   printf("Y%c%c",5,0); //Enable touch screen
   while (ACK!=1);
   ACK=0;
   
   while (TRUE)
   {
    
      printf("%c%c",0x6F,0x01); // Get Touch coodinates
      while(detectSc1==1)
      {
         dataRX=0;
         output_d(1);
         detectSc1=0;
      }
      printf("%c%c",0x6F,0x01); // Get Touch coodinates
      while(detectSc2==1)
      {
         dataRX=0;

         output_d(2);
         detectSc2=0;
      }
      printf("%c%c",0x6F,0x01); // Get Touch coodinates
      while(detectSc3==1)
      {
         dataRX=0;
         output_d(4);

         detectSc3=0;
      }
      printf("%c%c",0x6F,0x01); // Get Touch coodinates
      while(ledOFF==1)
      {
         dataRX=0;
         output_d(0);
         ledOFF=0;
      } 

   }//End of the While(TRUE)
   
   
}//End of the main
