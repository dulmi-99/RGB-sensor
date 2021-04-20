#include <Keypad.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

//const byte ROWS = 4; //four rows
//const byte COLS = 3; //four columns
char deciKeys[4][4] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};
byte rowPins[4] = {2, 3, 4, 5}; //connect to the row pinouts of the keypad
byte colPins[4] = {6, 7, 8, 12}; //connect to the column pinouts of the keypad
Keypad customKeypad = Keypad( makeKeymap(deciKeys), rowPins, colPins, 4, 4);
LiquidCrystal_I2C lcd(0x3F, 16, 2);  // set the LCD address to 0x27 for a 16 chars and 2 line display
int green = 10;
int red = 9;
int blue = 11;

int green_ = A2;
int red_ = A3;
int blue_ = A1;
void setup() {
  //getting LDR readings...

  pinMode(red, OUTPUT);
  pinMode(green, OUTPUT);
  pinMode(blue, OUTPUT);
  pinMode(red_, OUTPUT);
  pinMode(green_, OUTPUT);
  pinMode(blue_, OUTPUT);
  Serial.begin(9600);
  //display values in LCD display
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("1)Mode1");  // calibration
  lcd.setCursor(8, 0);
  lcd.print("2)Mode2");  // sensor
  lcd.setCursor(4, 1);
  lcd.print("3)Mode3");  // keyinput
}

char mode_input[1];
void loop() {
  char mode = customKeypad.getKey();

  Serial.print(mode);
  if (mode == '2') {                        // mode 2

    while (true) {
      lcd.clear();
      analogWrite(green, 255);
      delay(300);
      int gr_avg = average(get_reading(A0));
      gr_avg = int_to_byte(map(gr_avg, 140, 350, 0, 255));
      analogWrite(green, 0);

      analogWrite(red, 255);
      delay(300);
      int rd_avg = average(get_reading(A0));
      rd_avg = int_to_byte(map(rd_avg, 60, 160, 0, 255));
      analogWrite(red, 0);

      analogWrite(blue, 255);
      delay(300);
      int bl_avg = average(get_reading(A0));
      bl_avg = int_to_byte( map(bl_avg, 210, 490, 0, 255));
      analogWrite(blue, 0);

      //byte rd_avg_new,gr_avg_new, bl_avg_new = byte(develop(rd_avg,gr_avg,bl_avg)); //if want some pushup for values...!
      lcd.setCursor(0, 0);
      lcd.print("R-");
      lcd.setCursor(2, 0);
      lcd.print(rd_avg);

      lcd.setCursor(10, 0);
      lcd.print("G-");
      lcd.setCursor(12, 0);
      lcd.print(gr_avg);

      lcd.setCursor(6, 1);
      lcd.print("B-");
      lcd.setCursor(8, 1);
      lcd.print(bl_avg);


      Serial.print("Green :");
      Serial.print(gr_avg);
      Serial.print(" RED :");
      Serial.print(rd_avg);
      Serial.print(" Blue :");
      Serial.print(bl_avg);
      Serial.println("");
      analogWrite(red_, 255 - rd_avg);
      analogWrite(green_, 255 - gr_avg);
      analogWrite(blue_, 255 - bl_avg);
      delay(500);
    }
  }

  else if (mode == '1') {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Calibration");
    //min segment
    lcd.setCursor(0, 1);
    lcd.print("Min");
    lcd.setCursor(5, 1);
    lcd.print("R-");
    char R = customKeypad.getKey();
    if (R) {
      lcd.print(R);
      Serial.println(R);
    }

    lcd.setCursor(5, 1);
    lcd.print("G-");
    char G = customKeypad.getKey();
    if (G) {
      lcd.print(G);
    }

    lcd.setCursor(5, 1);
    lcd.print("B-");
    char B = customKeypad.getKey();
    if (B) {
      lcd.print(B);
    }

  }
}

int average(int list[]) {
  int sum = 0;
  for (int i = 0; i < sizeof(list); i++) {
    sum += list[i];
  }
  return sum / sizeof(list);
}
int* get_reading(int pin) {
  int list[10];
  for (int i = 0; i < 10; i++) {
    list[i] = analogRead(pin);
    delay(25);
  }
  return list;
}
int* develop(byte r, byte g, byte b) {
  if (r < g and b < g) {
    r = r - 50;
    b = b - 50;
    g = g + 50;

  }
  else if (b < r and g < r) {
    r = r + 50;
    b = b - 50;
    g = g - 50;

  }
  else if (r < b and g < b) {
    r = r - 50;
    b = b + 50;
    g = g - 50;

  }
  return r, g, b;
}
byte  int_to_byte(int val) {
  if (val < 255) {
    return byte(val);
  }
  else {
    return 255;
  }
}
