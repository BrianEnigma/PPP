#include <Adafruit_NeoPixel.h>

#ifdef __AVR__
 #include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#endif

#define LED_PIN    13
#define LED_COUNT 40
Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);

void setup() {
    #if defined(__AVR_ATtiny85__) && (F_CPU == 16000000)
        clock_prescale_set(clock_div_1);
    #endif
    strip.begin();
    strip.show();
    strip.setBrightness(100);
    Serial.begin(38400);
}

void lightAll(unsigned char red, unsigned char green, unsigned char blue, unsigned int wait)
{
    const unsigned char skip = 1;
    uint32_t color = strip.Color(red, green, blue);
    for (int i = 0; i < strip.numPixels(); i += skip)
    {
        strip.setPixelColor(i, color);
    }
    strip.show();
    delay(wait);
}

void off()
{
    for (int i = 0; i < strip.numPixels(); i++)
    {
        strip.setPixelColor(i, 0x000000);
    }
    strip.show();
}

void redPulse()
{
    const unsigned char low = 96;
    const unsigned char high = 255;
    const unsigned int delayStart = 4;
    const unsigned int delayPulse = 8;
    // Fade up
    for (int i = 0; i < low; i++)
        lightAll(i, 0, 0, delayStart);
    // Pulse
    for (int repeat = 0; repeat < 6; repeat++)
    {
        for (int i = low; i < high; i++)
            lightAll(i, 0, 0, delayPulse);
        for (int i = high; i > low; i--)
            lightAll(i, 0, 0, delayPulse);
    }
    // Fade out
    for (int i = low; i >= 0; i--)
        lightAll(i, 0, 0, delayStart);
    off();
}

void greenPulse()
{
    const unsigned char low = 96;
    const unsigned char high = 255;
    const unsigned int delayStart = 4;
    const unsigned int delayPulse = 8;
    // Fade up
    for (int i = 0; i < low; i++)
        lightAll(0, i, 0, delayStart);
    // Pulse
    for (int repeat = 0; repeat < 6; repeat++)
    {
        for (int i = low; i < high; i++)
            lightAll(0, i, 0, delayPulse);
        for (int i = high; i > low; i--)
            lightAll(0, i, 0, delayPulse);
    }
    // Fade out
    for (int i = low; i >= 0; i--)
        lightAll(0, i, 0, delayStart);
    off();
}

void redWithLightning()
{
    const unsigned char low = 128;
    const unsigned int delayStart = 10;
    const unsigned char lightningHigh = 192;
    const unsigned int delayLightning = 2;
    // Fade up
    for (int i = 0; i < low; i++)
        lightAll(i, 0, 0, delayStart);
    delay(1000);
    // Pulse
    for (int repeat = 0; repeat < 3; repeat++)
    {
        for (int i = lightningHigh; i > 0; i--)
            lightAll(i > low ? i : low, i, i, delayLightning);
        delay(1000);
    }
    // Fade out
    for (int i = low; i >= 0; i--)
        lightAll(i, 0, 0, delayStart);
    off();

}

void redWithRedLightning()
{
    const unsigned char low = 128;
    const unsigned int delayStart = 10;
    const unsigned char lightningHigh = 255;
    const unsigned int delayLightning = 2;
    // Fade up
    for (int i = 0; i < low; i++)
        lightAll(i, 0, 0, delayStart);
    delay(1000);
    // Pulse
    for (int repeat = 0; repeat < 3; repeat++)
    {
        for (int i = lightningHigh; i > low; i--)
            lightAll(i, 0, 0, delayLightning);
        delay(1000);
    }
    // Fade out
    for (int i = low; i >= 0; i--)
        lightAll(i, 0, 0, delayStart);
    off();

}

void lightning()
{
    const unsigned char bright = 255;
    const unsigned int wait = 2;
    for (int repeat = 0; repeat < 3; repeat++)
    {
        for (int i = bright; i > 0; i--)
            lightAll(i, i, i, wait);
        off();
        delay(1000);
    }
}

void rgbTest()
{
    const uint32_t colors[] = {
        strip.Color(255, 0, 0),
        strip.Color(0, 255, 0),
        strip.Color(0, 0, 255),
        strip.Color(255, 255, 255),
        strip.Color(0, 0, 0)
    };
    const unsigned int colorCount = sizeof(colors) / sizeof(colors[0]);
    const int wipeDelay = 20;
    const int interWipeDelay = 750;
    for (int color = 0; color < colorCount; color++)
    {
        for (int i = 0; i < strip.numPixels(); i += 1)
        {
            strip.setPixelColor(i, colors[color]);
            strip.show();
            delay(wipeDelay);
        }
        delay(interWipeDelay);
    }
}

void threeRedHits()
{
    int positions[] = {30, 20, 10};
    const unsigned int positionCount = sizeof(positions) / sizeof(positions[0]);
    const unsigned int wipeDelay = 10;
    const unsigned int interWipeDelay = 1000;
    const int fadeMultiplier = 20;
    for (int position = 0; position < positionCount; position++)
    {
        for (int color = 0; color < 2; color++)
        {   
            uint32_t theColor = strip.Color(0, 0, 0);
            for (int i = 0; i < strip.numPixels(); i++)
            {
                if (color == 0)
                    if (255 - fadeMultiplier * i > 0)
                        theColor = strip.Color(255 - fadeMultiplier * i, 0, 0);
                int left = positions[position] - i;
                int right = positions[position] + i;
                if (left >= 0)
                    strip.setPixelColor(left, theColor);
                if (right < strip.numPixels())
                    strip.setPixelColor(right, theColor);
                strip.show();
                delay(wipeDelay);
            }
        }
        delay(interWipeDelay);
    }
}

unsigned int previousMenuSelection = 0;
unsigned int doMenu()
{
    unsigned int result = 0;
    do
    {
        Serial.println("\n\n\n\n\n\n\n\n\n\n");
        Serial.println("MENU");
        Serial.println("~~~~");
        Serial.println("1.  Red Pulse");
        Serial.println("2.  Green Pulse");
        Serial.println("3.  Red With Lightning");
        Serial.println("4.  Lightning");
        Serial.println("5.  Red With Red Lightning");
        Serial.println("6.  Three Red Hits");
        Serial.println("99. RGB Test");
        do {
        } while (!Serial.available());
        String entry = Serial.readStringUntil('\n');
        if (entry.length() == 0)
        {
            result = previousMenuSelection;
        } else {
            result = entry.toInt();
        }
    } while ((result == 0 || result > 6) && result != 99);
    previousMenuSelection = result;
    return result;
}

void loop() {
    unsigned int value = doMenu();
    Serial.print("GOT VALUE: ");
    Serial.println(value);
    switch(value)
    {
        case 1: redPulse(); break;
        case 2: greenPulse(); break;
        case 3: redWithLightning(); break;
        case 4: lightning(); break;
        case 5: redWithRedLightning(); break;
        case 6: threeRedHits(); break;
        case 99: rgbTest(); break;
    }

}
