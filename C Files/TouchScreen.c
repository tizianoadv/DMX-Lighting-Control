#include <18f4620.h>
#use delay (crystal=20MHz) // preprocessor directive (specifies clock type and speed)
#use rs232 (baud=9600,parity=N,xmit=PIN_C6,rcv=PIN_C7,bits=8) 

//Init Screen
unsigned int8 ACK=0;
unsigned int8 dataRX=0;
unsigned int8 autobaud=0x55;//Letter U = 0x55 (Autobaud)
unsigned int8 clearScreen=0x45;//Clear the screen
unsigned int8 displayControl=0x59;//Display controls
unsigned int8 touchControl=0x05;//Touch controls command
unsigned int8 enableTouchScreen=0x00;//Enable the touchScreen command
unsigned int8 setTouchRegion=0x75;//Detect the touch Region
unsigned int8 commandTouchScreen=0x6f;//Commands Get touch coordinates (P.47)
unsigned int8 touchActivity=0x04;//Get touch activity
unsigned int16 x,y;//Touch coordinates
unsigned int16 CORD[5]= {0,0,0,0,0};//Array
unsigned int8 greenButton=0, green=0;


/*----------------------Background-------------------------*/
unsigned int8 replaceBackgroundColor=0x42;//Set a background color command
unsigned int8 red_MSB=0xf0, red_LSB=0x00;//Red color

/*----------------------DRAWINGS-------------------------*/
unsigned int8 circle=0x43;//Circle command
//Circle 1
unsigned int8 x1MSB=0x00,x1LSB=0x7e,y1MSB=0x00,y1LSB=0x2f;//Location of the circle 1
unsigned int8 colorCirc1MSB=0x0f,colorCirc1LSB=0x00;//Color of the circle 1
unsigned int8 radius_MSB=0x00, radius_LSB=0x23;//Radius of the circle

//Rectangle 1
unsigned int16 r1=0x72;//Rectangle command
unsigned int16 xr1MSB=0x00, xr1LSB=0x30, yr1MSB=0x00, yr1LSB=0xc5, xr2MSB=0x00, xr2LSB=0xc5, yr2MSB=0x01, yr2LSB=0x30;
unsigned int16 colorRec1=0x00, coloRec2=0x00;



// INTERUPT 
#INT_RDA
void interupt()
{
   if(kbhit()) //Return TRUE when a character is received in the buffer (Buffer is full)
   {
      dataRX=getc();
      
      if (dataRX == 0x06)//ACK == good return 
         {
            ACK=1;//
         }
      if (dataRX>0x15)
      {
         int nbyte = 0;                 
         
            WHILE(nbyte<5)
            {               // réception des 4 octets de coordonnées
               CORD[nbyte] = getc();
               while(!kbhit());        // atente de nouvelle donnée
               nbyte++;
            }
         y = make16(CORD[0],CORD[1]);  // met les coordonnées dans un mote de 16 bits ya que el LCD las envía en 2 bytes
         x = make16(CORD[2],CORD[3]);
         
         if(y>0x00c5 && y<0x0130)
         {
            greenButton = 1;
            dataRX=0;
         }
      }   
   }
}





// MAIN PROGRAM//
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
   printf("%c",clearScreen); //Clear the Screen 
   while(ACK!=1); ACK = 0; // To check if the buffer is in the next step
     
     
   /*-----------------------BACKGROUND-------------------------*/
   printf("%c%c%c",replaceBackgroundColor,red_MSB,red_LSB);//set a red background
   while(ACK!=1); ACK = 0; // To check if the buffer is in the next step
   
   /*--------------------TOUCHSCREEN CONTROL----------------*/
   printf("%c%c%c",displayControl,touchControl,enableTouchScreen);//Enable the touchScreen control
   while(ACK!=1); ACK = 0; // To check if the buffer is in the next step²

    
     
     
   printf("%c%c%c%c%c%c%c%c%c%c%c",r1,xr1MSB,xr1LSB,yr1MSB,yr1LSB,xr2MSB,xr2LSB,yr2MSB,yr2LSB,colorRec1,colorRec2);
   
   
   
   
   
   
   while(TRUE)
   {
      printf("%c%c",0x6F,0x01); // Get Touch coodinates
      while(greenButton==1)
      {
         dataRX=0;
         //Set a green full circle in the top and in the middle of the screen
         green=1;
         greenButton=0;
         
      }
      
      if(green==1)
      {
         printf("%c%c%c%c%c%c%c%c%c%c",circle,xr1MSB,xr1LSB,yr1MSB,xr2MSB,xr2LSB,yr2MSB,yr2LSB,colorRec1,colorRec2);
         //delay_ms(3000);
      }
   
  }
   
}//End of the main 




