// PS2 Library Include

#include <PS2X_lib.h>        //for v1.6

PS2X ps2x;                   // create PS2 Controller Class


void steps(int);
void Stepper_angle(int);


int error = 0; 
byte type = 0;
byte vibrate = 0;


int countf1 = 0;             //variable for counting pulses when PAD_UP/DOWN of PS2 is pressed
int countb1 = 0;             //variable for counting pulses when PAD_RIGHT/LEFT of PS2 is pressed
int countf = 0;              //variable for changing RPM OF Front Frisbee Throwing Wheel
int countb = 0;              //variable for changing RPM OF Back Frisbee Throwing Wheel
int dirf = 3;                //pin for direction of Front Frisbee Throwing Wheel
int dirb = 5;                //pin for direction of Back Frisbee Throwing Wheel
int twf = 2;                 //pin for pwm(RPM) of Front Frisbee Throwing Wheel
int twb = 4;                 //pin for pwm(RPM) of Back Frisbee Throwing Wheel

int dirrack1 = 30;           //pin for controlling direction of pinion motor by Relay 1 for loading mechanism
int dirrack2 = 32;           //pin for controlling direction of pinion motor by Relay 2 for loading mechanism

int dirStpr = 23;            //pin for determining direction of BALL-SCREW
int stepsStpr = 25;          //pin for giving angle to stepper motor

int dir1 = 13;               //pin for direction of Base Motor 1
int dir2 = 11;               //pin for direction of Base Motor 2
int dir3 = 9;                //pin for direction of Base Motor 3
int dir4 = 7;                //pin for direction of Base Motor 4
int pwm1 = 12;               //pin for pwm of Base Motor 1
int pwm2 = 10;               //pin for pwm of Base Motor 2
int pwm3 = 8;                //pin for pwm of Base Motor 3
int pwm4 = 6;                //pin for pwm of Base Motor 4

void setup()
{
 Serial.begin(115200);

 pinMode(twf, OUTPUT);         //RPM of Front Frisbee Throwing Wheel
 pinMode(twb, OUTPUT);         //RPM of Back Frisbee Throwing Wheel
 pinMode(dirf, OUTPUT);        //Direction of Front Frisbee Throwing Wheel
 pinMode(dirb, OUTPUT);        //Direction of Back Frisbee Throwing Wheel

 pinMode(dirrack1, OUTPUT);
 pinMode(dirrack2, OUTPUT);

 pinMode(dirStpr, OUTPUT);
 pinMode(stepsStpr, OUTPUT);

 pinMode(dir1, OUTPUT);        //Direction Base Motor 1
 pinMode(dir2, OUTPUT);        //Direction Base Motor 2
 pinMode(dir3, OUTPUT);        //Direction Base Motor 3
 pinMode(dir4, OUTPUT);        //Direction Base Motor 4
 pinMode(pwm1, OUTPUT);        //pwm1 Base Motor 1
 pinMode(pwm2, OUTPUT);        //pwm2 Base Motor 2
 pinMode(pwm3, OUTPUT);        //pwm3 Base Motor 3
 pinMode(pwm4, OUTPUT);        //pwm4 Base Motor 4

 digitalWrite(dirf, LOW);
 digitalWrite(dirb, LOW);

 digitalWrite(dirrack1, LOW); 
 digitalWrite(dirrack2, LOW);
 
 digitalWrite(dirStpr, LOW);
 digitalWrite(stepsStpr, LOW);

 digitalWrite(dir1, LOW);
 digitalWrite(dir2, LOW);
 digitalWrite(dir3, LOW);
 digitalWrite(dir4, LOW);


 error = ps2x.config_gamepad(50,46,48,44, true, true);   //setup pins and settings:  GamePad(clock, command, attention, data, Pressures?, Rumble?) check for error
 
 if(error == 0)
 {
  Serial.println("Found Controller, configured successful");
 }

 Serial.print(ps2x.Analog(1), HEX);
   
 type = ps2x.readType(); 
 switch(type)
 {
  case 0:
  Serial.println("Unknown Controller type");
  break;
  case 1:
  Serial.println("DualShock Controller Found");
  break;
  case 2:
  Serial.println("GuitarHero Controller Found");
  break;
 }
  
}

void loop()
{
 if(error == 1)                                //skip loop if no controller found
  return; 
  
 if(type == 2)                                 //Guitar Hero Controller
  ps2x.read_gamepad();                         //read controller 

 else
 {                                             //DualShock Controller
  ps2x.read_gamepad();                         //read controller

  if(ps2x.Button(PSB_START))                   //will be TRUE as long as button is pressed
   Serial.println("Start is being held");
  if(ps2x.Button(PSB_SELECT))
   Serial.println("Select is being held");
        
//Code to Control the RPM of Front & Back Frisbee Throwing Wheel Starts.
  if(ps2x.Button(PSB_PAD_UP))                  //will be TRUE as long as button is pressed
  {         
   if(!(countf > 254))                         //If countf value is less than 254
   {
    ++countf1;                                 //Increase counter one by one by pressing & holding UP Button 
    countf = countf1*3;                        //Multiplying factor is 3 which decides how fast value of countf is to be increased.
   }
   if(countf == 256)                           //If countf value exceeds 255 set it to 255.
   {        
    countf = 255;    
   }
  }

  if(ps2x.Button(PSB_PAD_DOWN))
  {
   if(!(countf < 1))                           //If countf value is greater than 1
   {
    --countf1;                                 //Decrease counter one by one by pressing & holding DOWN Button.
    countf = countf1*3;                        //Multiplying factor is 3 which decides how fast value of countf is to be decreased.
   }
   if(countf == -1)                            //If countf value is less than 0 set it to 0.
   { 
    countf = 0;    
   }
  }
   
  if(ps2x.Button(PSB_PAD_RIGHT))
  {      
   if(!(countb > 254))
   {
    ++countb1;
    countb = countb1*3;
   }      
   if(countb == 256)
   {   
    countb = 255;    
   }
  }
   
  if(ps2x.Button(PSB_PAD_LEFT))
  {
   if(!(countb < 1 ))
   {
    --countb1;
    countb = countb1*3;
   }    
   if(countb == -1)
   {
    countb = 0;    
   }
  }
   
  digitalWrite(dirf, HIGH);
  analogWrite(twf, (countf));                                 //Setting RPM of Front Throwing Wheel by varying countf by pressing & holding Game-Pad UP/DOWN
     
  digitalWrite(dirb, HIGH);
  analogWrite(twb, (countb));                                 //Setting RPM of Front Throwing Wheel by varying countb by pressing & holding Game-Pad RIGHT/LEFT


  if(ps2x.Button(PSB_L2))                                     //When L2 is being pressed Throwing Wheels rotate at fixed rpm.
  {
   analogWrite(twf, 90);
   digitalWrite(dirf, HIGH);
   analogWrite(twb, 90);
   digitalWrite(dirb, HIGH);
   countf = 90;
   countf1 = 30;
   countb = 90;
   countb1 = 30;
  }
  if(ps2x.Button(PSB_R2))                                     //When R2 is being pressed Throwing Wheels Stops rotating.
  {
    analogWrite(twf,0);
    analogWrite(twb,0);
    digitalWrite(dirf,LOW);
    digitalWrite(dirb,LOW);
  }
//Code to Control the RPM of Front & Back Frisbee Throwing Wheel ends.


    
  vibrate = ps2x.Analog(PSAB_BLUE);           //this will set the large motor vibrate speed based on how hard you press the blue (X) button    



//Code of Rack & Pinion for LOADING MECHANISM starts.
  
  
  
  if(ps2x.Button(PSB_SQUARE))                                     //Striker moves down if SQUARE is pressed
  {
   digitalWrite(dirrack1, LOW);
   digitalWrite(dirrack2, HIGH);
  }
  else if(!(ps2x.Button(PSB_RED) || ps2x.Button(PSB_SQUARE)))     //Striker stops if any one of SQUARE & CIRCLE are not pressed
  {
   digitalWrite(dirrack1, LOW);
   digitalWrite(dirrack2, LOW);
  }

  if(ps2x.Button(PSB_RED))                                        //Striker moves up CIRCLE is pressed
  {
   digitalWrite(dirrack1, HIGH);
   digitalWrite(dirrack2, LOW);
  }

  else if(!(ps2x.Button(PSB_RED) || ps2x.Button(PSB_SQUARE)))
  {
   digitalWrite(dirrack1, LOW);
   digitalWrite(dirrack2, LOW);
  }

//Code of Rack & Pinion for Loading Mechanism ends.



//Code for changing angle by BALL-SCREW arrangement begins.
  if(ps2x.Button(PSB_GREEN))                                     //Angle decreases when Triangle is pressed
  {
   steps(-50);   
  }
  if(ps2x.Button(PSB_BLUE))                                      //Angle increases when X is pressed
  {
   steps(50);
  }


  if(ps2x.Button(PSB_GREEN))
  {
   steps(-50);   
  }
  if(ps2x.Button(PSB_BLUE))
  {
   steps(50);
  }


  if(ps2x.Button(PSB_GREEN))
  {
   steps(-50);   
  }
  if(ps2x.Button(PSB_BLUE))
  {
   steps(50);
  }


  if(ps2x.Button(PSB_GREEN))
  {
   steps(-50);   
  }  
  if(ps2x.Button(PSB_BLUE))
  {
   steps(50);
  }
//Code for changing angle by BALL-SCREW arrangement ends.



//This Code is used for giving X-Y motion to Frisbee by controlling Base Motors.
  int LY;
  if(ps2x.Analog(PSS_LY) <= 120)
  {
   digitalWrite(dir3, LOW);
   digitalWrite(dir4, HIGH);
   LY = ps2x.Analog(PSS_LY);
   LY = map(LY,120,0,0,150);
   analogWrite(pwm3, LY);
   analogWrite(pwm4, LY);
  }
  else if(ps2x.Analog(PSS_LY) >= 140)
  {
   digitalWrite(dir3, HIGH);
   digitalWrite(dir4, LOW);
   LY = ps2x.Analog(PSS_LY);
   LY = map(LY,140,255,0,150);
   analogWrite(pwm3, LY);
   analogWrite(pwm4, LY);
  }  
  else if((ps2x.Analog(PSS_LY)) > 120  && (ps2x.Analog(PSS_LY) < 140))
  {
   digitalWrite(dir3, LOW);
   digitalWrite(dir4, LOW);
   analogWrite(pwm3, 0);
   analogWrite(pwm4, 0);
  }

//Control of Motor 1 & Motor 2 by right joystick(X-axis) of PS2.
  int RX;
  if(ps2x.Analog(PSS_RX) > 120 && ps2x.Analog(PSS_RX) < 140)
  {
   digitalWrite(dir1, LOW);
   digitalWrite(dir2, LOW);
   analogWrite(pwm1, 0);
   analogWrite(pwm2, 0);
  }
  else if(ps2x.Analog(PSS_RX) <= 120)
  {
   digitalWrite(dir1, HIGH);
   digitalWrite(dir2, HIGH);
   RX = ps2x.Analog(PSS_RX);
   RX = map(RX,120,0,0,150);  
   analogWrite(pwm1, RX);
   analogWrite(pwm2, RX);
  }
  else if(ps2x.Analog(PSS_RX) >= 140)
  { 
   digitalWrite(dir1, LOW);
   digitalWrite(dir2, LOW);
   RX = ps2x.Analog(PSS_RX);
   RX = map(RX,140,255,0,150);
   analogWrite(pwm1, RX);
   analogWrite(pwm2, RX);
  } 



//This Code is used for controlling Base Motors .
  if(ps2x.Button(PSB_R1))
  {
//Control of Motor 3 & Motor 4 by left joystick(Y-axis) of PS2.
   int LY;
   if(ps2x.Analog(PSS_LY) <= 120)
   {
    digitalWrite(dir3, LOW);
    digitalWrite(dir4, HIGH);
    LY = ps2x.Analog(PSS_LY);
    LY = map(LY,120,0,0,225);
    analogWrite(pwm3, LY);
    analogWrite(pwm4, LY);
   }
   else if(ps2x.Analog(PSS_LY) >= 140)
   {
    digitalWrite(dir3, HIGH);
    digitalWrite(dir4, LOW);
    LY = ps2x.Analog(PSS_LY);
    LY = map(LY,140,255,0,225);
    analogWrite(pwm3, LY);
    analogWrite(pwm4, LY);
   }  
   else if((ps2x.Analog(PSS_LY)) > 120  && (ps2x.Analog(PSS_LY) < 140))
   {
    digitalWrite(dir3, LOW);
    digitalWrite(dir4, LOW);
    analogWrite(pwm3, 0);
    analogWrite(pwm4, 0);
   }

//Control of Motor 1 & Motor 2 by right joystick(X-axis) of PS2.
   int RX;
   if(ps2x.Analog(PSS_RX) > 120 && ps2x.Analog(PSS_RX) < 140)
   {
    digitalWrite(dir1, LOW);
    digitalWrite(dir2, LOW);
    analogWrite(pwm1, 0);
    analogWrite(pwm2, 0);
   }
   else if(ps2x.Analog(PSS_RX) <= 120)
   {
    digitalWrite(dir1, HIGH);
    digitalWrite(dir2, HIGH);
    RX = ps2x.Analog(PSS_RX);
    RX = map(RX,120,0,0,225);  
    analogWrite(pwm1, RX);
    analogWrite(pwm2, RX);
   }
   else if(ps2x.Analog(PSS_RX) >= 140)
   { 
    digitalWrite(dir1, LOW);
    digitalWrite(dir2, LOW);
    RX = ps2x.Analog(PSS_RX);
    RX = map(RX,140,255,0,225);
    analogWrite(pwm1, RX);
    analogWrite(pwm2, RX);
   }

  }



//Bot Moves either in Clockwise or in Anticlockwise sense when these buttons are pressed.
  if(ps2x.Button(PSB_L1))
  {
      
   if(ps2x.Analog(PSS_RX) <= 120)
   {
    digitalWrite(dir1, HIGH);
    digitalWrite(dir2, LOW);
    digitalWrite(dir3, LOW);
    digitalWrite(dir4, LOW);
    analogWrite(pwm1, 65);
    analogWrite(pwm2, 65);
    analogWrite(pwm3, 65);
    analogWrite(pwm4, 65);
   }

   else if(ps2x.Analog(PSS_RX) >= 140)
   {
    digitalWrite(dir1, LOW);
    digitalWrite(dir2, HIGH);
    digitalWrite(dir3, HIGH);
    digitalWrite(dir4, HIGH);
    analogWrite(pwm1, 65);
    analogWrite(pwm2, 65);
    analogWrite(pwm3, 65);
    analogWrite(pwm4, 65);
   }
//To remain BOT in State of rest.     
   else if(ps2x.Analog(PSS_RX) > 120 && ps2x.Analog(PSS_RX) < 140)
   {
    digitalWrite(dir1, LOW);
    digitalWrite(dir2, LOW);
    digitalWrite(dir3, LOW);
    digitalWrite(dir4, LOW);
    analogWrite(pwm1, 0);
    analogWrite(pwm2, 0);
    analogWrite(pwm3, 0);
    analogWrite(pwm4, 0);
   }

  }

 }
 delay(50);

}
//Function for giving Direction to BALL-SCREW arrangement.
void steps(int stepValue)
{
 if(stepValue >= 0)
  digitalWrite(dirStpr, HIGH); 
 else
  digitalWrite(dirStpr, LOW);

 if(stepValue < 0)
  stepValue *= -1;

 Stepper_angle(stepValue);
}

//Function for giving pitch to BALL-SCREW arrangement.
void Stepper_angle(int angle)
{
 for(int i=0;i<angle;i++)
 {
  digitalWrite(stepsStpr, HIGH);
  delayMicroseconds(500);
  digitalWrite(stepsStpr, LOW);
  delayMicroseconds(500);
 }
}
