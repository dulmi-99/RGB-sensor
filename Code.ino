#include<Wire.h>
#include<LiquidCrystal_I2C.h>
#include <Keypad.h>

LiquidCrystal_I2C lcd(0x3F, 16, 2);

byte rowPins[4] = {2, 3, 4, 5};
byte colPins[4] = {6, 7, 8, 12};

char keys[4][4] {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};
//constructor
Keypad key_input = Keypad(makeKeymap(keys), rowPins, colPins, 4, 4);

int R_in, B_in, G_in;

int led_r = 9;
int led_g = 10;
int led_b = 11;

int out_r = A3;
int out_g = A2;
int out_b = A1;


void setup() {
  Serial.begin(9600);

  // for the sensor part output
  pinMode(led_r, OUTPUT);
  pinMode(led_g, OUTPUT);
  pinMode(led_b, OUTPUT);

  //lcd display
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("1)Mode1"); // sensoring
  lcd.setCursor(8, 0);
  lcd.print("2)Mode2"); // key input
  lcd.setCursor(5, 1);
  lcd.print("3)Mode3");  // callibration

}

void loop() {
  char mode = key_input.getKey();
  if (mode == '1') {
    while (true) {
      analogWrite(led_r, 255);
      analogWrite(led_g, 0);
      analogWrite(led_b, 0);
      R_in = avr_values();
      //R_in = map(R_in,0,1023,0,255);
      R_in = int_to_byte(map(R_in, 28, 392, 0, 255));
      delay(300);

      analogWrite(led_r, 0);
      analogWrite(led_g, 255);
      analogWrite(led_b, 0);
      G_in = avr_values();
      //G_in = map(G_in,0,1023,0,255);
      G_in = int_to_byte(map(G_in, 141, 682, 0, 255));
      delay(300);

      analogWrite(led_r, 0);
      analogWrite(led_g, 0);
      analogWrite(led_b, 255);
      B_in = avr_values();
      //B_in = map(B_in,0,1023,0,255);
      B_in = int_to_byte(map(B_in, 152, 632, 0, 255));
      delay(300);

      //output the colour
      analogWrite(out_r, R_in);
      analogWrite(out_g, G_in);
      analogWrite(out_b, B_in);
      delay(500);
      analogWrite(out_r, 0);
      analogWrite(out_g, 0);
      analogWrite(out_b, 0);

      //Display on lcd monitor
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("R: ");
      lcd.setCursor(3, 0);
      lcd.print(R_in);

      lcd.setCursor(10, 0);
      lcd.print("G: ");
      lcd.setCursor(13, 0);
      lcd.print(G_in);

      lcd.setCursor(5, 1);
      lcd.print("B: ");
      lcd.setCursor(8, 1);
      lcd.print(B_in);
      delay(700);
    }
  }

  else if (mode == '2') {
    while (true) {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Input RGB values");
      delay(1000);
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("R:");
      delay(2000);
      char R1 = key_input.getKey();
      lcd.setCursor(0, 1);
      lcd.print(int(R1));
      delay(1000);
      lcd.clear();
      char R2 = key_input.getKey();
      lcd.setCursor(0, 1);
      lcd.print(int(R2));
      delay(1000);
      lcd.clear();
      char R3 = key_input.getKey();
      lcd.setCursor(0, 1);
      lcd.print(int(R3));
      delay(1000);
      lcd.clear();
      R_in = int(R1) * 100 + int(R2) * 10 + int(R3);
      lcd.clear();

      lcd.setCursor(0, 0);
      lcd.print("G:");
      delay(2000);
      char G1 = key_input.getKey();
      char G2 = key_input.getKey();
      char G3 = key_input.getKey();
      G_in = int(G1) * 100 + int(G2) * 10 + int(G3);
      lcd.clear();

      lcd.setCursor(0, 0);
      lcd.print("B:");
      delay(2000);
      char Bo = key_input.getKey();
      char B2 = key_input.getKey();
      char B3 = key_input.getKey();
      B_in = int(Bo) * 100 + int(B2) * 10 + int(B3);
      lcd.clear();

      //output the colour
      analogWrite(out_r, R_in);
      analogWrite(out_g, G_in);
      analogWrite(out_b, B_in);
      delay(500);
      analogWrite(out_r, 0);
      analogWrite(out_g, 0);
      analogWrite(out_b, 0);

      //Display on lcd monitor
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("R: ");
      lcd.setCursor(3, 0);
      lcd.print(R_in);

      lcd.setCursor(10, 0);
      lcd.print("G: ");
      lcd.setCursor(13, 0);
      lcd.print(G_in);

      lcd.setCursor(5, 1);
      lcd.print("B: ");
      lcd.setCursor(8, 1);
      lcd.print(B_in);
      delay(700);

      break;
    }
  }
  else {
  }


  // Dispaly on serial monitor
  //  Serial.print("R: ");
  //  Serial.println(R_in);
  //  Serial.print("G: ");
  //  Serial.println(G_in);
  //  Serial.print("B: ");
  //  Serial.println(B_in);



}

int avr_values() { // to take the average of 10 readings
  float total = 0.0;
  for (int i = 0; i < 10; i++) {
    total = total + analogRead(A0);
    delay(25);
  }
  total = total / 10;
  return (total);
}

byte  int_to_byte(int val) {
  if (val < 255) {
    return byte(val);
  }
  else {
    return 255;
  }
}
