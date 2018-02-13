/*Scott Clandinin 2018

 */

#include <LiquidCrystal.h>

const int button = 6; 
const int onLED = A5;   
const int offLED = A3; 

int wifi_status;
int internet_status;

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);


//**************************************************
//  setup
//**************************************************
void setup() 
{
  Serial.begin(9600);
  pinMode(onLED, OUTPUT);
  pinMode(offLED, OUTPUT);
  pinMode(button, INPUT_PULLUP);
  
  lcd.begin(16, 2);
  // Print a message to the LCD.
  lcd.print("Connecting...");
}


//**************************************************
//  loop
//**************************************************
void loop() 
{
  int buttonState;
  int wait_flag = 0;
  unsigned long firstTime;
  unsigned long currentTime;

  for (int i = 0; i < 50; i++)      //update the status every 5 seconds
  {
     buttonState = digitalRead(button);

    if (buttonState == LOW) 
    {
      wait_flag = 1;              //once button is released the program will delay for 5 seconds in addition to regular delays
      lcd.clear();
      lcd.print("ID: Middle Earth");
      lcd.setCursor(0, 1);        //second row
      lcd.print("PASS: ForFrodo");
      i=0;                        //reset count so it will wait another 5 seconds before updating
      firstTime = millis();
    } 
    while (buttonState == LOW)
    {
      currentTime = millis();
      if (currentTime - firstTime > 5000)
      {
        light_demo();
      }
      buttonState = digitalRead(button);
    }
  
    if (wait_flag == 1) 
    {
      delay(5000);   //together with the count resetting makes 10 seconds of Wifi info displayed after releasing button
      wait_flag = 0; //reset wait flag
    }
  
    delay(100);
  
  
  }
  update_status();
}



//**************************************************
//  update_status
//**************************************************
void update_status()
{
  wifi_status = check_wifi();
  internet_status = check_internet();
  
  if (wifi_status == 1 && internet_status == 1)
  {
    digitalWrite(onLED, HIGH);
    digitalWrite(offLED, LOW);
    lcd.clear();
    lcd.print("WiFi Connected");
    lcd.setCursor(0, 1);        //second row
    lcd.print("Internet Access");
  }
  else if (wifi_status == 1 && internet_status == 0)
  {
    digitalWrite(onLED, LOW);
    digitalWrite(offLED, HIGH);
    lcd.clear();
    lcd.print("WiFi Connected");
    lcd.setCursor(0, 1);        //second row
    lcd.print("Internet Down");
  }
  else
  {
    digitalWrite(onLED, LOW);
    digitalWrite(offLED, HIGH);
    lcd.clear();
    lcd.print("WiFi Down");
    lcd.setCursor(0, 1);        //second row
    lcd.print("Internet Down");
  }
  
}


//**************************************************
//  check_wifi
//**************************************************
int check_wifi()
{
  String response;
  Serial.println("AT+CWJAP?");    //connect module to user wifi
  while(Serial.available() == 0);
  response = Serial.readString();
  
  if(response.indexOf("OK") > 0)
  {
    return 1;
  }
  else
  { 
    return 0;
  }  
}


//**************************************************
//  check_internet
//**************************************************
int check_internet()
{
  String response, string_buffer;
  
  Serial.println("AT+CIPSTART=\"TCP\",\"google.com\",80");    //connect module to googles server
  while(Serial.available() == 0);
  response = Serial.readString();
  delay(50);

  Serial.println("AT+CIPCLOSE");   //close connection
  while(Serial.available() == 0);
  string_buffer = Serial.readString();
  delay(50);

  if(response.indexOf("OK") > 0)
  {
    return 1;
  }
  else
  {
    return 0;
  }  
}



//**************************************************
//  light_demo
//**************************************************
void light_demo()
{
  lcd.clear();
  lcd.print("Made by:");
  lcd.setCursor(0, 1);        //second row
  lcd.print("Scott Clandinin");
  
  for (int i = 0; i < 5; i++)
  {
    digitalWrite(onLED, LOW);
    digitalWrite(offLED, LOW);
    delay(500);
    digitalWrite(onLED, LOW);
    digitalWrite(offLED, HIGH);
    delay(500);
    digitalWrite(onLED, HIGH);
    digitalWrite(offLED, LOW);
    delay(500);
    digitalWrite(onLED, HIGH);
    digitalWrite(offLED, HIGH);
    delay(500);
    digitalWrite(onLED, LOW);
    digitalWrite(offLED, LOW);
    delay(500);
  }
}

