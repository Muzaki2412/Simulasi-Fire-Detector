#include <LiquidCrystal.h>

//For LCD:
char normValues1[]= "LOW GAS,LOW TEMP";
char normValues2[]= "GAS:300,TEMP:350";

char normValuesButHighTemp1[]= "LOW GAS,HIGH TEMP";
char normValuesButHighTemp2[]= "GAS:300,TEMP:300";

char elevatedValuesWithHighTemp1[]= "HIGH GAS,HIGH TEMP";
char elevatedValuesWithHighTemp2[]= "GAS:300,TEMP:301";

char elevatedValuesNormTemp1[]= "HIGH GAS,LOW TEMP";
char elevatedValuesNormTemp2[]= "GAS:300,TEMP:301";

char critical1[]="FIRE DETECTED!";
char critical2[]="GAS:324,TEMP:122";



//define LCD pins:
//LiquidCrystal lcd(RS, E, DB4, DB5, DB6, DB7);
LiquidCrystal lcd(12, 11, 3, 4, 9, 10);

//Various constants for input:
const int gas_sens = A5; //Gassensorpin
const int tmp = A2; //Temperaturepin
const int buzz = 8; //Buzzerpin.
const int button = 2;


//Various varibles for input:
int LEDs[] = {7,6,5}; //Greed LED: 7, Yellow LED: 6, Red LED: 5
int temp;
int gas_konc;
int vol = 0; //knobvalue from potentiometer.

//The interrupt sequence activated by the pushbutton:
void interruptHandler()
{
  noTone(buzz); //Shuts off the alarm.
  delay(2000); //Waiting 2 sec. for the smoke to clear.
  temp = 0;
  gas_konc = 0; 
}

void setup() 
{
  //Serial.begin(9600);
  lcd.begin(16,2);
  
  for(char i = 0; i<3;i++) 
  {
    //Loop to make sure the LEDs are off at the start.
    pinMode(LEDs[i], OUTPUT);
    digitalWrite(LEDs[i], LOW);
  }
  pinMode(button, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(button),interruptHandler, HIGH);
 
  
}

void loop()
{
  //Initiating the global varibles for the gaskoncentration and temperature.
  temp = 0; 
  gas_konc = 0;
  
  //*******GAS***********************
  for(char j = 0; j<10; j++)
  {
    //Collecting 10 samples to calculate mean value
    gas_konc = gas_konc + analogRead(gas_sens);
    delay(100);
  }
  
  gas_konc = gas_konc/10;
  
  //******TEMP********************
  
  for(char m = 0; m<10; m++)
  {
    //Collecting 10 samples to calculate mean value
    temp = temp + analogRead(tmp);
    delay(100);
  }
  temp = temp/10;
  
  //*******val*********************
  if(gas_konc < 300)
  {
    //Normal values, Green LED -> On. All others are off.
    for(char k = 1; k<3;k++)
    {
      digitalWrite(LEDs[k], LOW);
    }
    
    digitalWrite(LEDs[0], HIGH); 
    
    //Check if temperatur is abnormal.
    if(temp > 190)
    {
      lcd.setCursor(0,0);
      lcd.print(normValuesButHighTemp1);
      lcd.setCursor(0,1);
      lcd.print(normValuesButHighTemp2);
      
      //resets the varables:
      temp = 0;
      gas_konc = 0;
    }
    
    else
    {
      
      //resets the varables:
      temp = 0;
      gas_konc = 0;
      
      lcd.setCursor(0,0);
      lcd.print(normValues1);
      lcd.setCursor(0,1);
      lcd.print(normValues2);
    }
  }
  
  else if(gas_konc > 300 && gas_konc <500)
  {
    //Heightened state of alert, Yellow LED -> On. All other LEDs are off.
    digitalWrite(LEDs[0], LOW);
    digitalWrite(LEDs[2], LOW);
    digitalWrite(LEDs[1], HIGH);
    
    //Check if temperatur is dangerously high
    if(temp > 190 && temp < 250)
    {
      lcd.setCursor(0,0);
      lcd.print(elevatedValuesWithHighTemp1);
      lcd.setCursor(0,1);
      lcd.print(elevatedValuesWithHighTemp2);
      
      //resets the varables:
      temp = 0;
      gas_konc = 0;
    }
    
    else
    {
      lcd.setCursor(0,0);
      lcd.print(elevatedValuesNormTemp1);
      lcd.setCursor(0,1);
      lcd.print(elevatedValuesNormTemp2);
      
      //resets the varables:
      temp = 0;
      gas_konc = 0;
    }
  }
  
  while (gas_konc > 500)
  {
    //Critical state of alert, Red LED -> On, Buzzeralarm -> On. All other LEDs are off.
    //Resetbutton needs to be pressed in order for the alarm to power off.
    
    for(char l = 0; l<2;l++)
    {
      digitalWrite(LEDs[l], LOW);
    }
    
    lcd.setCursor(0,0);
    lcd.print(critical1);
    lcd.setCursor(0,1);
    lcd.print(critical2);
    
    digitalWrite(LEDs[2], HIGH);
    tone(buzz,262);//Activate alarm. 
  }
  
}