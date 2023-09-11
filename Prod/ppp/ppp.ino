#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>
#include <Adafruit_NeoPixel.h>

// --------------------------------------------------------------------------------
// --- User-Servicible Parts:

// Wait until the proximity sensor returns at least this value to trigger animations.
// The higher the value, the closer someone is to the sensor, but the distance-to-value
// curve is not at all linear. 
// For more details, see page 4 of the datasheet: https://cdn-shop.adafruit.com/product-files/1031/GP2Y0A02YK-DATA-SHEET.PDF
#define ANALOG_THRESHOLD 900

// How long to wait after powering-up before we start to check the proximity sensor.
// This gives a "minimum time to reach safe distance" so that you don't immediately
// trigger the animation at boot.
// This is in seconds.
#define POST_BOOT_HOLDOFF_TIMER 3

// Minimum time (in minutes) to randomly hold off between actions.
#define HOLDOFF_MIN_TIME_MINUTES 10
// Maximum time (in minutes) to randomly hold off between actions.
#define HOLDOFF_MAX_TIME_MINUTES 15

// One-in-this-many (1:10) chance of hitting the high note instead of the sequence of low notes.
#define HIGH_HIT_RANDOM 8

// --------------------------------------------------------------------------------
// Beyond this, thar be dragons. Try not to mess with any of this unless you really
// know what you're doing.

//#define INCLUDE_SERIAL

#define SOLENOID_PIN_1 3
#define SOLENOID_PIN_2 12
#define SOLENOID_PIN_3 13
#define SOLENOID_PIN_4 15

#define NEOPIXEL_PIN 14
#define NUMPIXELS 40
Adafruit_NeoPixel pixels(NUMPIXELS, NEOPIXEL_PIN, NEO_GRB + NEO_KHZ800);

Adafruit_SH1107 display = Adafruit_SH1107(64, 128, &Wire);
#define BUTTON_A  0
#define BUTTON_B 16
#define BUTTON_C  2

// 1-bit pixel art representation of the tombstone logo.
const uint8_t LOGO[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x01, 0xFF, 0xFF, 0x80, 0x00, 0x00, 
0x00, 0x00, 0x7E, 0x00, 0x00, 0x7E, 0x00, 0x00, 
0x00, 0x07, 0x80, 0x00, 0x00, 0x01, 0xE0, 0x00, 
0x00, 0x38, 0x00, 0x00, 0x00, 0x00, 0x1C, 0x00, 
0x01, 0xC0, 0x00, 0x1F, 0xE0, 0x00, 0x03, 0x80, 
0x02, 0x00, 0x00, 0x0F, 0xF0, 0x00, 0x00, 0x40, 
0x0C, 0x43, 0xFC, 0x0C, 0x38, 0xFF, 0x00, 0x30, 
0x15, 0x81, 0xFE, 0x0C, 0x1C, 0x7F, 0x80, 0x38, 
0x16, 0x01, 0x87, 0x0C, 0x1C, 0x61, 0xC0, 0x28, 
0x22, 0x01, 0x83, 0x8C, 0x1C, 0x60, 0xE0, 0x64, 
0x21, 0x01, 0x83, 0x8C, 0x1C, 0x60, 0xE0, 0x94, 
0x22, 0x81, 0x83, 0x8C, 0x1C, 0x60, 0xE0, 0x14, 
0x22, 0x41, 0x83, 0x8C, 0x38, 0x60, 0xE0, 0x24, 
0x24, 0x01, 0x83, 0x8C, 0x30, 0x60, 0xE0, 0x04, 
0x20, 0x01, 0x87, 0x0C, 0x70, 0x61, 0xC0, 0x04, 
0x20, 0x01, 0x86, 0x1C, 0xC0, 0x61, 0x80, 0x04, 
0x20, 0x01, 0x8E, 0x0D, 0x80, 0x63, 0x80, 0x04, 
0x20, 0x03, 0x98, 0x0F, 0x00, 0xE6, 0x00, 0x04, 
0x10, 0x01, 0xB0, 0x0E, 0x00, 0x6C, 0x00, 0x04, 
0x08, 0x01, 0xE0, 0x0E, 0x00, 0x78, 0x00, 0x04, 
0x04, 0x01, 0xC0, 0x0F, 0x00, 0x70, 0x00, 0x08, 
0x38, 0x01, 0xC0, 0x0C, 0x00, 0x70, 0x00, 0x10, 
0x20, 0x01, 0xE0, 0x0C, 0x00, 0x78, 0x00, 0x08, 
0x20, 0x01, 0x80, 0x0C, 0x00, 0x60, 0x00, 0x04, 
0x20, 0x01, 0x80, 0x0C, 0x00, 0x60, 0x00, 0x04, 
0x20, 0x01, 0x80, 0x0C, 0x00, 0x60, 0x00, 0x04, 
0x20, 0x03, 0xC0, 0x1E, 0x00, 0xF0, 0x00, 0x04, 
0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 
0x26, 0x23, 0x36, 0x66, 0xD8, 0x64, 0x92, 0x44, 
0x25, 0x54, 0x44, 0x88, 0x94, 0x55, 0x5A, 0xA4, 
0x26, 0x52, 0x26, 0x44, 0xD4, 0x65, 0xD6, 0xA4, 
0x24, 0x51, 0x14, 0x22, 0x94, 0x45, 0x52, 0xA4, 
0x24, 0x23, 0x36, 0x66, 0xD8, 0x45, 0x52, 0x44, 
0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1C, 
0x21, 0x00, 0x19, 0x88, 0x59, 0x38, 0x00, 0x24, 
0x36, 0x00, 0x15, 0x54, 0x52, 0x90, 0x00, 0x44, 
0x28, 0x00, 0x19, 0x94, 0x5A, 0x10, 0x00, 0x44, 
0x20, 0x00, 0x11, 0x55, 0x52, 0x90, 0x00, 0x04, 
0x20, 0x00, 0x11, 0x48, 0x99, 0x10, 0x00, 0x04, 
0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 
0x3F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFC, 
0x09, 0xDC, 0x9D, 0xDC, 0x9D, 0xC9, 0xDD, 0xD0, 
0x09, 0xDC, 0x9D, 0xDC, 0x9D, 0xC9, 0xDD, 0xD0, 
0x09, 0xDC, 0x9D, 0xDC, 0x9D, 0xC9, 0xDD, 0xD0, 
0x09, 0xDC, 0x9D, 0xDC, 0x9D, 0xC9, 0xDD, 0xD0, 
0x09, 0xDC, 0x9D, 0xDC, 0x9D, 0xC9, 0xDD, 0xD0, 
0x09, 0xDC, 0x9D, 0xDC, 0x9D, 0xC9, 0xDD, 0xD0, 
0x08, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x90, 
0x08, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x90, 
0x0F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xF0};
const unsigned int LOGO_SIZE = sizeof(LOGO) / sizeof(LOGO[0]);

char g_forcedTrigger = 0;

// Light up all of the LEDs the same color.
void lightAll(unsigned char red, unsigned char green, unsigned char blue, unsigned int wait)
{
    uint32_t color = pixels.Color(red, green, blue);
    for (int i = 0; i < NUMPIXELS; i++)
    {
        pixels.setPixelColor(i, color);
    }
    pixels.show();
    delay(wait);
}

// Turn off the LED strip.
void off()
{
    for (int i = 0; i < NUMPIXELS; i++)
    {
        pixels.setPixelColor(i, 0x000000);
    }
    pixels.show();
}

// Load the logo onto the screen and display it.
// Note that this doesn't first clear the screen.
void loadLogo()
{
    unsigned int x = 0;
    unsigned int y = 0;
    for (unsigned int byte = 0; byte < LOGO_SIZE; byte++)
    {
        for (unsigned int bit = 0; bit < 8; bit++)
        {
            display.drawPixel(y, 63 - x, (LOGO[byte] >> (7 - bit)) & 0x01);
            x++;
        }
        if (x >= 64)
        {
            x = 0;
            y++;
        }
    }
    display.display();
    display.setTextSize(1);
    display.setTextColor(SH110X_WHITE);
    display.setCursor(54,0);
}

void holdoffTimer(const char *message, int seconds)
{
    bool earlyAbort = false;
    display.clearDisplay();
    display.setRotation(1);
    loadLogo();
    display.setCursor(54,0);
    display.print(message);
    display.setCursor(54, 10);
    display.print("A to skip");
    while (seconds > 0 && !earlyAbort)
    {
        int displayMinutes = seconds / 60;
        int displaySeconds = seconds % 60;
        display.fillRect(54, 20, 128 - 54, 64 - 20, 0x00);
        display.setCursor(54, 20);
        display.setTextSize(2);
        display.print(displayMinutes);
        display.print(":");
        if (displaySeconds < 10)
            display.print("0");
        display.print(displaySeconds);
        seconds--;
        display.display();
        for (int repeat = 0; repeat < 10 && !earlyAbort; repeat++)
        {
            if (!digitalRead(BUTTON_A))
            {
                earlyAbort = true;
            }
            delay(100);
        }
        if (seconds % 20 == 0) // Double-check the LEDs are off every so often.
            off();
    }
    display.clearDisplay();
    display.setRotation(1);
    loadLogo();
    display.display();
    delay(500);
}

void setup() 
{
    // Set up all the pins early to help prevent glitches on the line due to floating pins
    // while the other components are initializing.
    pinMode(SOLENOID_PIN_1, OUTPUT);
    pinMode(SOLENOID_PIN_2, OUTPUT);
    pinMode(SOLENOID_PIN_3, OUTPUT);
    pinMode(SOLENOID_PIN_4, OUTPUT);
    digitalWrite(SOLENOID_PIN_1, LOW);
    digitalWrite(SOLENOID_PIN_2, LOW);
    digitalWrite(SOLENOID_PIN_3, LOW);
    digitalWrite(SOLENOID_PIN_4, LOW);

#ifdef INCLIDE_SERIAL
    Serial.begin(115200);
#endif

#if defined(__AVR_ATtiny85__) && (F_CPU == 16000000)
    clock_prescale_set(clock_div_1);
#endif
    pixels.begin(); // INITIALIZE NeoPixel strip object (REQUIRED)
    pixels.setBrightness(100);
    off();
    pixels.show();

#ifdef INCLIDE_SERIAL
    Serial.println("Piano Controller Booting Up...");
#endif
    delay(250); // wait for the OLED to power up
    display.begin(0x3C, true); // Address 0x3C default

#ifdef INCLIDE_SERIAL
    Serial.println("OLED started");
#endif

    display.clearDisplay();
    display.setRotation(1);
    loadLogo();
    display.display();
    delay(1000);

    // text display tests
    display.setTextSize(1);
    display.setTextColor(SH110X_WHITE);
    display.setCursor(54,0);
    display.print("Test Mode");
    display.setCursor(54,8);
    display.print("A=LED Strip");
    display.setCursor(54,16);
    display.print("B=Sol 1,2,3");
    display.setCursor(54,24);
    display.print("C=Sol 4");

    display.display(); // actually display all of the above

    // This is the only analog pin, and it's not left floating, so is reading actively from the distance
    // sensor. Here, we just hope the distance sensor is providing values that are "random enough" to
    // give us a decent seed.
    randomSeed(analogRead(A0));

    // Give some time to "reach minimum safe distance" before prox sensing kicks in.
    holdoffTimer("StartupDelay", POST_BOOT_HOLDOFF_TIMER);
}

void waitForProx()
{
    off();
    display.clearDisplay();
    display.setRotation(1);
    loadLogo();
    display.setCursor(54,0);
    display.print("Waiting for");
    display.setCursor(54,10);
    display.print("Prox...");
    display.setCursor(54, 55);
    display.print("C/B = force");
    display.display();
    while (true)
    {
        //int value = map(analogRead(A0), 0, 1023, 0, 255);
        int value = analogRead(A0);
        display.fillRect(54, 20, 128 - 54, 16, 0x00);
        display.setTextSize(2);
        display.setCursor(54,20);
        display.print("v:");
        display.print(value);
        display.display();
        if (value >= ANALOG_THRESHOLD)
            break;
        if (!digitalRead(BUTTON_C))
        {
            g_forcedTrigger = 1;
            break;
        } else if (!digitalRead(BUTTON_B))
        {
            g_forcedTrigger = 2;
            break;
        }
        delay(250);        
    }
    display.fillRect(54, 0, 128 - 54, 64, 0x00);
    display.setTextSize(2);
    display.setCursor(54,8);
    display.display();
    display.print("Prox Trig");
}

void bangLow()
{
    const unsigned char low = 128;
    const unsigned int delayStart = 3;
    const unsigned int delayEnd = 10;
    const unsigned char lightningHigh = 192;
    const unsigned int delayLightning = 2;
#ifdef INCLIDE_SERIAL
    Serial.println("Testing LEDs");
#endif
    // Fade up red.
    for (int i = 0; i < low; i++)
        lightAll(i, 0, 0, delayStart);
    delay(1000);
    // Pulse
    for (int repeat = 0; repeat < 3; repeat++)
    {
#if 1        
        // Activate each key in sequence.
        switch (repeat)
        {
            case 0: digitalWrite(SOLENOID_PIN_1, HIGH); break;
            case 1: digitalWrite(SOLENOID_PIN_2, HIGH); break;
            case 2: digitalWrite(SOLENOID_PIN_3, HIGH); break;
        }
#else        
        // First key stays down. Hit second, then third. Only two keys every held down at once.
        switch (repeat)
        {
            case 0: 
                digitalWrite(SOLENOID_PIN_1, HIGH); 
                break;
            case 1: 
                digitalWrite(SOLENOID_PIN_2, HIGH); 
                break;
            case 2: 
                digitalWrite(SOLENOID_PIN_2, LOW); 
                digitalWrite(SOLENOID_PIN_3, HIGH); 
                break;
        }
#endif
        // Lightning strike
        for (int i = lightningHigh; i > 0; i--)
            lightAll(i > low ? i : low, i, i, delayLightning);
#if 1            
        delay(1000);
#else
        delay(500);
        digitalWrite(SOLENOID_PIN_2, LOW);
        delay(500);
#endif        
    }
    // Let go of all keys.
    digitalWrite(SOLENOID_PIN_1, LOW);
    digitalWrite(SOLENOID_PIN_2, LOW);
    digitalWrite(SOLENOID_PIN_3, LOW);
    // Fade to black
    for (int i = low; i >= 0; i--)
        lightAll(i, 0, 0, delayEnd);
    // Ensure we're off.
    off();
#ifdef INCLIDE_SERIAL
    Serial.println("Done");
#endif
}

void bangHigh()
{
    const int lightningHigh = 192;
    const int lightningLow = 0;
    const int delayLightning = 2;
#ifdef INCLIDE_SERIAL
    Serial.println("Banging High Solenoid");
#endif
    // Activate key
    digitalWrite(SOLENOID_PIN_4, HIGH);
    // Single lightning flash.
    for (int i = lightningHigh; i > 0; i--)
        lightAll(i, i, i, delayLightning);
    delay(250);
    // Deactivate key
    digitalWrite(SOLENOID_PIN_4, LOW);
    off();
    delay(1000);
#ifdef INCLIDE_SERIAL
    Serial.println("Done");
#endif
}

void loop() 
{
    waitForProx();
    if (g_forcedTrigger == 2 || (((int) random(0, HIGH_HIT_RANDOM - 1)) == 0))
    {
        bangHigh();
    } else {
        bangLow();
    }
    g_forcedTrigger = 0;
    // Delay between 25 and 45 minutes...
    holdoffTimer("HoldoffTimer", 60 * random(HOLDOFF_MIN_TIME_MINUTES, HOLDOFF_MAX_TIME_MINUTES));
    delay(10);
    yield();
}
