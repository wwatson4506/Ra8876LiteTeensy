/*
An example of the RA8876 screen with the capacitive touch screen that use FT5316
Such as BuyDisplay ER-TFTM070-6
The FT5206 is normally used with the RA8875 screens but it seems all the FT chips work the same...
Get the RA8876_t3 library from: https://github.com/mjs513/Ra8876LiteTeensy/tree/RA8876-redo
Get the FT5206 library from: https://github.com/sumotoy/FT5206
*/
#include <FT5206.h>
#include <RA8876_t3.h>

#define RA8876_CS 10
#define RA8876_RESET 8
#define BACKLITE 7 //My copy of the display is set for external backlight control
RA8876_t3 tft = RA8876_t3(RA8876_CS, RA8876_RESET); //Using standard SPI pins


#define CTP_INT           0    // Use an interrupt capable pin such as pin 2 (any pin on a Teensy)

uint8_t registers[FT5206_REGISTERS];
uint16_t new_coordinates[5][2];
uint16_t old_coordinates[5][2]; 
uint8_t current_touches = 0;
uint8_t old_touches = 0;


FT5206 cts = FT5206(CTP_INT);


void setup() 
{
  Serial.begin(38400);
  long unsigned debug_start = millis ();
  while (!Serial && ((millis () - debug_start) <= 5000)) ;
  Serial.println("RA8876 touchscreen test starting!");
  Serial.print("Compiled ");
  Serial.print(__DATE__);
  Serial.print(" at ");
  Serial.println(__TIME__);

  //I'm guessing most copies of this display are using external PWM
  //backlight control instead of the internal RA8876 PWM.
  //Connect a Teensy pin to pin 14 on the display.
  //Can use analogWrite() but I suggest you increase the PWM frequency first so it doesn't sing.
  pinMode(BACKLITE, OUTPUT);
  digitalWrite(BACKLITE, HIGH);
  
  tft.begin();
  cts.begin();
  tft.setTextColor(0xFFFF,0x0000);
  Serial.println("inited...");
}



void loop() 
{
  //cts.setTouchLimit(1);//from 1 to 5
  if (cts.touched()){
    uint8_t i;
    uint16_t x,y;
    cts.getTSregisters(registers);
    current_touches = cts.getTScoordinates(new_coordinates, registers);
    if (current_touches < 1) return;
    
    for (i = 1; i <= old_touches; i++){// remove previous touches on screen
      tft.fillCircle(old_coordinates[i-1][0], old_coordinates[i-1][1], 70, BLACK);
    }
    
    Serial.print(current_touches);
    Serial.print(" touches: ");
    
    for (i = 1; i <= current_touches; i++){// mark touches on screen
      //Touch panel is upside-down to the screen coordinates, so we must subtract
      x = SCREEN_WIDTH-new_coordinates[i-1][0]-1;
      y = SCREEN_HEIGHT-new_coordinates[i-1][1]-1;
      
      Serial.print(x);
      Serial.print(",");
      Serial.print(y);
      Serial.print("  ");
      
      // Mark touches on screen
      tft.fillCircle(x, y, 70, BLUE);
      tft.fillCircle(x, y, 50, WHITE);
      tft.fillCircle(x, y, 30, WHITE);
      old_coordinates[i-1][0] = x;
      old_coordinates[i-1][1] = y;
    }
    
    old_touches = current_touches;
    Serial.println();
  }

}
