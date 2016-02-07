//
// Starfield 2D
//
// for the LEDmePlay
//
// by Daniel Hirscher 2016
//

#include <Adafruit_GFX.h>   // Core graphics library
#include <RGBmatrixPanel.h> // Hardware-specific library

// Zeit zwischen Frames
#define interval 30

// Anzahl Sterne
#define nh 15 // hinten, langsam, bei jedem dritten step
#define nm 10 // mitte, mittel, bei jedem zweiten step
#define nv  8 // vorne, schnell, bei jedem step

#define w 32
#define h 32

#define CLK 50
#define LAT 10
#define OE  51
#define A   A0
#define B   A1
#define C   A2
#define D   A3

RGBmatrixPanel matrix(A, B, C, D, CLK, LAT, OE, true);

#define F2(progmem_ptr) (const __FlashStringHelper *)progmem_ptr
const char str[] PROGMEM = "Das ist der Sternenflug.";
int textX   = w;
int textMin = sizeof(str) * -12;

uint16_t ch = matrix.Color333(1, 1, 1);
uint16_t cm = matrix.Color333(4, 4, 4);
uint16_t cv = matrix.Color333(7, 7, 7);

// Sterne, pro Stern x,y
int sh[nh][2];
int sm[nm][2];
int sv[nv][2];

byte step = 0;

unsigned long waittime = 0;
unsigned long t;

void setup() {
  // nur einen Analog-Input auszulesen liefert manchmal den gleichen Wert,
  // deshalb werden mehrere miteinander verrechnet
  randomSeed(analogRead(7) * analogRead(8) + analogRead(9));

  matrix.begin();
  matrix.setTextWrap(false);
  matrix.setTextSize(2);
  matrix.setTextColor(matrix.Color333(0, 0, 1));

  for (int i = 0; i < nh; i++) {
    sh[i][0] = random(w);
    sh[i][1] = random(h);
  }
  for (int i = 0; i < nm; i++) {
    sm[i][0] = random(w);
    sm[i][1] = random(h);
  }
  for (int i = 0; i < nv; i++) {
    sv[i][0] = random(w);
    sv[i][1] = random(h);
  }
}

void loop() {
  t = millis();
  if (t > waittime)
  {
    waittime = t + interval;

    matrix.fillScreen(0);

    ++step;
    if (step >= 6) step = 0;

    for (int i = 0; i < nh; i++) {
      if (step % 3 == 0) {
        ++sh[i][0];
        if (sh[i][0] > w) { sh[i][0] = 0; sh[i][1] = random(h); }
      }
      matrix.drawPixel(sh[i][0], sh[i][1], ch);
    }
    for (int i = 0; i < nm; i++) {
      if (step % 2 == 0) {
        ++sm[i][0];
        if (sm[i][0] > w) { sm[i][0] = 0; sm[i][1] = random(h); }
      }
      matrix.drawPixel(sm[i][0], sm[i][1], cm);
    }

    matrix.setCursor(textX, 9);
    matrix.print(F2(str));

    if ((--textX) < textMin) textX = matrix.width();
    
    for (int i = 0; i < nv; i++) {
      ++sv[i][0];
      if (sv[i][0] > w + random(5)) { sv[i][0] = 0; sv[i][1] = random(h); }
      matrix.drawPixel(sv[i][0], sv[i][1], cv);
    }

    matrix.swapBuffers(false);
  }
}
