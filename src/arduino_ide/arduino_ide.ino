#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_ST7735.h> // Hardware-specific library for ST7735
#include <Adafruit_ST7789.h> // Hardware-specific library for ST7789
#include <SPI.h>
#include <ArduinoJson.h>

// Uncomment the following for ESP32
#define ESP32

#ifdef ESP32
#define TFT_CS 15
#define TFT_RST 4
#define TFT_DC 2
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>
#define SERVICE_UUID "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
#define CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a8"
BLECharacteristic *gp_Characteristics = NULL;
#else
// For the breakout board, you can use any 2 or 3 pins.
// These pins will also work for the 1.8" TFT shield.
#define TFT_CS 10
#define TFT_RST 8 // Or set to -1 and connect to Arduino RESET pin
#define TFT_DC 9
#endif

// #define TEST_DISPLAY

Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST);

#define MSG_TYPE_TEXT 1

void displaySetup()
{
  tft.initR(INITR_MINI160x80); // Init ST7735S mini display
  tft.setRotation(1);
  tft.invertDisplay(true);
  tft.fillScreen(ST7735_BLACK);
  // tft.setSPISpeed(100000000);
}

#ifdef ESP32
class CharacteristicCallbacks : public BLECharacteristicCallbacks
{
  void onRead(BLECharacteristic *pCharacteristic) {}

  void onWrite(BLECharacteristic *pCharacteristic)
  {
    Serial.println("On Write");
    String buffer = String(pCharacteristic->getValue().c_str());

    Serial.println(buffer);
    // TODO: Process buffer from JSON string in buffer
    pCharacteristic->setValue("");

    StaticJsonDocument<200> doc;
    // Deserialize the JSON document
    DeserializationError error = deserializeJson(doc, buffer);

    // Test if parsing succeeds.
    if (error)
    {
      displayMessage(F("System"), F("deserializeJson() failed"));
      Serial.println(error.f_str());
    }
    else
    {
      uint32_t type = doc["type"];
      String title = doc["title"];
      String content = doc["content"];
      if (type != NULL)
      {
        switch (type)
        {
        case MSG_TYPE_TEXT:
          displayMessage(title, content);
          break;
        default:
          displayMessage(F("System"), F("Unknown message type encountered. Aborting display."));
          break;
        }
      }
      else
      {
        Serial.println(F("Unknown message type. Aborting parsing."));
      }
    }

    delay(200);
  }

  void onNotify(BLECharacteristic *pCharacteristic)
  {
    Serial.println("On Notify");
  }

  void onStatus(BLECharacteristic *pCharacteristic, Status s, uint32_t code) {}
};
#endif // #ifdef ESP32

void displayMessage(const String title, const String content)
{
  Serial.println(title);
  Serial.println(content);
  tft.fillScreen(ST7735_BLACK);
  tft.setCursor(0, 0);
  tft.setTextColor(ST7735_YELLOW);
  tft.setTextWrap(true);
  tft.print(title.c_str());
  tft.setCursor(0, 20);
  tft.setTextColor(ST7735_WHITE);
  tft.print(content.c_str());
}

void bleSetup()
{
#ifdef ESP32
  Serial.println("Starting BLE work!");

  BLEDevice::init("ESP32 Smart Glasses");
  // Create server
  BLEServer *pServer = BLEDevice::createServer();
  // Create service
  BLEService *pService = pServer->createService(SERVICE_UUID);
  // Create characteristic
  gp_Characteristics = pService->createCharacteristic(
      CHARACTERISTIC_UUID,
      BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_WRITE | BLECharacteristic::PROPERTY_NOTIFY);

  // Set initial value and start service
  gp_Characteristics->setValue("");

  // Set the callbacks to the characteristics
  CharacteristicCallbacks *pCallbacks = new CharacteristicCallbacks();
  gp_Characteristics->setCallbacks(pCallbacks);

  pService->start();

  // Advertising service
  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(SERVICE_UUID);
  pAdvertising->setScanResponse(true);
  pAdvertising->setMinPreferred(0x06);
  BLEDevice::startAdvertising();

  Serial.println("Characteristic defined! Now you can read it in your phone!");
#endif // #ifdef ESP32
}

void setup(void)
{
  Serial.begin(115200);

  displaySetup();
  bleSetup();

  uint16_t time = millis();
  time = millis() - time;
}

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
#endif // #ifdef TEST_DISPLAY

void displayLoop()
{
#ifdef TEST_DISPLAY
  testPrintText();
  testPrintTiles();
#endif // #ifdef TEST_DISPLAY
}

void loop()
{
  displayLoop();
}
