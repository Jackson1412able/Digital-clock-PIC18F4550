/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//      PIN A0  - Mode/Confirm                                                                                             //
//      PIN A1  - Alarm                                                                                                    //
//      PIN A2  - Cancel                                                                                                   //
//      PIN A3  - Increment                                                                                                //
//      PIN A6  - Cancel Alarm                                                                                             //
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <18F4550.h>
#FUSES NOWDT                    //No Watch Dog Timer
#FUSES INTRC_IO                 //Crystal osc <= 4mhz for PCM/PCH , 3mhz to 10 mhz for PCD
#FUSES NOPBADEN                 //PORTB pins are configured as digital I/O on RESET
#FUSES NOLVP                    //No low voltage prgming, B3(PIC16) or B5(PIC18) used for I/O
#FUSES NOXINST                  //Extended set extension and Indexed Addressing mode disabled (Legacy mode)
#FUSES NODEBUG
#device ADC=8
#use delay(clock=4000000)

#define bit0_a       PIN_D0
#define bit1_a       PIN_D1
#define bit2_a       PIN_D2
#define bit3_a       PIN_D3

#define bit0_b       PIN_C1
#define bit1_b       PIN_C2
#define bit2_b       PIN_C6
#define bit3_b       PIN_C7

#define bit0_c       PIN_B0
#define bit1_c       PIN_B1
#define bit2_c       PIN_B2
#define bit3_c       PIN_B3

#define bit0_d       PIN_B5
#define bit1_d       PIN_B6
#define bit2_d       PIN_B7
#define bit3_d       PIN_E2

#define bit0_e       PIN_D4
#define bit1_e       PIN_D5
#define bit2_e       PIN_D6
#define bit3_e       PIN_D7

#define bit0_f       PIN_B4
#define bit1_f       PIN_E1
#define bit2_f       PIN_E0
#define bit3_f       PIN_A5

#define blink        PIN_C0

void signal(int aiyoyo, int time_unit);
void no_display(int aiyo);
void zero(int aiyo);
void one(int aiyo);
void two(int aiyo);
void three(int aiyo);
void four(int aiyo);
void five(int aiyo);
void six(int aiyo);
void seven(int aiyo);
void eight(int aiyo);
void nine(int aiyo);

long int bit0[]=bit0_a, bit0_b, bit0_c, bit0_d, bit0_e, bit0_f;
long int bit1[]=bit1_a, bit1_b, bit1_c, bit1_d, bit1_e, bit1_f;
long int bit2[]=bit2_a, bit2_b, bit2_c, bit2_d, bit2_e, bit2_f;
long int bit3[]=bit3_a, bit3_b, bit3_c, bit3_d, bit3_e, bit3_f;

int hour_1st_digit;
int hour_2nd_digit;
int min_2nd_digit;
int min_1st_digit;
int sec_2nd_digit;
int sec_1st_digit;

int alarm_hour_1st_digit=0;
int alarm_hour_2nd_digit=0;
int alarm_min_2nd_digit=0;
int alarm_min_1st_digit=0;
int alarm_sec_2nd_digit=0;
int alarm_sec_1st_digit=0;

void main()
{  
   setup_oscillator(OSC_4MHZ);
   int i;
   int check, check2=0, check3=0, alarm=0;
      
   set_tris_a(0x0f);  //0100 1111, RA0~RA3,RA6 as input, RA4,RA5,RA7 as output
   set_tris_b(0x00);  //port B set as output
   set_tris_c(0x00);  //port C set as output
   set_tris_d(0x00);  //port D set as output
   set_tris_e(0x00);  //port E set as output
  
   while(1)
   {
      for(hour_2nd_digit=0;hour_2nd_digit<=2;hour_2nd_digit++)  //hours loop second digit
      {
         for(hour_1st_digit=0;hour_1st_digit<=check;hour_1st_digit++) //hours loop first digit
         {            
            for(min_2nd_digit=0;min_2nd_digit<=5;min_2nd_digit++) //minute loop second digit
            {
               for(min_1st_digit=0;min_1st_digit<=9;min_1st_digit++)  //minute loop first digit
               {    
                  for(sec_2nd_digit=0;sec_2nd_digit<=5;sec_2nd_digit++) //seconds loop second digit
                  {                        
                     for(sec_1st_digit=0;sec_1st_digit<=9;sec_1st_digit++)  //seconds loop first digit
                     {  
                        delay_us(1000);
                        output_toggle(blink);
                        for(i=1;i<=41;i++)
                        { 
                           if(input(PIN_A1)&&(!check2)){  //If Confirm button pressed, stop modifying time
                           check2=1;
                           while(input(PIN_A1));}

                           if(check2==0)
                           {
                              signal(0, sec_1st_digit); //send 1st digit of seconds to binary to 7 segment decoder
                              signal(1, sec_2nd_digit); //send 2nd digit of seconds to binary to 7 segment decoder
                              signal(2, min_1st_digit); //send 1st digit of minutes to binary to 7 segment decoder
                              signal(3, min_2nd_digit); //send 2nd digit of minutes to binary to 7 segment decoder
                              signal(4, hour_1st_digit); //send 1st digit of hour to binary to 7 segment decoder
                              signal(5, hour_2nd_digit); //send 2nd digit of hour to binary to 7 segment decoder
                              
                              while(input(PIN_A0)) //Modify seconds
                              {
                                 do{      
                                 while(input(PIN_A0));
                                 
                                 //Modify seconds
                                 do{ 
                                 signal(0, sec_1st_digit);
                                 signal(1, sec_2nd_digit);
                                 signal(2, 10);
                                 signal(3, 10);
                                 signal(4, 10);
                                 signal(5, 10);
                                 
                                 if(input(PIN_A3))  //Increment seconds
                                 {
                                    if((sec_1st_digit==9)&&sec_2nd_digit==5)
                                    {
                                       sec_2nd_digit=0;
                                       sec_1st_digit=0;
                                    }
                                    
                                    else if((sec_1st_digit==9)&&(sec_2nd_digit!=5))
                                    {
                                       sec_2nd_digit++;
                                       sec_1st_digit=0;
                                    }
                                    else
                                    {
                                       sec_1st_digit++;                                 
                                    }
                                    while(input(PIN_A3));
                                 }
                                 }while((!input(PIN_A2))&&(!input(PIN_A0)));  //exits mode if confirm or cancel button is pressed
                                 if(input(PIN_A0))  
                                 break;
                                 while(input(PIN_A2));
                                 
                                 //Modify minutes
                                 do{
                                 signal(0, 10);
                                 signal(1, 10);
                                 signal(2, min_1st_digit);
                                 signal(3, min_2nd_digit);
                                 signal(4, 10);
                                 signal(5, 10);
                                 
                                    if(input(PIN_A3))  //Increment minutes
                                    {
                                    if((min_1st_digit==9)&&(min_2nd_digit==5))
                                    {
                                       min_2nd_digit=0;
                                       min_1st_digit=0;
                                    }
                                    
                                    else if((min_1st_digit==9)&&(min_2nd_digit!=5))
                                    {
                                       min_2nd_digit++;
                                       min_1st_digit=0;
                                    }
                                    else
                                    {
                                       min_1st_digit++;                                 
                                    }
                                    while(input(PIN_A3));
                                    }       
                                 }while((!input(PIN_A2))&&(!input(PIN_A0)));  //exits mode if confirm or cancel button is pressed
                                 if(input(PIN_A0)) 
                                 break;
                                 while(input(PIN_A2));
                                 
                                 //Modify hours
                                 do{
                                 signal(0, 10);
                                 signal(1, 10);
                                 signal(2, 10);
                                 signal(3, 10);
                                 signal(4, hour_1st_digit);
                                 signal(5, hour_2nd_digit);
                               
                                    if(input(PIN_A3))  //Increment hours
                                    {
                                       if((hour_1st_digit==3)&&(hour_2nd_digit==2))
                                       {
                                          hour_2nd_digit=0;
                                          hour_1st_digit=0;
                                       }
                                       else if((hour_1st_digit==9)&&(hour_2nd_digit!=2))
                                       {
                                          hour_2nd_digit++;
                                          hour_1st_digit=0;
                                       }
                                       else
                                       {
                                          hour_1st_digit++;
                                       }
                                       while(input(PIN_A3));
                                    }
                                 
                                 
                                 }while((!input(PIN_A2))&&(!input(PIN_A0)));
                                 if(input(PIN_A0)) //exit adjust mode if Confirm is pressed
                                 break;
                                 while(input(PIN_A2));
                                 
                                 }while(!input(PIN_A0));  //loop exit if Confirm is pressed
                                 if(input(PIN_A0))
                                 break;
                              }
                              while(input(PIN_A0));
                           }
                       
                           //Set Alarm
                           if(check2==1)
                           {
                              if(input(PIN_A1)){  //If Alarm pressed again, alarm set
                              check2=0;
                              alarm=1;
                              check3=0;
                              while(input(PIN_A1));}
                              
                              if(input(PIN_A0)){  //If Mode pressed, alarm cancelled
                              check2=0;
                              check3=0;
                              if(!input(PIN_A6))  //If Cancel alarm pressed, alarm settings unsaved and exits
                              alarm=0;
                              alarm_hour_1st_digit=0;
                              alarm_hour_2nd_digit=0;
                              alarm_min_2nd_digit=0;
                              alarm_min_1st_digit=0;
                              while(input(PIN_A0));}
                              
                              signal(0, 0);
                              signal(1, 0);
                              signal(2, alarm_min_1st_digit);
                              signal(3, alarm_min_2nd_digit);
                              signal(4, alarm_hour_1st_digit);
                              signal(5, alarm_hour_2nd_digit);
                              
                              if(input(PIN_A2)) //If A2 pressed, modify either alarm minutes or hours in alternate manner
                              {
                                 check3=check3+1;
                                 if(check3==10)
                                 check3=0;
                                 while(input(PIN_A2));
                              }
                              
                              if((check3%2)==0)
                              {
                                 if(input(PIN_A3))  //Increment alarm minutes
                                 {
                                    if((alarm_min_1st_digit==9)&&(alarm_min_2nd_digit==5))
                                    {
                                       alarm_min_2nd_digit=0;
                                       alarm_min_1st_digit=0;
                                    }
                                 
                                    else if((alarm_min_1st_digit==9)&&(alarm_min_2nd_digit!=5))
                                    {
                                       alarm_min_2nd_digit++;
                                       alarm_min_1st_digit=0;
                                    }
                                    else
                                    {
                                       alarm_min_1st_digit++;                                 
                                    }
                                 while(input(PIN_A3));
                                 }       
                              }
                            
                              if((check3%2)!=0)
                              {
                                 if(input(PIN_A3))  //Increment alarm hours
                                 {
                                    if((alarm_hour_1st_digit==3)&&(alarm_hour_2nd_digit==2))
                                    {
                                       alarm_hour_2nd_digit=0;
                                       alarm_hour_1st_digit=0;
                                    }
                                    else if((alarm_hour_1st_digit==9)&&(alarm_hour_2nd_digit!=2))
                                    {
                                       alarm_hour_2nd_digit++;
                                       alarm_hour_1st_digit=0;
                                    }
                                    else
                                    {
                                       alarm_hour_1st_digit++;
                                    }
                                    while(input(PIN_A3));
                                 }
                              }
                           }
                           
                           //Alarm
                           if((alarm==1)&&(alarm_min_1st_digit==min_1st_digit)&&(alarm_min_2nd_digit==min_2nd_digit)&&(alarm_hour_1st_digit==hour_1st_digit)&&(alarm_hour_2nd_digit==hour_2nd_digit)&&(alarm_sec_1st_digit==sec_1st_digit)&&(alarm_sec_2nd_digit==sec_2nd_digit))
                           output_high(PIN_A4);
                           
                           //Off Alarm
                           if((alarm==1)&&(input(PIN_A6))&&((input(PIN_A3))||(input(PIN_A2))))
                           {
                              alarm=0;
                              output_low(PIN_A4);
                              while(input(PIN_A2));
                              while(input(PIN_A3));
                           }
                           
                     }
                        output_toggle(blink);
                        for(i=1;i<=41;i++)
                        {
                           if(input(PIN_A1)&&(!check2)){
                           check2=1;
                           while(input(PIN_A1));}

                           if(check2==0)
                           {
                              signal(0, sec_1st_digit);
                              signal(1, sec_2nd_digit);
                              signal(2, min_1st_digit);
                              signal(3, min_2nd_digit);
                              signal(4, hour_1st_digit);
                              signal(5, hour_2nd_digit);
                              while(input(PIN_A0)) //when A0 is pressed
                              {
                                 do{
                                 while(input(PIN_A0));
                                 
                                 do{ //adjust for sec, until A0 or A1 is pressed
                                 signal(0, sec_1st_digit);
                                 signal(1, sec_2nd_digit);
                                 signal(2, 10);
                                 signal(3, 10);
                                 signal(4, 10);
                                 signal(5, 10);
                                 
                                 if(input(PIN_A3))
                                 {
                                    if((sec_1st_digit==9)&&sec_2nd_digit==5)
                                    {
                                       sec_2nd_digit=0;
                                       sec_1st_digit=0;
                                    }
                                    
                                    else if((sec_1st_digit==9)&&(sec_2nd_digit!=5))
                                    {
                                       sec_2nd_digit++;
                                       sec_1st_digit=0;
                                    }
                                    else
                                    {
                                       sec_1st_digit++;                                 
                                    }
                                    while(input(PIN_A3));
                                 }
                                 }while((!input(PIN_A2))&&(!input(PIN_A0)));
                                 if(input(PIN_A0))
                                 break;
                                 while(input(PIN_A2));
                                 
                                 do{
                                 signal(0, 10);
                                 signal(1, 10);
                                 signal(2, min_1st_digit);
                                 signal(3, min_2nd_digit);
                                 signal(4, 10);
                                 signal(5, 10);
                                 
                                    if(input(PIN_A3))
                                    {
                                    if((min_1st_digit==9)&&(min_2nd_digit==5))
                                    {
                                       min_2nd_digit=0;
                                       min_1st_digit=0;
                                    }
                                    
                                    else if((min_1st_digit==9)&&(min_2nd_digit!=5))
                                    {
                                       min_2nd_digit++;
                                       min_1st_digit=0;
                                    }
                                    else
                                    {
                                       min_1st_digit++;                                 
                                    }
                                    while(input(PIN_A3));
                                    }       
                                 }while((!input(PIN_A2))&&(!input(PIN_A0)));
                                 if(input(PIN_A0)) //exit adjust mode if A0 is pressed
                                 break;
                                 while(input(PIN_A2));
                                 
                                 do{
                                 signal(0, 10);
                                 signal(1, 10);
                                 signal(2, 10);
                                 signal(3, 10);
                                 signal(4, hour_1st_digit);
                                 signal(5, hour_2nd_digit);
                               
                                
                                    if(input(PIN_A3))
                                    {
                                       if((hour_1st_digit==3)&&(hour_2nd_digit==2))
                                       {
                                          hour_2nd_digit=0;
                                          hour_1st_digit=0;
                                       }
                                       else if((hour_1st_digit==9)&&(hour_2nd_digit!=2))
                                       {
                                          hour_2nd_digit++;
                                          hour_1st_digit=0;
                                       }
                                       else
                                       {
                                          hour_1st_digit++;
                                       }
                                       while(input(PIN_A3));
                                    }
                                 
                                 
                                 }while((!input(PIN_A2))&&(!input(PIN_A0)));
                                 if(input(PIN_A0)) //exit adjust mode if A0 is pressed
                                 break;
                                 while(input(PIN_A2));
                                 
                                 }while(!input(PIN_A0));  //loop exit if A0 is pressed
                                 if(input(PIN_A0))
                                 break;
                              }
                              while(input(PIN_A0));
                           }
                       
                           if(check2==1)
                           {
                              if(input(PIN_A1)){
                              check2=0;
                              alarm=1;
                              check3=0;
                              while(input(PIN_A1));}
                              
                              if(input(PIN_A0)){
                              check2=0;
                              check3=0;
                              if(!input(PIN_A6))
                              alarm=0;
                              alarm_hour_1st_digit=0;
                              alarm_hour_2nd_digit=0;
                              alarm_min_2nd_digit=0;
                              alarm_min_1st_digit=0;
                              while(input(PIN_A0));}
                              
                              signal(0, 0);
                              signal(1, 0);
                              signal(2, alarm_min_1st_digit);
                              signal(3, alarm_min_2nd_digit);
                              signal(4, alarm_hour_1st_digit);
                              signal(5, alarm_hour_2nd_digit);
                              
                              if(input(PIN_A2))
                              {
                                 check3=check3+1;
                                 if(check3==10)
                                 check3=0;
                                 while(input(PIN_A2));
                              }
                              
                              if((check3%2)==0)
                              {
                                 if(input(PIN_A3))
                                 {
                                    if((alarm_min_1st_digit==9)&&(alarm_min_2nd_digit==5))
                                    {
                                       alarm_min_2nd_digit=0;
                                       alarm_min_1st_digit=0;
                                    }
                                 
                                    else if((alarm_min_1st_digit==9)&&(alarm_min_2nd_digit!=5))
                                    {
                                       alarm_min_2nd_digit++;
                                       alarm_min_1st_digit=0;
                                    }
                                    else
                                    {
                                       alarm_min_1st_digit++;                                 
                                    }
                                 while(input(PIN_A3));
                                 }       
                              }
                            
                              if((check3%2)!=0)
                              {
                                 if(input(PIN_A3))
                                 {
                                    if((alarm_hour_1st_digit==3)&&(alarm_hour_2nd_digit==2))
                                    {
                                       alarm_hour_2nd_digit=0;
                                       alarm_hour_1st_digit=0;
                                    }
                                    else if((alarm_hour_1st_digit==9)&&(alarm_hour_2nd_digit!=2))
                                    {
                                       alarm_hour_2nd_digit++;
                                       alarm_hour_1st_digit=0;
                                    }
                                    else
                                    {
                                       alarm_hour_1st_digit++;
                                    }
                                    while(input(PIN_A3));
                                 }
                              }
                           }
                           
                           if((alarm==1)&&(alarm_min_1st_digit==min_1st_digit)&&(alarm_min_2nd_digit==min_2nd_digit)&&(alarm_hour_1st_digit==hour_1st_digit)&&(alarm_hour_2nd_digit==hour_2nd_digit)&&(alarm_sec_1st_digit==sec_1st_digit)&&(alarm_sec_2nd_digit==sec_2nd_digit))
                           output_high(PIN_A4);
                           
                           if((alarm==1)&&(input(PIN_A6))&&((input(PIN_A3))||(input(PIN_A2))))
                           {
                              alarm=0;
                              output_low(PIN_A4);
                              while(input(PIN_A2));
                              while(input(PIN_A3));
                           }
                           
                     }
                  }
               }
            }
            }
         if(hour_2nd_digit==2)
         check=3;
         else
         check=9;
         }
       }
   }
}

void signal(int aiyoyo, int time_unit)
{
         switch(time_unit)
         {
               case 1:one(aiyoyo);
               break;
               case 2:two(aiyoyo);
               break;
               case 3:three(aiyoyo);
               break;
               case 4:four(aiyoyo);
               break;
               case 5:five(aiyoyo);
               break;
               case 6:six(aiyoyo);
               break;
               case 7:seven(aiyoyo);
               break;
               case 8:eight(aiyoyo);
               break;
               case 9:nine(aiyoyo);
               break;
               case 10:no_display(aiyoyo);
               break;
               default:zero(aiyoyo);
               break;
         }
}

void no_display(int aiyo)
{
      output_high(bit0[aiyo]);
      output_high(bit1[aiyo]);
      output_high(bit2[aiyo]);
      output_high(bit3[aiyo]);
}  

void zero(int aiyo)
{
      output_low(bit0[aiyo]);
      output_low(bit1[aiyo]);
      output_low(bit2[aiyo]);
      output_low(bit3[aiyo]);
}  

void one(int aiyo)
{
      output_high(bit0[aiyo]);
      output_low(bit1[aiyo]);
      output_low(bit2[aiyo]);
      output_low(bit3[aiyo]);
}  

void two(int aiyo)
{
      output_low(bit0[aiyo]);
      output_high(bit1[aiyo]);
      output_low(bit2[aiyo]);
      output_low(bit3[aiyo]);
}  

void three(int aiyo)
{
      output_high(bit0[aiyo]);
      output_high(bit1[aiyo]);
      output_low(bit2[aiyo]);
      output_low(bit3[aiyo]);
}  

void four(int aiyo)
{
      output_low(bit0[aiyo]);
      output_low(bit1[aiyo]);
      output_high(bit2[aiyo]);
      output_low(bit3[aiyo]);
}  

void five(int aiyo)
{
      output_high(bit0[aiyo]);
      output_low(bit1[aiyo]);
      output_high(bit2[aiyo]);
      output_low(bit3[aiyo]);
}  

void six(int aiyo)
{
      output_low(bit0[aiyo]);
      output_high(bit1[aiyo]);
      output_high(bit2[aiyo]);
      output_low(bit3[aiyo]);
}  

void seven(int aiyo)
{
      output_high(bit0[aiyo]);
      output_high(bit1[aiyo]);
      output_high(bit2[aiyo]);
      output_low(bit3[aiyo]);
}  

void eight(int aiyo)
{
      output_low(bit0[aiyo]);
      output_low(bit1[aiyo]);
      output_low(bit2[aiyo]);
      output_high(bit3[aiyo]);
}  

void nine(int aiyo)
{
      output_high(bit0[aiyo]);
      output_low(bit1[aiyo]);
      output_low(bit2[aiyo]);
      output_high(bit3[aiyo]);
}  
