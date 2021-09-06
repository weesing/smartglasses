#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_ST7735.h> // Hardware-specific library for ST7735
#include <Adafruit_ST7789.h> // Hardware-specific library for ST7789
#include <SPI.h>

#define ESP32
#ifdef ESP32
#define TFT_CS 15
#define TFT_RST 4
#define TFT_DC 2
#else
// For the breakout board, you can use any 2 or 3 pins.
// These pins will also work for the 1.8" TFT shield.
#define TFT_CS 10
#define TFT_RST 8 // Or set to -1 and connect to Arduino RESET pin
#define TFT_DC 9
#endif

Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST);

void displaySetup()
{
  tft.initR(INITR_MINI160x80); // Init ST7735S mini display
  tft.setRotation(1);
  tft.invertDisplay(true);
  tft.fillScreen(ST77XX_BLACK);
  // tft.setSPISpeed(100000000);
}

void setup(void)
{
  Serial.begin(9600);

  displaySetup();

  uint16_t time = millis();
  time = millis() - time;
}

#define TEST_DISPLAY

#ifdef TEST_DISPLAY
void testPrintText()
{
  uint16_t colors[] = {ST7735_WHITE, ST7735_YELLOW, ST7735_GREEN, ST7735_RED, ST7735_BLUE};
  for (uint8_t i = 0; i < sizeof(colors) / sizeof(colors[0]); ++i)
  {
    // large block of text
    tft.fillScreen(ST77XX_BLACK);
    tft.setCursor(0, 0);
    tft.setTextColor(colors[i]);
    tft.setTextWrap(true);
    tft.print("Lorem ipsum dolor sit amet, consectetur adipiscing elit. Curabitur adipiscing ante sed nibh tincidunt feugiat. Maecenas enim massa, fringilla sed malesuada et, malesuada sit amet turpis. Sed porttitor neque ut ante pretium vitae malesuada nunc bibendum. Nullam aliquet ultrices massa eu hendrerit. Ut sed nisi lorem. In vestibulum purus a tortor imperdiet posuere.");
    delay(1000);
  }
}

#define R_SIZE 5
#define G_SIZE 6
#define B_SIZE 5
const uint8_t R_MAX = pow(2, R_SIZE);
const uint8_t G_MAX = pow(2, G_SIZE);
const uint8_t B_MAX = pow(2, B_SIZE);
#define TILE_WIDTH 16
#define TILE_HEIGHT 16
#define TILE_DISPLAY_TIMES 30

void testPrintTiles()
{
  for (uint8_t times = 0; times < TILE_DISPLAY_TIMES; ++times)
  {
    for (uint8_t y = 0; y < 80; y += TILE_HEIGHT)
    {
      for (uint8_t x = 0; x < 160; x += TILE_WIDTH)
      {
        uint8_t r = rand() % R_MAX;
        uint8_t g = rand() % G_MAX;
        uint8_t b = rand() % B_MAX;
        uint16_t color = (r << G_SIZE + B_SIZE) | (g << B_SIZE) | b;
        tft.fillRect(x, y, TILE_WIDTH, TILE_HEIGHT, color);
      }
    }
    delay(50);
  }
  delay(2000);
}

#endif

void displayLoop()
{
#ifdef TEST_DISPLAY
  testPrintText();
  testPrintTiles();
#endif
}

void loop()
{
  displayLoop();
}
