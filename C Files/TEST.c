//#include <18f4620.h>
#include <18f4620.h>
#use delay(crystal=20Mhz)
#fuses NOPROTECT
#use rs232(baud=9600, PARITY=N, BITS=8, xmit=PIN_C6, rcv=PIN_C7)



   int8 CORD[5]={0,0,0,0,0};
   int8 in=0;

   int memoire=0;
   int scene=0;
   int a=0;
   int b=0;
   int c=0;
   int d=0;
   int e=0;
   int f=0;
   int g=0;
   int h=0;
   int i,j=0;
   
//Définition des variables

int ACK = 0;
int16 dataACK=0;
int detectSc1=0;
int detectSc2=0;
int detectSc3=0;
int detectOFF=0;
int16 x,y;

#INT_RDA                //sous-programme d'interruption
void RS232_ACK()        //
{                       //
   if (kbhit())         //kbhit --> détecte automatiquement retour sur TX
   {
      dataACK= getc();      //ACK prend la valeur venant du port C
      if (dataACK==0x06)
      {
         ACK=1;         //ACK devient l'image du port C en réponse booléenne
      }
      if (dataACK>21)
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
         
         if (y>5 && y<80)
         {
            detectSc1=1;
            dataACK=0;
         }
         if (y>90 && y<165)
         {
            detectSc2=1;
            dataACK=0;
         }
         if (y>175 && y<250)
         {
            detectSc3=1;
            dataACK=0;
         }
         if (y>250)
         {
            detectOFF=1;
            dataACK=0;
         }
      }
   }
}

void main()
{
//PROTOCOLE DE RESET ET D'INITIALISATION
   output_low(PIN_C5);
   delay_ms(500);
   output_high(PIN_C5);
   delay_ms(1500);   // Temps obligatoire pour Reset de l'écran
   
   //enable_interrupts(int_rda);   //définition du sous-programme d'interruption
   enable_interrupts(GLOBAL);
   enable_interrupts(INT_RDA);    
   printf("U");
   while (ACK!=1);               //boucle WHILE dépendant de la boucle d'interruption
   ACK=0;  //Réponse de la boucle d'interruption remise à zéro
   printf("E"); //clear screen
   while (ACK!=1);
   ACK=0;
   printf("F%c",1); //Définit la taille des caractères
   while (ACK!=1);
   ACK=0;
   printf("O%c",0); //Définit l'opacité du fond des caractères
   while (ACK!=1);
   ACK=0;
   printf("B%c%c",255,255); //changement de couleur
   while (ACK!=1);
   ACK=0;
//////////////////////////////////MISE EN FORME GRAPHISME\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\
   
   printf("c%c%c%c%c%c%c%c%c%c%c",0,126,0,105,0,230,0,35,240,0);  //Rectangle NOIR (x1;y1)=(5;5) (x2;y2)=(230;80)
   while (ACK!=1);
   ACK=0;
   printf("s%c%c%c%c%c%s%c",11,3,2,255,255,"Scene 1",0);
   while(ACK!=1);
   ACK=0;
   
   printf("r%c%c%c%c%c%c%c%c%c%c",0,5,0,90,0,230,0,165,0,0);  //Rectangle NOIR (x1;y1)=(5;90) (x2;y2)=(230;165)
   while (ACK!=1);
   ACK=0;
   printf("s%c%c%c%c%c%s%c",11,10,2,255,255,"Scene 2",0);
   while(ACK!=1);
   ACK=0;
   
   printf("r%c%c%c%c%c%c%c%c%c%c",0,5,0,175,0,230,0,250,0,0);  //Rectangle NOIR (x1;y1)=(5;175) (x2;y2)=(230;250)
   while (ACK!=1);
   ACK=0;
   printf("s%c%c%c%c%c%s%c",11,17,2,255,255,"Scene 3",0);
   while(ACK!=1);
   ACK=0;
   
   //printf("r%c%c%c%c%c%c%c%c%c%c",0,67,1,4,0,177,1,50,0,0);  //Rectangle NOIR (x1;y1)=(5;175) (x2;y2)=(230;250)
   //while (ACK!=1);
   //ACK=0;
   printf("s%c%c%c%c%c%s%c",13,23,2,0,0,"OFF",0);
   while(ACK!=1);
   ACK=0;
 
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
   
// Tacile

   printf("Y%c%c",5,0); //Enable touch screen
   while (ACK!=1);
   ACK=0;
   
   while (true)
   {
    
      printf("%c%c",0x6F,0x01); // Get Touch coodinates
      while(detectSc1==1)
      {
         dataACK=0;
         output_d(1);
         detectSc1=0;
      }
      printf("%c%c",0x6F,0x01); // Get Touch coodinates
      while(detectSc2==1)
      {
         dataACK=0;
         output_d(2);
         detectSc2=0;
      }
      printf("%c%c",0x6F,0x01); // Get Touch coodinates
      while(detectSc3==1)
      {
         dataACK=0;
         output_d(4);
         detectSc3=0;
      }
      printf("%c%c",0x6F,0x01); // Get Touch coodinates
      while(detectOFF==1)
      {
         dataACK=0;
         output_d(0);
         detectOFF=0;
      }
      
         
   }
}
