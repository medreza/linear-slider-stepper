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

int read_LCD_buttons()
{
 adc_key_in = analogRead(0);
 if (adc_key_in > 1000) return btnNONE;
 if (adc_key_in < 50)   return btnRIGHT;  
 if (adc_key_in < 250)  return btnUP; 
 if (adc_key_in < 450)  return btnDOWN; 
 if (adc_key_in < 650)  return btnLEFT; 
 if (adc_key_in < 850)  return btnSELECT;  

 return btnNONE; 
}


// Motor pin definitions
#define motorPin1  A2     // IN1 on the ULN2003 driver 1
#define motorPin2  A3     // IN2 on the ULN2003 driver 1
#define motorPin3  A4     // IN3 on the ULN2003 driver 1
#define motorPin4  A5     // IN4 on the ULN2003 driver 1

// Initialize with pin sequence IN1-IN3-IN2-IN4 for using the AccelStepper with 28BYJ-48
AccelStepper stepper1(HALFSTEP, motorPin1, motorPin3, motorPin2, motorPin4);

void setup() {
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
}//--(end setup )---

int aStep = 2000;
bool inMainMenu = true;
int milimeter = 0;


void loop() {
  lcd.setCursor(3,0);
  lcd.print(stepper1.currentPosition());
  lcd.setCursor(3,1);
  lcd.print(milimeter);
  if (milimeter < 0) {
    milimeter = 0;
  }
  if (milimeter > 11) {
    milimeter = 11;
  }
  stepper1.moveTo(aStep*milimeter);
  stepper1.run();
  
  lcd_key = read_LCD_buttons();
  if (lcd_key == 1) {
    milimeter++;
    delay(500);
    lcd.clear();
  }
  if (lcd_key == 2) {
    milimeter--;
    delay(500);
    lcd.clear();
  }
}
