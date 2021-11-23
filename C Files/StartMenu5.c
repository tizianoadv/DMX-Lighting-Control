#include <18f4620.h>
#use delay (crystal=20MHz) // preprocessor directive (specifies clock type and speed)
#use rs232 (baud=9600,parity=N,xmit=PIN_C6,rcv=PIN_C7,bits=8) 

//Init Screen
unsigned int8 ACK=0;
unsigned int8 REFRESH=0;//Refresh Command
unsigned int8 dataRX=0; //PIN_C7 receive
unsigned int8 autobaud=0x55;//Letter U = 0x55 (Autobaud)
unsigned int8 clearScreen=0x45;//Clear screen command
unsigned int8 setBackgroundColor=0x4B;//Set background color command
unsigned int8 replaceBackgroundColor=0x42;//Replace Background color command
unsigned int8 command=0x59;
unsigned int8 touchControl=0x05;
unsigned int8 enable=0x00;
unsigned int16 CORD[5];
unsigned int16 x=0,y=0;
unsigned int8 detectTouchRegion=0x75;
unsigned int8 detectSc1=0,detectSc2=0, detectSc3=0;
unsigned int8 manual=0,automatic=0,musical=0;

/*-----------------------BACKGROUND COLORS-------------------------*/
//Red
unsigned int16 redMSB=0xf0, redLSB=0x00;
//Green 
unsigned int16 greenMSB=0xff, greenLSB=0xff;
//Blue
unsigned int16 blueMSB=0x0c, blueLSB=0xff;
//White
unsigned int16 whiteMSB=0xff, whiteLSB=0xff;

/*-----------------------DRAWINGS-------------------------*/
//Drawing Circle 1
unsigned int16 c1=0x43;
unsigned int16 x1MSB=0x00,x1LSB=0x7e, y1MSB=0x00,y1LSB=0x69, radius1MSB=0x00, radius1LSB=0x23,color1MSB=0xf0, color1LSB=0x00;
//Drawing Circle 2
unsigned int16 c2=0x43;
unsigned int16 x2MSB=0x00, x2LSB=0x7e, y2MSB=0x00, y2LSB=0xb9, radius2MSB=0x00, radius2LSB=0x23, color2MSB=0x0f, color2LSB=0x00;
//Drawing Circle 3
unsigned int16 c3=0x43;
unsigned int16 x3MSB=0x00, x3LSB=0x7e, y3MSB=0x01, y3LSB=0x09, radius3MSB=0x00, radius3LSB=0x23, color3MSB=0x00, color3LSB=0xff;
//Drawing rectangle 1
unsigned int16 r1=0x72;
unsigned int16 xr1MSB=0x00, xr1LSB=0x00, yr1MSB=0x00, yr1LSB=0x00, xr2MSB=0x00, xr2LSB=0xef, yr2MSB=0x00, yr2LSB=0x30, colorRec1=0x00, colorRec2=0x00;

/*-------------------------TEXT---------------------------*/
//Used by all text command
unsigned int16 text=0x73, colorText1=0xff, colorText2=0xff, terminator=0x00;
//Welcome
unsigned int16 colWel=0x07, rowWel=0x09;
//Select the scene
unsigned int16 col=0x02, row=0x01, font=0x03;
//Scene 1 text
unsigned int16 colS1=0x0a, rowS1=0x06, fontS1=0x03;
//Scene 2 text
unsigned int16 colS2=0x0a, rowS2=0x0b;
//Scene 3 textunsigned int16 colS3=0x0a, rowS3=0x10;

/*
   %c => print a Value 
   %d,%i... => print a Character
*/




// INTERUPT
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
      if(dataRX>0x15)
      {
         int nbyte = 0;
         while(nbyte<5)
         {
            CORD[nbyte] = getc();
            while(!kbhit());        // atente de nouvelle donnée
            nbyte++;
         }
         y = make16(CORD[0],CORD[1]);  // met les coordonnées dans un mote de 16 bits ya que el LCD las envía en 2 bytes
         x = make16(CORD[2],CORD[3]);
         
         
         
         
         //***************** Button Rectangle ***********************
         
         if((x>0x0005 &&x<0x00ea) && (y>0x004a && y<0x0080))//COORD Manual 
         {
            manual=1;
            dataRX=0;
         }
         if((x>0x0005 &&x<0x00ea) && (y>0x009a && y<0x00d1))//COORD Automatic
         {
            automatic=1;
            dataRX=0;
         }
         if ((x>0x0005 &&x<0x00ea) && (y>0x00ea && y<0x0120))//COORD Musical
         {
            musical=1;
            dataRX=0;
         } 
         
         //***************** Button Circles *******************
         
         if((x>0x0059 &&x<0x00a2) && (y>0x0040 && y<0x0090))//COORD Button 1 
         {
            detectSc1=1;
            dataRX=0;
         }
         if((x>0x0059 &&x<0x00a2) && (y>0x0093 && y<0x00df))//COORD Button 2
         {
            detectSc2=1;
            dataRX=0;
         }
         if ((x>0x0059 &&x<0x00a2) && (y>0x00e4 && y<0x0130))//COORD Button 3
         {
            detectSc3=1;
            dataRX=0;
         }
         
      }

   }
       
}





// PROGRAMME PRINCIPAL //
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
   
    printf("%c%c%c%c%c%cWELCOME%c",text,colWel,rowWel,fontS1,colorText1,colorText2,terminator);//Welcome text
    while(ACK!=1); ACK = 0; // To check if the buffer is in the next step
    delay_ms(2000);
    
      /*-----------------------BACKGROUND-------------------------*/
      printf("%c%c%c",replaceBackgroundColor,WhiteMSB,WhiteLSB);
      while(ACK!=1); ACK = 0; // Interupt 
      
      /*-----------------------DRAWINGS-------------------------*/
      //Cercle 1
      printf("%c%c%c%c%c%c%c%c%c",c1,x1MSB,x1LSB,y1MSB,y1LSB,radius1MSB,radius1LSB,color1MSB,color1LSB);
      while(ACK!=1); ACK = 0; // Interupt
      //Cercle 2
      printf("%c%c%c%c%c%c%c%c%c",c2,x2MSB,x2LSB,y2MSB,y2LSB,radius2MSB,radius2LSB,color2MSB,color2LSB);
      while(ACK!=1); ACK = 0; // Interupt
      //Cercle 3
      printf("%c%c%c%c%c%c%c%c%c",c3,x3MSB,x3LSB,y3MSB,y3LSB,radius3MSB,radius3LSB,color3MSB,color3LSB);
      while(ACK!=1); ACK = 0; // Interupt
     
      //Rectangle Black
      printf("%c%c%c%c%c%c%c%c%c%c%c",r1,xr1MSB,xr1LSB,yr1MSB,yr1LSB,xr2MSB,xr2LSB,yr2MSB,yr2LSB,colorRec1,colorRec2);
      while(ACK!=1); ACK = 0; // Interupt 
      
      /*-----------------------TEXT-------------------------*/
      //Welcome Text
      printf("%c%c%c%c%c%cSelect the scene %c",text,col,row,font,colorText1,colorText2,terminator);
      while(ACK!=1); ACK = 0; // Interupts 
      //Scene 1
      printf("%c%c%c%c%c%c1%c",text,colS1,rowS1,fontS1,colorText1,colorText2,terminator);
      while(ACK!=1); ACK = 0; // Interupts 
      //Scene 2
      printf("%c%c%c%c%c%c2%c",text,colS2,rowS2,fontS1,colorText1,colorText2,terminator);
      while(ACK!=1); ACK = 0; // Interupts  
      //Scene 3
      printf("%c%c%c%c%c%c3%c",text,colS3,rowS3,fontS1,colorText1,colorText2,terminator);
      while(ACK!=1); ACK = 0; // Interupts       
  
      
      
   while (TRUE)
   {
    
      printf("%c%c",0x6F,0x01); // Get Touch coodinates
      while(manual==1)
      {
         dataRX=0;
         manualMode();
         manual=0;
      }
      printf("%c%c",0x6F,0x01); // Get Touch coodinates
      while(automatic==1)
      {
         dataRX=0;
         //....
         detectSc2=0;
      }
      printf("%c%c",0x6F,0x01); // Get Touch coodinates
      while(detectSc3==1)
      {
         dataRX=0;
         //....
         detectSc3=0;
      }
   }
   
}//END of the MAIN 










