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

// TODO: poss. to combine as single spritemap and just offset start?
const uint8_t PROGMEM sprite1[] = {
  // inverted
  0x00, 0x00, 0x00, 0x00, 0x1c, 0xbe, 0xee, 0xae, 0x8c, 0x48, 0x20, 0x18, 0x18, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x06, 0x06, 0xc1, 0xc0, 0x21, 0x1f, 0x0f, 0x0e, 0x10, 0x10, 0x60, 0x60, 0x00, 0x00,
//  0xff, 0xff, 0xff, 0xff, 0xe3, 0x41, 0x11, 0x51, 0x73, 0xb7, 0xdf, 0xe7, 0xe7, 0xff, 0xff, 0xff,
//  0xff, 0xff, 0xf9, 0xf9, 0x3e, 0x3f, 0xde, 0xe0, 0xf0, 0xf1, 0xef, 0xef, 0x9f, 0x9f, 0xff, 0xff,
};
const uint8_t PROGMEM sprite2[] = {
  // inverted
  0x00, 0x00, 0x00, 0x00, 0x00, 0x1c, 0xbe, 0xee, 0xae, 0x8c, 0x88, 0x40, 0x30, 0x30, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x36, 0x36, 0x41, 0x20, 0x1f, 0x0f, 0x1f, 0x20, 0xc0, 0xc0, 0x00, 0x00, 0x00,
//  0xff, 0xff, 0xff, 0xff, 0xff, 0xe3, 0x41, 0x11, 0x51, 0x73, 0x77, 0xbf, 0xcf, 0xcf, 0xff, 0xff,
//  0xff, 0xff, 0xff, 0xc9, 0xc9, 0xbe, 0xdf, 0xe0, 0xf0, 0xe0, 0xdf, 0x3f, 0x3f, 0xff, 0xff, 0xff,
};

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
    arduboy.drawBitmap(x_position, y_position, sprite2, 16, 16, WHITE);
  } else {
    arduboy.drawBitmap(x_position, y_position, sprite1, 16, 16, WHITE);    
  }

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


