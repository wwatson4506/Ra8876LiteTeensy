#include <Adafruit_GFX.h>

#include <SPI.h>
#include "RA8876_t3.h"
#define RA8876_CS 10
#define RA8876_RESET 9
#define BACKLITE 7 //External backlight control connected to this Arduino pin

#include "font_Arial.h"
#include "font_ArialBold.h"
#include "font_ComicSansMS.h"
#include "font_OpenSans.h"
#include "font_DroidSans.h"
#include "font_Michroma.h"
#include "font_Crystal.h"
#include "font_ChanceryItalic.h"

// maybe a few GFX FOnts?
#include <Fonts/FreeMonoBoldOblique12pt7b.h>
#include <Fonts/FreeSerif12pt7b.h>

typedef struct {
  const ILI9341_t3_font_t *ili_font;
  const GFXfont       *gfx_font;
  const char          *font_name;
  uint16_t            font_fg_color;
  uint16_t            font_bg_color;
} ili_fonts_test_t;

const uint16_t  PINK       = 0xFCFF; // M.Sandercock
const uint16_t  PURPLE       = 0x8017; // M.Sandercock

const ili_fonts_test_t font_test_list[] = {
  {nullptr, nullptr,  "Internal Font", RED, YELLOW},
  {&Arial_14, nullptr,  "Arial_14", WHITE, WHITE},
  {&Arial_14_Bold, nullptr,  "ArialBold 14", YELLOW, YELLOW},
  {&ComicSansMS_14, nullptr,  "ComicSansMS 14", GREEN, GREEN},
  {&DroidSans_14, nullptr,  "DroidSans_14", WHITE, WHITE},
  {&Michroma_14, nullptr,  "Michroma_14", YELLOW, YELLOW},
  {&Crystal_24_Italic, nullptr,  "CRYSTAL_24", BLACK, YELLOW},
  {&Chancery_24_Italic, nullptr,  "Chancery_24_Italic", GREEN, GREEN},
  {&OpenSans24, nullptr,  "OpenSans 18", RED, YELLOW},
  {nullptr, &FreeMonoBoldOblique12pt7b,  "GFX FreeMonoBoldOblique12pt7b", WHITE, WHITE},
  {nullptr, &FreeMonoBoldOblique12pt7b,  "GFX FreeMonoBoldOblique12pt7b", RED, YELLOW},
  {nullptr, &FreeSerif12pt7b,  "GFX FreeSerif12pt7b", WHITE, WHITE},
  {nullptr, &FreeSerif12pt7b,  "GFX FreeSerif12pt7b", RED, YELLOW},

} ;

extern void displayStuff(const char *font_name);


RA8876_t3 tft = RA8876_t3(RA8876_CS, RA8876_RESET); //Using standard SPI pins

uint8_t test_screen_rotation = 0;


void setup() {
  Serial.begin(38400);
  long unsigned debug_start = millis ();
  while (!Serial && ((millis () - debug_start) <= 5000)) ;
  Serial.println("Setup");
  tft.begin(30000000);
  tft.backlight(true);
  pinMode(BACKLITE, OUTPUT);
  digitalWrite(BACKLITE, HIGH);

//  tft.setRotation(4);
  tft.fillScreen(BLACK);
  tft.setTextColor(WHITE);
  tft.setFont(Arial_14);
  displayStuff("Arial_14");

  tft.setTextColor(YELLOW);
  tft.setFont(Arial_14_Bold);
  displayStuff("ArialBold 14");

  tft.setTextColor(GREEN);
  tft.setFont(ComicSansMS_14);
  displayStuff("ComicSansMS 14");

  nextPage();
  tft.setTextColor(WHITE);
  tft.setFont(DroidSans_14);
  displayStuff("DroidSans_14");

  tft.setTextColor(YELLOW);
  tft.setFont(Michroma_14);
  displayStuff("Michroma_14");
  stepThrough();

  tft.setTextColor(BLACK, YELLOW);
  tft.setFont(Crystal_24_Italic);
  displayStuff("CRYSTAL_24");

  nextPage();

  tft.setTextColor(GREEN);
  tft.setFont(Chancery_24_Italic);
  displayStuff("Chancery_24_Italic");

  //anti-alias font OpenSans
  tft.setTextColor(RED, YELLOW);
  tft.setFont(OpenSans24);
  displayStuff("OpenSans24");

  Serial.println("Basic Font Display Complete");
  Serial.println("Loop test for alt colors + font");
}

void loop()
{
  Serial.printf("\nRotation: %d\n", test_screen_rotation);
  tft.setRotation(test_screen_rotation);
  tft.fillScreen(RED);
  tft.setCursor(tft.width()/2, tft.height()/2, true);
  tft.printf("Rotation: %d", test_screen_rotation);
  test_screen_rotation = (test_screen_rotation + 1) & 0x3;
  tft.setCursor(200, 300);
  Serial.printf("  Set cursor(200, 300), retrieved(%d %d)",
                tft.getCursorX(), tft.getCursorY());
  tft.setCursor(50, 50);
  tft.write('0');
  tft.setCursor(tft.width() - 50, 50);
  tft.write('1');
  tft.setCursor(50, tft.height() - 50);
  tft.write('2');
  tft.setCursor(tft.width() - 50, tft.height() - 50);
  tft.write('3');

  for (uint8_t font_index = 0; font_index < (sizeof(font_test_list) / sizeof(font_test_list[0])); font_index++) {
    nextPage();
    if (font_test_list[font_index].font_fg_color != font_test_list[font_index].font_bg_color)
      tft.setTextColor(font_test_list[font_index].font_fg_color, font_test_list[font_index].font_bg_color);
    else
      tft.setTextColor(font_test_list[font_index].font_fg_color);
    if (font_test_list[font_index].ili_font) tft.setFont(*font_test_list[font_index].ili_font);
    else if (font_test_list[font_index].gfx_font)  tft.setFont(font_test_list[font_index].gfx_font);
    else tft.setFontDef();
    tft.println(font_test_list[font_index].font_name);
    displayStuff1();
  }
  nextPage();
}

void displayStuff(const char *font_name)
{
  elapsedMillis elapsed_time = 0;
  tft.println(font_name);
  tft.println("ABCDEFGHIJKLMNOPQRSTUVWXYZ");
//  stepThrough();
  tft.println("abcdefghijklmnopqrstuvwxyz");
//  stepThrough();
  tft.println("0123456789");
//  stepThrough();
  tft.println("!@#$%^ &*()-");
//  stepThrough();
  tft.println(); tft.println();
//  stepThrough();
  Serial.printf("%s: %dms\n", font_name, (uint32_t)elapsed_time);
}

uint32_t displayStuff1()
{
  elapsedMillis elapsed_time = 0;
  tft.println("ABCDEFGHIJKLMNOPQRSTUVWXYZ");
  tft.println("abcdefghijklmnopqrstuvwxyz");
  tft.println("0123456789");
  tft.println("!@#$%^ &*()-");

  int16_t cursorX = tft.getCursorX();
  int16_t cursorY = tft.getCursorY();

  uint16_t width = tft.width();
  uint16_t height = tft.height();
  Serial.printf("DS1 (%d,%d) %d %d\n", cursorX, cursorY, width, height);
  uint16_t rect_x = width / 2 - 50;
  uint16_t rect_y = height - 75;
  tft.drawRect(rect_x, rect_y, 100, 50, WHITE);
  for (uint16_t y = rect_y + 5; y < rect_y + 50; y += 5)
    tft.drawFastHLine(rect_x + 1, y, 98, PINK);
  for (uint16_t x = rect_x + 5; x < rect_x + 100; x += 5)
    tft.drawFastVLine(x, rect_y+1, 48, PINK);
  tft.setCursor(width / 2, height - 50, true);
  tft.print("Center");

  // Lets try again with CENTER X keyword.
  rect_y -= 100;
  tft.drawRect(rect_x, rect_y, 100, 50, PINK);
  for (uint16_t y = rect_y + 5; y < rect_y + 50; y += 5)
    tft.drawFastHLine(rect_x + 1, y, 98, CYAN);
  for (uint16_t x = rect_x + 5; x < rect_x + 100; x += 5)
    tft.drawFastVLine(x, rect_y+1, 48, CYAN);
  tft.setCursor(CENTER, rect_y);
  tft.print("XCENTR");

  // Lets try again with CENTER Y keyword.
  rect_x = 50;
  rect_y = tft.height()/2 -25;
  tft.drawRect(rect_x, rect_y, 100, 50, CYAN);
  for (uint16_t y = rect_y + 5; y < rect_y + 50; y += 5)
    tft.drawFastHLine(rect_x + 1, y, 98, PINK);
  for (uint16_t x = rect_x + 5; x < rect_x + 100; x += 5)
  tft.setCursor(50, CENTER);
  tft.print("YCENTR");
  
  // Lets see how close the getTextBounds gets the bounds of the text
  rect_x = 200;
  rect_y += 25; //center 
  static const char rectText[] = "RectText";
  int16_t xT, yT;
  uint16_t wT, hT;
  tft.getTextBounds(rectText, rect_x, rect_y, &xT, &yT, &wT, &hT);
  Serial.printf("getTextBounds: (%d, %d): %d %d %d %d\n", rect_x, rect_y, xT, yT, wT, hT);
  tft.setCursor(rect_x, rect_y);
  tft.print(rectText);
  tft.drawRect(xT, yT, wT, hT, CYAN);

  tft.setCursor(cursorX, cursorY);
  static const char alternating_text[] = "AbCdEfGhIjKlMnOpQrStUvWxYz\raBcDeFgHiJkLmNoPqRsTuVwXyZ";

  for (uint8_t i = 0; i < sizeof(alternating_text); i++) {
    if (i & 1) tft.setTextColor(WHITE, RED);
    else tft.setTextColor(YELLOW, BLUE);
    tft.write(alternating_text[i]);
  }

  tft.println(); tft.println();



  return (uint32_t) elapsed_time;
}

void nextPage()
{
  Serial.println("Press anykey to continue");
  while (Serial.read() == -1) ;
  while (Serial.read() != -1) ;

  tft.fillScreen(BLACK);
  tft.setCursor(0, 0);
}
void stepThrough()
{
  Serial.println("Press anykey to continue");
  while (Serial.read() == -1) ;
  while (Serial.read() != -1) ;
}
