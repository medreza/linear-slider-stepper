//Written by Ahmed Reza Rafsanzani (github.com/medreza) for CV. AKSA OETAMA ELECTRONICS
#include <AccelStepper.h>
#include <LiquidCrystal.h>
#define HALFSTEP 8

LiquidCrystal lcd(8, 9, 4, 5, 6, 7);
int lcd_key     = 0;
int adc_key_in  = 0;
#define btnRIGHT  0
#define btnUP     1
#define btnDOWN   2
#define btnLEFT   3
#define btnSELECT 4
#define btnNONE   5

int aStep = 525;
int calMM = 0;
int calStep = aStep/10;
bool inMainMenu = true;
int milimeter = 0;
int menu = 0;
bool calRunOnce = true;
bool moveRunOnce = true;
unsigned long time_now1 = 0, time_now2 = 0, time_now3 = 0 ;

int read_LCD_buttons()
{
 adc_key_in = analogRead(0);
 if (adc_key_in > 1000) return btnNONE;
 if (adc_key_in < 50)   return btnRIGHT;  
 if (adc_key_in < 250)  return btnUP; 
 if (adc_key_in < 350)  return btnDOWN; //default: 450
 if (adc_key_in < 600)  return btnLEFT; //default: 650
 if (adc_key_in < 850)  return btnSELECT;  

 return btnNONE; 
}

byte loading1[] = {0x00,0x00,0x0A,0x04,0x0A,0x00,0x00,0x00};
byte loading2[] = {0x00,0x04,0x04,0x1F,0x04,0x04,0x00,0x00};
byte loading3[] = {0x00,0x04,0x0E,0x1F,0x0E,0x04,0x00,0x00};

// Motor pin definitions
#define motorPin1  A2     // IN1 on the ULN2003 driver 1
#define motorPin2  A3     // IN2 on the ULN2003 driver 1
#define motorPin3  A4     // IN3 on the ULN2003 driver 1
#define motorPin4  A5     // IN4 on the ULN2003 driver 1

AccelStepper stepper1(HALFSTEP, motorPin1, motorPin3, motorPin2, motorPin4);

void setup() {
   Serial.begin(9600);
   lcd.begin(16, 2);
   stepper1.setMaxSpeed(1500.0);
   stepper1.setAcceleration(100.0);
   stepper1.setSpeed(500);
   lcd.setCursor(3,0);
   lcd.print("Memulai.");
   delay(300);
   lcd.setCursor(3,0);
   lcd.print("Memulai..");
   delay(300);
   lcd.setCursor(3,0);
   lcd.print("Memulai...");
   delay(300);
   lcd.clear();
   lcd.createChar(1, loading1);
   lcd.createChar(2, loading2);
   lcd.createChar(3, loading3);
}


void loop() {
  if (menu == 0)
  {
    calRunOnce = true;
    lcd.setCursor(0,1);
    lcd.print("<Gerak");
    lcd.setCursor(11,1);
    lcd.print("Kal.>");
    lcd_key = read_LCD_buttons();
    Serial.println(adc_key_in);
    if (lcd_key == btnLEFT) {
      menu = 1;
      delay(500);
      lcd.clear();
    }
    if (lcd_key == btnRIGHT) {
      menu = 2;
      delay(500);
      lcd.clear();
    }
  }
  
  
  if (menu == 1)
  {
    if (moveRunOnce)
    {
      milimeter = 0;
      moveRunOnce = false;
    }
    if (milimeter < 0) {
      milimeter = 0;
    }
    if (milimeter > 11) {
      milimeter = 11;
    }
    lcd.setCursor(5,0);
    lcd.print("Jarak");
    lcd.setCursor(5,1);
    lcd.print(milimeter);
    lcd.setCursor(7,1);
    lcd.print("mm");
    lcd.setCursor(0,1);
    lcd.print("<");
    stepper1.moveTo(aStep*milimeter);
    stepper1.run();

    if (stepper1.isRunning())
    {
      lcd.setCursor(15,0);
      if(millis() > time_now1 + 600){time_now1 = millis();lcd.write((byte)1);}
      if(millis() > time_now2 + 1000){time_now2 = millis();lcd.write((byte)2);}
      if(millis() > time_now3 + 1400){time_now3 = millis();lcd.write((byte)3);}
    } else {
      lcd.setCursor(15,0);
      lcd.print(" ");
    }
  
 
    lcd_key = read_LCD_buttons();
    if (lcd_key == btnUP) 
    {
      milimeter++;
      delay(500);lcd.clear();
    }
    if (lcd_key == btnDOWN) 
    {
      milimeter--;
      delay(500);lcd.clear();
    }
    if (lcd_key == btnLEFT) 
    {
      menu = 0;
      delay(500);lcd.clear();
    }
  }



  if (menu == 2)
  {
    if (calRunOnce)
    {
      calMM = 0;
      stepper1.setCurrentPosition(0);
      lcd.setCursor(0,0);
      lcd.print("Set posisi saat");
      lcd.setCursor(0,1);
      lcd.print("plat berdempet");
      delay(1000);
      lcd.setCursor(0,1);
      lcd.print("plat berdempet.");
      delay(1000);
      lcd.setCursor(0,1);
      lcd.print("plat berdempet..");
      delay(1000);
      calRunOnce = false;
      lcd.clear();
    }
      lcd.setCursor(3,0);
      lcd.print("Posisi:");
      lcd.setCursor(11,0);
      lcd.print(calMM);
      lcd.setCursor(0,1);
      lcd.print("Select");
      stepper1.moveTo(calMM*calStep);
      stepper1.run();
    
    if (stepper1.isRunning())
    {
      lcd.setCursor(15,0);
      if(millis() > time_now1 + 600){time_now1 = millis();lcd.write((byte)1);}
      if(millis() > time_now2 + 1000){time_now2 = millis();lcd.write((byte)2);}
      if(millis() > time_now3 + 1400){time_now3 = millis();lcd.write((byte)3);}
    } else {
      lcd.setCursor(15,0);
      lcd.print(" ");
    }
    
    lcd_key = read_LCD_buttons();
    if (lcd_key == btnUP) 
    {
      calMM++;
      delay(500);lcd.clear();
    }
    if (lcd_key == btnDOWN) 
    {
      calMM--;
      delay(500);lcd.clear();
    }
    if (lcd_key == btnRIGHT) 
    {
      calMM = calMM + 10;
      delay(500);lcd.clear();
    }
    if (lcd_key == btnLEFT) 
    {
      calMM = calMM - 10;
      delay(500);lcd.clear();
    }
    if (lcd_key == btnSELECT) 
    {
      stepper1.setCurrentPosition(0);
      lcd.setCursor(0,1);
      lcd.print("OK    ");
      delay(500);
      menu = 0;
      delay(500);lcd.clear();
    }
  }
}


