#include<XBOXUSB.h>
//define motor 1 pins
#define m1c 2 //direction
#define m1a 3 //pwm

#define m2c 4 //direction
#define m2a 5 //pwm

#define m3c 7 //direction
#define m3a 6 //pwm

#define LEFTHATXMIN -32767 //lowest value of joystick x-axis
#define LEFTHATXMAX 32767 //highest value of joystick x-axis
#define LEFTHATXPOSMED 7500 //higher side median error value for joystick x-axis as suggested by experimental trials by people on the internet
#define LEFTHATXNEGMED -7500 //lower side median error value for joystick x-axis as suggested by experimental trials by people on the internet
#define LEFTHATYPOSMED 7500 //higher side median error value for joystick y-axis as suggested by experimental trials by people on the internet
#define LEFTHATYNEGMED -7500 //higher side median error value for joystick y-axis as suggested by experimental trials by people on the internet
#define LEFTHATYMIN -32767 //lowest value of joystick y-axis
#define LEFTHATYMAX 32767 //highest value of joystick y-axis
/*note: if median value is neglected while creating conditional cases, the input from joystick may result in garbage outputs like -
  motors rotating on their own for infinite period of time.*/
#define MOTORMAX 255 //maximum motor pwm integer value supported by 8-bit Arduino Uno. 0 represents 0% duty cycle and 255 represents 100% duty cycle
#define yawmotormax 100 //maximum motor pwm set only for yawing. Strategically decided to provide precision

int pwm,pwm2,pwm3; //pwm values for motors m1, m2, m3 respectively

USB Usb;
XBOXUSB Xbox(&Usb);
void setup() {
  //pin designation
  pinMode(m1c,OUTPUT);
  pinMode(m1a,OUTPUT);
  pinMode(m2c,OUTPUT);
  pinMode(m2a,OUTPUT);
  pinMode(m3c,OUTPUT);
  pinMode(m3a,OUTPUT);
  
  Serial.begin(9600); //this number needs to be matched with the baud rate of the serial monitor to avoid garbage values
  #if !defined(__MIPSEL__)
  while (!Serial); 
  #endif
  
  if (Usb.Init() == -1) {
  Serial.print(F("\r\nOSC did not start"));
  while (1); 
  }
  Serial.print(F("\r\nXBOX USB Library Started")); //confirmation printed after Bluetooth controller is connected
}   

void loop() 
{
  Usb.Task();
  if (Xbox.Xbox360Connected) {    
    // If Joystick is at central position, stop motors
    // Central position has been given a buffer value ranging from the positive median error value to the negative median error value as mentioned in the defining statements above
    // For y-axis
    if(Xbox.getAnalogHat(LeftHatY)>=0 && Xbox.getAnalogHat(LeftHatY)<=LEFTHATYPOSMED && Xbox.getAnalogHat(LeftHatY)<=0 && Xbox.getAnalogHat(LeftHatY)>=LEFTHATYNEGMED ){
      analogWrite(m1a,0);
      digitalWrite(m1c,LOW);

      analogWrite(m2a,0);
      digitalWrite(m2c,LOW);

      analogWrite(m3a,0);
      digitalWrite(m3c,LOW);
      }
    // For x-axis
    if(Xbox.getAnalogHat(LeftHatX)>=0 && Xbox.getAnalogHat(LeftHatX)<=LEFTHATXPOSMED && Xbox.getAnalogHat(LeftHatX)<=0 && Xbox.getAnalogHat(LeftHatX)>=LEFTHATXNEGMED ){
      analogWrite(m1a,0);
      digitalWrite(m1c,LOW);

      analogWrite(m2a,0);
      digitalWrite(m2c,LOW);

      analogWrite(m3a,0);
      digitalWrite(m3c,LOW);
      }
    // Considering joystick drift values - checking whether Joystick is moved
    if (Xbox.getAnalogHat(LeftHatX) > LEFTHATXPOSMED || Xbox.getAnalogHat(LeftHatX) < LEFTHATXNEGMED || Xbox.getAnalogHat(LeftHatY) > LEFTHATYPOSMED || Xbox.getAnalogHat(LeftHatY) < LEFTHATYNEGMED){
      Serial.println(Xbox.getAnalogHat(LeftHatX));
      Serial.println(Xbox.getAnalogHat(LeftHatY));
      //checking digonal northwest click
      if (Xbox.getAnalogHat(LeftHatX) < 0 && Xbox.getAnalogHat (LeftHatY) > 0){
        Serial.println("calling northwest");
        analogWrite(m1a,MOTORMAX);
        digitalWrite(m1c,LOW);
        
        analogWrite(m2a,0);
        digitalWrite(m2c,LOW);
        
        analogWrite(m3a,MOTORMAX);
        digitalWrite(m3c,HIGH);
       }
       //checking digonal northeast click
       else if (Xbox.getAnalogHat(LeftHatX) > 0 && Xbox.getAnalogHat (LeftHatY) > 0)
       {
        Serial.println("calling northeast");
        analogWrite(m1a,MOTORMAX);
        digitalWrite(m1c,HIGH);
        
        analogWrite(m2a,MOTORMAX);
        digitalWrite(m2c,LOW);
        
        analogWrite(m3a,0);
        digitalWrite(m3c,HIGH);
       }
       //checking digonal southeast click
       else if (Xbox.getAnalogHat(LeftHatX) > 0 && Xbox.getAnalogHat (LeftHatY) < 0)
       {
        Serial.println("calling southeast");
        analogWrite(m1a,MOTORMAX);
        digitalWrite(m1c,HIGH);
        
        analogWrite(m2a,0);
        digitalWrite(m2c,LOW);
        
        analogWrite(m3a,MOTORMAX);
        digitalWrite(m3c,LOW);
       }
       //checking digonal southwest click
       else if (Xbox.getAnalogHat(LeftHatX) < 0 && Xbox.getAnalogHat (LeftHatY) < 0)
       {
        Serial.println("calling southwest");
        analogWrite(m1a,MOTORMAX);
        digitalWrite(m1c,LOW);
        
        analogWrite(m2a,MOTORMAX);
        digitalWrite(m2c,HIGH);
        
        analogWrite(m3a,0);
        digitalWrite(m3c,HIGH);
       }
       //checking Forward, backward, right, left click
       else
       {
      if (Xbox.getAnalogHat(LeftHatY) > LEFTHATYPOSMED)  
       {
        Serial.println("calling forward");
        forward(Xbox.getAnalogHat(LeftHatY));
       }

       if (Xbox.getAnalogHat(LeftHatY) < LEFTHATYNEGMED)  
       {
        Serial.println("calling backward");
        backward(Xbox.getAnalogHat(LeftHatY));
       }

       if (Xbox.getAnalogHat(LeftHatX) > LEFTHATXPOSMED)  
       {
        Serial.println("calling right");
        right(Xbox.getAnalogHat(LeftHatX));
       }

       if (Xbox.getAnalogHat(LeftHatX) < LEFTHATXNEGMED)  
       {
        Serial.println("calling left");
        left(Xbox.getAnalogHat(LeftHatX));
       }
      }
  } // Joystick move check loop end

// Check if analog LT is clicked, if yes, yaw left
 if (Xbox.getButtonPress(L2))
 {
  analogWrite(m1a,yawmotormax);
  digitalWrite(m1c,LOW);

  analogWrite(m2a,yawmotormax);
  digitalWrite(m2c,LOW);

  analogWrite(m3a,yawmotormax);
  digitalWrite(m3c,LOW);
 }

// Check if analog RT is clicked, if yes, yaw right
 if (Xbox.getButtonPress(R2))
  {
    analogWrite(m1a,yawmotormax);
    digitalWrite(m1c,HIGH);

    analogWrite(m2a,yawmotormax);
    digitalWrite(m2c,HIGH);

    analogWrite(m3a,yawmotormax);
    digitalWrite(m3c,HIGH);
  }
 }  // xbox connected loop end
 delay(55);
}// main loop end

void forward(int f)
{
  pwm = map(f,LEFTHATYPOSMED,LEFTHATYMAX,0,MOTORMAX);
  Serial.println("inside forward");
  analogWrite(m1a,0);
  digitalWrite(m1c,LOW);

  analogWrite(m2a,pwm);
  digitalWrite(m2c,LOW);

  analogWrite(m3a,pwm);
  digitalWrite(m3c,HIGH);
}

void backward(int b)
{
  Serial.println("inside backward");
  pwm = map(b,LEFTHATYNEGMED,LEFTHATYMIN,0,MOTORMAX);
  
 analogWrite(m1a,0);
  digitalWrite(m1c,LOW);

  analogWrite(m2a,pwm);
  digitalWrite(m2c,HIGH);

  analogWrite(m3a,pwm);
  digitalWrite(m3c,LOW);
}

void right(int r)
{
  Serial.println("inside right");
  pwm = map(r,LEFTHATXPOSMED,LEFTHATXMAX,0,MOTORMAX);
  pwm2 = pwm3 = (pwm/2);
  
 analogWrite(m1a,pwm);
  digitalWrite(m1c,HIGH);

  analogWrite(m2a,pwm2);
  digitalWrite(m2c,LOW);

  analogWrite(m3a,pwm3);
  digitalWrite(m3c,LOW);
}

void left(int l)
{
  Serial.println("inside left");
  pwm = map(l,LEFTHATXNEGMED,LEFTHATXMIN,0,MOTORMAX);
  pwm2 = pwm3 = (pwm/2);
  
 analogWrite(m1a,pwm);
  digitalWrite(m1c,LOW);

  analogWrite(m2a,pwm2);
  digitalWrite(m2c,HIGH);

  analogWrite(m3a,pwm3);
  digitalWrite(m3c,HIGH);
}
