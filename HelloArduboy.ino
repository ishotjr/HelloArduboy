/*
 * -----------------------------------------------------------------------------
 *
 * HelloArduboy - getting to know the Arduboy API!
 *
 * -----------------------------------------------------------------------------
 *
 * 0.2 - G&W Walkabout
 * 
 * -----------------------------------------------------------------------------
 *
 */

#include <Arduboy.h>
#include "sprites.h"

#define DEBUG true

#define MAX_VELOCITY 2
#define MAX_FREEFALL 4

Arduboy arduboy;

static int8_t x_position = 0;
static int8_t y_position = 0;
static int8_t x_velocity = 0;
static int8_t y_velocity = 0;
static boolean sound_enabled = 1;

static int8_t no_movement = 0;

void setup() {
  arduboy.begin();

  // clear screen/boot logo
  arduboy.clear();

  drawLevel();

  // slow it waaaay down to G&W speed!
  arduboy.setFrameRate(1);
}

void loop() {
  
  // limit framerate
  if (!arduboy.nextFrame()) {
    return;    
  }

  // clear buffer
  arduboy.clear();

  drawLevel();

  // input
  // TODO: some kind of de-bounce or smoothing?!
  no_movement++;
  if (arduboy.pressed(UP_BUTTON)) {
    y_position--;
    y_velocity--;
    if (y_velocity < -1 * MAX_VELOCITY) {
      y_velocity = -1 * MAX_VELOCITY;
    }
    //no_movement = 0;
  }
  if (arduboy.pressed(DOWN_BUTTON)) {
    y_position++;
    y_velocity++;
    if (y_velocity > MAX_VELOCITY) {
      y_velocity = MAX_VELOCITY;
    }
    //no_movement = 0;
  }
  if (arduboy.pressed(RIGHT_BUTTON)) {
    x_position++;
    x_velocity++;
    if (x_velocity > MAX_VELOCITY) {
      x_velocity = MAX_VELOCITY;
    }
    no_movement = 0;
  }
  if (arduboy.pressed(LEFT_BUTTON)) {
    x_position--;
    x_velocity--;
    if (x_velocity < -1 * MAX_VELOCITY) {
      x_velocity = -1 * MAX_VELOCITY;
    }
    no_movement = 0;
  }
  if (arduboy.pressed(A_BUTTON)) {
    // reset
    x_position = 0;
    y_position = 0;
    x_velocity = 0;
    y_velocity = 0;
    sound_enabled = 1;
    arduboy.clear();
  }
  if (arduboy.pressed(B_BUTTON)) {
    // jump

    // prevent flying/freefall jump
    // TODO: add check for ground below!
    if (y_velocity == 0) {
      y_velocity = -5;
    }
  }


  // bounds check
  if (y_position > arduboy.height()) {
    // start offscreen vs. appearing entirely on-screen
    y_position = -1 * SPRITE_SIZE;
    //x_position = arduboy.height();
  } else if (y_position < (-1 * SPRITE_SIZE)) {
    // loop back the other way too!
    y_position = arduboy.height();
  }
  // note comparison to (x_velocity + 1) to prevent/correct overflow (range of int8_t is only 127)
  // TODO: there's still an issue in certain cases
  if (x_position > (arduboy.width() - (x_velocity + 1))) {
    x_position = -1 * SPRITE_SIZE;
  } else if (x_position < (-1 * SPRITE_SIZE)) {
    // loop back the other way too!
    x_position = arduboy.width() - 1; // prevent overflow
  }



  x_position = x_position + x_velocity;
  y_position = y_position + y_velocity;
  arduboy.setCursor(x_position, y_position);



  // ease to a stop horizontally if no input for 3 frames
  if (no_movement > 2) {
    // TODO: refactor?
    if (x_velocity > 0) {
      x_velocity--;
    } else if (x_velocity < 0) {
      x_velocity++;
    }
  }

  // gravity
  // *prevent fall-through due to velocity

  // TODO: refactor!!!
  // check for platform - feet are only middle 10/16 pixels
  if ((arduboy.getPixel(x_position + 3, y_position + SPRITE_SIZE + y_velocity + 1) == WHITE) ||
      (arduboy.getPixel(x_position + 4, y_position + SPRITE_SIZE + y_velocity + 1) == WHITE) ||
      (arduboy.getPixel(x_position + 5, y_position + SPRITE_SIZE + y_velocity + 1) == WHITE) ||
      (arduboy.getPixel(x_position + 6, y_position + SPRITE_SIZE + y_velocity + 1) == WHITE) ||
      (arduboy.getPixel(x_position + 7, y_position + SPRITE_SIZE + y_velocity + 1) == WHITE) ||
      (arduboy.getPixel(x_position + 8, y_position + SPRITE_SIZE + y_velocity + 1) == WHITE) ||
      (arduboy.getPixel(x_position + 9, y_position + SPRITE_SIZE + y_velocity + 1) == WHITE) ||
      (arduboy.getPixel(x_position + 10, y_position + SPRITE_SIZE + y_velocity + 1) == WHITE) ||
      (arduboy.getPixel(x_position + 11, y_position + SPRITE_SIZE + y_velocity + 1) == WHITE) ||
      (arduboy.getPixel(x_position + 12, y_position + SPRITE_SIZE + y_velocity + 1) == WHITE)) {
    y_velocity = 0;
  } else if (y_position < (arduboy.height() - SPRITE_SIZE - y_velocity)) {
    y_velocity++;
    if (y_velocity > MAX_FREEFALL) {
      y_velocity = MAX_FREEFALL;
    }
  } else {
    y_velocity = 0;
  }


  // only animate if in motion
  if (arduboy.everyXFrames(2) && ((abs(x_velocity) + abs(y_velocity)) > 0)) {
    arduboy.drawBitmap(x_position, y_position, sprites[1], SPRITE_SIZE, SPRITE_SIZE, WHITE);

    // make sound only while moving
    if (sound_enabled) {
      arduboy.tunes.tone((arduboy.width() - x_position) + (arduboy.height() - y_position) * 10, 1000 / 60);
    }

  } else {
    arduboy.drawBitmap(x_position, y_position, sprites[0], SPRITE_SIZE, SPRITE_SIZE, WHITE);    
  }


#if DEBUG
  // font is 6x8
  arduboy.setCursor(arduboy.width() - 3*8, 0); // font is 8px high
  //arduboy.print(x_position, DEC);
  arduboy.print(y_velocity, DEC);
  
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

}

void drawLevel() {
  arduboy.fillRect(20, arduboy.height() - 8, 12, MAX_FREEFALL, WHITE);
  arduboy.fillRect(36, arduboy.height() - 16, 12, MAX_FREEFALL, WHITE);
  arduboy.fillRect(52, arduboy.height() - 24, 12, MAX_FREEFALL, WHITE);
  arduboy.fillRect(68, arduboy.height() - 32, 12, MAX_FREEFALL, WHITE);
}
