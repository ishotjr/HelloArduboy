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
#include "sprites.h"

#define DEBUG true

#define MAX_VELOCITY 5

Arduboy arduboy;

static int8_t x_position = 0;
static int8_t y_position = 0;
static int8_t x_velocity = 1;
static int8_t y_velocity = 1;
static boolean sound_enabled = 1;

void setup() {
  arduboy.begin();

  // clear screen/boot logo
  arduboy.clear();

  // slow it waaaay down to G&W speed!
  arduboy.setFrameRate(1);
}

void loop() {
  
  // limit framerate
  if (!arduboy.nextFrame()) {
    return;    
  }

  // NO LONGER intentionally adding to buffer :D
  arduboy.clear();
  //arduboy.fillScreen(WHITE);

  // input
  // TODO: some kind of de-bounce or smoothing?!
  if (arduboy.pressed(UP_BUTTON)) {
    y_velocity++;
    if (y_velocity > MAX_VELOCITY) {
      y_velocity = MAX_VELOCITY;
    }
  } else if (arduboy.pressed(DOWN_BUTTON)) {
    y_velocity--;
    if (y_velocity < -1 * MAX_VELOCITY) {
      y_velocity = -1 * MAX_VELOCITY;
    }
  } else if (arduboy.pressed(RIGHT_BUTTON)) {
    x_velocity++;
    if (x_velocity > MAX_VELOCITY) {
      x_velocity = MAX_VELOCITY;
    }
  } else if (arduboy.pressed(LEFT_BUTTON)) {
    x_velocity--;
    if (x_velocity < -1 * MAX_VELOCITY) {
      x_velocity = -1 * MAX_VELOCITY;
    }
  } else if (arduboy.pressed(A_BUTTON)) {
    // reset
    x_position = 0;
    y_position = 0;
    x_velocity = 1;
    y_velocity = 1;
    sound_enabled = 1;
    arduboy.clear();
  } else if (arduboy.pressed(B_BUTTON)) {
    // toggle sound
    sound_enabled = !sound_enabled;
  }

  // move text diagonally across the screen (looping at limits)
  x_position = x_position + x_velocity;
  y_position = y_position + y_velocity;
  arduboy.setCursor(x_position, y_position);

  //arduboy.fillCircle(x_position - 8, y_position, 4, WHITE);

  //arduboy.print("LOOK AROUND YOU");
  if (arduboy.everyXFrames(2)) {
    arduboy.drawBitmap(x_position, y_position, sprites[1], SPRITE_SIZE, SPRITE_SIZE, WHITE);
  } else {
    arduboy.drawBitmap(x_position, y_position, sprites[0], SPRITE_SIZE, SPRITE_SIZE, WHITE);    
  }

#if DEBUG
  // font is 6x8
  arduboy.setCursor(arduboy.width() - 3*8, 0); // font is 8px high
  arduboy.print(x_position, DEC);
  arduboy.setCursor(arduboy.width() - 3*8, arduboy.height() - 8);
  arduboy.print(y_position, DEC);
#endif

  arduboy.display();

  // express velocities via as a percentage of red/green intensity from 0 for -MAX_VELOCITY) to 255 for MAX_VELOCITY
  // blue indicates change in position
  // TODO: LED colors not quite as expected - reduce/offset intensity?
  /*
  arduboy.setRGBled(((MAX_VELOCITY + x_velocity) * 255) / (MAX_VELOCITY * 2), 
    ((MAX_VELOCITY + y_velocity) * 255) / (MAX_VELOCITY * 2), 
    ((x_position + y_position) * 255) / (arduboy.width() + arduboy.height()));
  */

  if (sound_enabled) {
    arduboy.tunes.tone((arduboy.width() - x_position) + (arduboy.height() - y_position) * 10, 1000 / 60);
  }
    
  if (y_position > arduboy.height()) {
    // start offscreen vs. appearing entirely on-screen
    y_position = -1 * SPRITE_SIZE;
    //x_position = arduboy.height();
  } else if (y_position < (-1 * SPRITE_SIZE)) {
    // loop back the other way too!
    y_position = arduboy.height();
  }
  // note comparison to (x_velocity + 1) to prevent/correct overflow (range of int8_t is only 127)
  if (x_position > (arduboy.width() - (x_velocity + 1))) {
    x_position = -1 * SPRITE_SIZE;
  } else if (x_position < (-1 * SPRITE_SIZE)) {
    // loop back the other way too!
    x_position = arduboy.width() - 1; // prevent overflow
  }

}


