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
}

void loop() {
  
  // limit framerate
  if (!arduboy.nextFrame()) {
    return;    
  }

  // intentionally adding to buffer :D
  //arduboy.clear();

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

  arduboy.print("LOOK AROUND YOU");

  arduboy.display();

  // express velocities via as a percentage of red/green intensity from 0 for -MAX_VELOCITY) to 255 for MAX_VELOCITY
  // blue indicates change in position
  // TODO: LED colors not quite as expected - reduce/offset intensity?
  arduboy.setRGBled(((MAX_VELOCITY + x_velocity) * 255) / (MAX_VELOCITY * 2), 
    ((MAX_VELOCITY + y_velocity) * 255) / (MAX_VELOCITY * 2), 
    ((x_position + y_position) * 255) / (arduboy.width() + arduboy.height()));

  if (sound_enabled) {
    arduboy.tunes.tone((arduboy.width() - x_position) + (arduboy.height() - y_position) * 10, 1000 / 60);
  }
    
  if (y_position > arduboy.height()) {
    y_position = 0;
    //x_position = arduboy.height();
  } else if (y_position < (-1 * arduboy.height())) {
    // loop back the other way too!
    // TODO: w/b better if comparison was to text height
    y_position = arduboy.height();
  }
  if (x_position > arduboy.width()) {
    x_position = 0 - arduboy.width();
  } else if (x_position < (-1 * arduboy.width())) {
    // loop back the other way too!
    // TODO: w/b better if comparison was to text width
    x_position = arduboy.width();
  }

}


