//I'm gonna call this bitch Alice

// include the library code:
#include <SoftwareSerial.h>

#define Alice    0                     
#define Bob      1                     
SoftwareSerial Xbee (Alice, Bob);

//Variables:
int sensorVal;
int sensorVal2;
bool started = false;  //True: Message is started
bool ended = false;    //True: Message is finished
char msg[3];           //Message - array from 0 to 2 (3 values - PWM - e.g. 240)
byte index;            //Index of array
char incomingByte;     //Variable to store the incoming byte


void setup() {

  //Start the serial communication
  Serial.begin(9600); //Baud rate must be the same as is on xBee module
  Xbee.begin(9600);
  pinMode(6,OUTPUT);
  pinMode(5, OUTPUT);
  }

void loop() {

    //-------------------TRANSMISSION CODE--------------------------
    
    //Read the analog value from pot and store it to "value" variable
    sensorVal = analogRead(A0);
    delay(2);
    sensorVal2 = analogRead(A3);


if(sensorVal < 5 && sensorVal2 < 5)
  {
      Serial.print('<');  //Starting symbol
      Serial.print(1);//
      Serial.println('>');//Ending symbol
  }
  else if (sensorVal < 5) // If the analog reading is non-zero
  {
      Serial.print('<');  //Starting symbol
      Serial.print(2);//
      Serial.println('>');//Ending symbol
  }
  
  else if (sensorVal2 < 5)
  {
      Serial.print('<');  //Starting symbol
      Serial.print(3);//
      Serial.println('>');//Ending symbol
  }
    else
  {
      Serial.print('<');  //Starting symbol
      Serial.print(0);//
      Serial.println('>');//Ending symbol
  }

 //-------------------------RECIEVER CODE-----------------------------
 while(Xbee.available()>0)
 {
  
  //Read the incoming byte
  incomingByte = Xbee.read();

  //Start the message when the '<' symbol is recieved
  if(incomingByte == '<')
  {
    started = true;
    index = 0;
    msg[index] = '\0'; //Throw away any incomplete packet
  }

  //End the message when the '>' symbol is recieved
  else if(incomingByte == '>')
  {
    ended = true;
    break; //Done reading - exit from while loop
  }

 //Read the message
 else
 {
   if(index < 4) //Make sure there is room
   {
    msg[index] = incomingByte; //Add char to array
    index++;
    msg[index] = '\0'; // Add NULL to end
    } 
  }
 }

 if(started && ended)
 {
  int value = atoi(msg);

  if(value == 1)
  {
    digitalWrite(6,HIGH);
    digitalWrite(5,HIGH);
  }
  else if(value == 2)
  {
   digitalWrite(6,HIGH); 
   digitalWrite(5,LOW);
  }  
  else if(value == 3)
  {
    digitalWrite(5,HIGH);
    digitalWrite(6,LOW);
  }
  else
  {
    digitalWrite(6,LOW);
    digitalWrite(5,LOW);
  }
  index = 0;
  msg[index] = '\0';
  started = false;
  ended = false;
 } 
}
