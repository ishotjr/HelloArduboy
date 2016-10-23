/*
 * -----------------------------------------------------------------------------
 *
 * HelloArduboy - getting to know the Arduboy API!
 *
 * -----------------------------------------------------------------------------
 *
 * 0.1 - LOOK AROUND YOU 
 * 
 * -----------------------------------------------------------------------------
 *
 */

#include <Arduboy.h>

Arduboy arduboy;

static int8_t x = 0;
static uint8_t y = 0;

void setup() {
  arduboy.begin();

  // clear screen/boot logo
  arduboy.clear();
}

void loop() {
  
  // limit framerate
  if (!arduboy.nextFrame()) {
    return;    
  }

  // intentionally adding to buffer :D
  //arduboy.clear();

  // move text diagonally across the screen (looping at limits)
  arduboy.setCursor(x++, y++);

  //arduboy.fillCircle(x - 8, y, 4, WHITE);

  arduboy.print("LOOK AROUND YOU");

  arduboy.display();

  arduboy.setRGBled((arduboy.width() - x) * (255 / arduboy.width()), 
    (arduboy.height() - y) * (255 / arduboy.height()), 0);
    //(x + y) * (255 / (arduboy.width() + arduboy.height())));

  arduboy.tunes.tone((arduboy.width() - x) + (arduboy.height() - y) * 10, 1000 / 60);
    
  if (y > arduboy.height()) {
    y = 0;
    //x = arduboy.height();
  }
  if (x > arduboy.width()) {
    x = 0 - arduboy.width();
  }

}


