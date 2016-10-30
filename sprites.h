#define SPRITE_SIZE 16

// size is SPRITE_SIZE * SPRITE_SIZE / 8 bits per uint8_t
const uint8_t PROGMEM sprites[][SPRITE_SIZE * SPRITE_SIZE / 8] = {
  {
    // inverted
    0x00, 0x00, 0x00, 0x00, 0x1c, 0xbe, 0xee, 0xae, 0x8c, 0x48, 0x20, 0x18, 0x18, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x06, 0x06, 0xc1, 0xc0, 0x21, 0x1f, 0x0f, 0x0e, 0x10, 0x10, 0x60, 0x60, 0x00, 0x00,
    // 0xff, 0xff, 0xff, 0xff, 0xe3, 0x41, 0x11, 0x51, 0x73, 0xb7, 0xdf, 0xe7, 0xe7, 0xff, 0xff, 0xff,
    // 0xff, 0xff, 0xf9, 0xf9, 0x3e, 0x3f, 0xde, 0xe0, 0xf0, 0xf1, 0xef, 0xef, 0x9f, 0x9f, 0xff, 0xff,

  },
  {
    // inverted
    0x00, 0x00, 0x00, 0x00, 0x00, 0x1c, 0xbe, 0xee, 0xae, 0x8c, 0x88, 0x40, 0x30, 0x30, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x36, 0x36, 0x41, 0x20, 0x1f, 0x0f, 0x1f, 0x20, 0xc0, 0xc0, 0x00, 0x00, 0x00,
    // 0xff, 0xff, 0xff, 0xff, 0xff, 0xe3, 0x41, 0x11, 0x51, 0x73, 0x77, 0xbf, 0xcf, 0xcf, 0xff, 0xff,
    // 0xff, 0xff, 0xff, 0xc9, 0xc9, 0xbe, 0xdf, 0xe0, 0xf0, 0xe0, 0xdf, 0x3f, 0x3f, 0xff, 0xff, 0xff,
  }
};