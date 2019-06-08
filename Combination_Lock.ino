#include <SPI.h>
#include <LiquidCrystal.h>
#include <IRremote.h>       // biblioteka

#define receiverpin 13     // pin Arduino, do którego został podłączony odbiornik IR

IRrecv irrecv(receiverpin); 
decode_results results;

LiquidCrystal lcd(12,11,5,4,3,2);
int portA1 = A1, portA2 = A2, portA3 = A3, portA4 = A4;
int V1, V2, V3, V4;
int keyCode, counter = 0,  password = 0000, developerPassword = 9999;
int writtenPassword, oldPassword, newPassword;
int remoteValue = 0;
boolean activity = false, changingState = false;
unsigned long time_;
#define waitTime 15000

void setup() 
{
  Serial.begin(9600);
  SPI.begin(); 
  lcd.begin(16,2);
  pinMode(7,OUTPUT);
  irrecv.enableIRIn();
}

void loop() 
{
  VoltageMeasurement();
  if (irrecv.decode(&results) || (V1 > 100 || V2 > 100 || V3 > 100 || V4 > 100)) //Check remote
  {
    lcd.display();
    activity = true;                        
    time_ = millis() + waitTime;
  }

  if (activity && time_ < millis())
    activity = false;

  if(activity==true)
    {
    lcd.clear();
    Registration();
    }
  else
    {
      lcd.noDisplay();
    }
    irrecv.resume();
}

void check()
{
  lcd.clear();
     lcd.setCursor(4,0);
           lcd.print("CHECKING");
           delay(2000);
           lcd.clear();
             if(writtenPassword == password)
             { 
              lcd.setCursor(4,0);
              lcd.print("WELCOME!");
              digitalWrite(7,HIGH);
              delay(6000);
              digitalWrite(7,LOW);
              lcd.clear();
             }
             else if (writtenPassword == developerPassword)
             {
              lcd.setCursor(2,0);
              lcd.print("DEVELOPER'S");
              lcd.setCursor(8,1);
              lcd.print("ACCESS");
              digitalWrite(7,HIGH);
              delay(6000);
              digitalWrite(7,LOW);
              lcd.clear();
             }
             else
             { 
              lcd.setCursor(2,0);
              lcd.print("INCORRECT");
              lcd.setCursor(6,1);
              lcd.print("PASSWORD");
              delay(3000);
             }
                 
           activity=false;
           counter=0;
}

void Registration()
{
          irrecv.resume();
       do
        {
          
          lcd.setCursor(1,0);
          lcd.print("PASSWORD:");
          lcd.setCursor(counter+11,1);
          delay(300);
          if(irrecv.decode(&results))
          {
            Serial.print(results.value);
            remoteValue = results.value;
          }
          else
          {
           VoltageMeasurement();
          }
          KeyIdentyfication();
          Display(keyCode);
          delay(200);
                   if(keyCode=="#")
                     {
                      keyCode = "noCode";
                      changingState = true;
                      counter=0;
                          lcd.clear();
                           lcd.setCursor(2,0);
                           lcd.print("PASSWORD");
                           lcd.setCursor(6,1);
                           lcd.print("CHANGING");
                                delay(3000);
                           lcd.clear();
                           do
                           {
                            lcd.setCursor(1,0);
                            lcd.print("LAST PASSWORD:");
                            lcd.setCursor(counter+11,1);
                            delay(300);
                            if(irrecv.decode(&results))
                            {
                              Serial.print(results.value);
                              remoteValue = results.value;
                            }
                            else
                            {
                               VoltageMeasurement();
                            }
                            KeyIdentyfication();
                            Display2(keyCode);
                            keyCode = "noCode";
                            delay(200);   
                            irrecv.resume();         
                           } while(counter<4);
                           
                           ResetPassword();
                     }
                     
          keyCode = "noCode";
          irrecv.resume();
         
        }while(counter<4);
        lcd.clear();
                          
      
      if(changingState == false) 
            { 
              check();
            }
}

void VoltageMeasurement()
{
  delay(200);
  V1 = analogRead(portA1);
  V2 = analogRead(portA2);
  V3 = analogRead(portA3);
  V4 = analogRead(portA4);
}

void KeyIdentyfication()
{
  if((V1<100 && V2<100 && V3<100 && V4<100) && (remoteValue > 40000000 || remoteValue == 0))
  {
    keyCode = "noCode";
  }
  
  else if (V4>100 && V4<300)
  {
   keyCode = "*";
   counter++;
  }
  
  else if ((V4>300 && V4<550) || remoteValue == 26775  )
  {
  keyCode = 0;
   counter++;
  }
  
  else if ((V4>550 && V4<800) || remoteValue == -28561 )
  {
  keyCode = "#";
  }
  
  else if ((V3>100 && V3<300) || remoteValue == 17085  )
  {
  keyCode = 7;
   counter++;
  }
  
  else if ((V3>300 && V3<550) || remoteValue == 19125 )
  {
  keyCode = 8;
   counter++;
  }
  
  else if ((V3>550 && V3<800) || remoteValue == 21165 )
  {
  keyCode = 9;
   counter++;
  }
  
  else if ((V2>100 && V2<300) || remoteValue == 4335 )
  {
  keyCode = 4;
   counter++;
  }
  
  else if ((V2>300 && V2<550) || remoteValue == 14535 )
  {
  keyCode = 5;
   counter++;
  }
  
  else if (V2>960 || remoteValue == 23205 )
  {
  keyCode = 6;
   counter++;
  } 
  
  else if ((V1>100 && V1<300) || remoteValue == 12495 )
  {
  keyCode = 1;
   counter++;
  }
  
  else if ((V1>300 && V1<550) || remoteValue == 6375 )
  {
  keyCode = 2;
   counter++;
  }
  
  else if ((V1>550 && V1<800) || remoteValue == 31365 )
  {
  keyCode = 3;
   counter++;
 } 
 remoteValue = 0;
}

void Display(int character)
{
if (character != "noCode" && (character==1 || character==2 || character==3 || character==4 || character==5 || character==6 || character==7 || character==8 || character==9 || character==0)){
 lcd.print(character);
 if(counter==1)
      writtenPassword=1000*character;
  if(counter==2)
      writtenPassword=writtenPassword+100*character;
  if(counter==3)
      writtenPassword=writtenPassword+10*character;
   if(counter==4){
      writtenPassword=writtenPassword+1*character;
      delay(800);}
 }
 }

void Display2(int character)
{
if (character != "noCode" && (character==1 || character==2 || character==3 || character==4 || character==5 || character==6 || character==7 || character==8 || character==9 || character==0)){
 lcd.print(character);
 if(counter==1)
      oldPassword=1000*character;
  if(counter==2)
      oldPassword=oldPassword+100*character;
  if(counter==3)
      oldPassword=oldPassword+10*character;
   if(counter==4){
      oldPassword=oldPassword+1*character;
      delay(800);}
 }
 
 }

 void Display3(int character)
{
if (character != "noCode" && (character==1 || character==2 || character==3 || character==4 || character==5 || character==6 || character==7 || character==8 || character==9 || character==0))
{
 lcd.print(character);
 if(counter==1)
      newPassword=1000*character;
  if(counter==2)
      newPassword=newPassword+100*character;
  if(counter==3)
      newPassword=newPassword+10*character;
   if(counter==4){
      newPassword=newPassword+1*character;
      delay(800);}
 }
}

void ResetPassword()
{
  if( oldPassword == password || oldPassword == developerPassword )
  {
    counter=0;
    lcd.clear();
    lcd.setCursor(4,0);
    lcd.print("CHANGING");
    delay(2000);
            do{
                             
                             irrecv.resume();
                             lcd.setCursor(1,0);
                             lcd.print("NEW PASSWORD:");
                             lcd.setCursor(counter+11,1);
                              delay(300);
                             if(irrecv.decode(&results))
                              {
                                remoteValue = results.value;
                              }
                              else
                              {
                               VoltageMeasurement();
                              }
                             KeyIdentyfication();
                             Display3(keyCode);
                             keyCode = "noCode";
                             delay(200);
                             
               } while(counter<4);
     SetNewPassword();
  }
  else
  {
    activity=false;
    counter=0;
    changingState = false;
    lcd.clear();
  } 
 
}

void SetNewPassword()
{

    password=newPassword;

                               
changingState = false;
activity=false;
counter=0;
lcd.clear();
}
