class Screen {
  uint16 *output;

  struct Regs {
    bool addsub_mode;
    bool direct_color;

    bool color_mode;
    bool color_halve;
    bool bg1_color_enable;
    bool bg2_color_enable;
    bool bg3_color_enable;
    bool bg4_color_enable;
    bool oam_color_enable;
    bool back_color_enable;

    uint8 color_b;
    uint8 color_g;
    uint8 color_r;
  } regs;

  void scanline();
  void run();
  void reset();

  uint16 light_table[16][32768];
  uint16 get_pixel(bool swap);
  uint16 addsub(uint64_t x, uint64_t y, bool halve);
  uint16 get_color(uint64_t palette);
  uint16 get_direct_color(uint64_t palette, uint64_t tile);

  void serialize(serializer&);
  Screen(PPU &self);

  PPU &self;
  friend class PPU;
};
