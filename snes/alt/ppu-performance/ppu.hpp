#ifndef __SNES_PPU_H
#define __SNES_PPU_H

class PPU : public Processor, public PPUcounter, public MMIO {
public:
  enum{ Threaded = true };
  alwaysinline void step(unsigned clocks);
  alwaysinline void synchronize_cpu();

  void latch_counters();
  bool interlace() const;
  bool overscan() const;
  bool hires() const;

  void enter();
  void power();
  void reset();
  void scanline();
  void frame();

  void layer_enable(unsigned layer, unsigned priority, bool enable);
  void set_frameskip(unsigned frameskip);

  void serialize(serializer&);
  PPU();
  ~PPU();

private:
  uint16 *surface;
  uint16 *output;


  #include "mmio/mmio.hpp"
  #include "window/window.hpp"
  #include "cache/cache.hpp"
  #include "background/background.hpp"
  #include "sprite/sprite.hpp"
  #include "screen/screen.hpp"

  friend class PPU::Cache;
  friend class PPU::Background;
  friend class PPU::Sprite;
  friend class PPU::Screen;
  friend class Video;

  Cache cache;
  Background bg1;
  Background bg2;
  Background bg3;
  Background bg4;
  Sprite oam;
  Screen screen;

  struct Display {
    bool interlace;
    bool overscan;
    unsigned width;
    unsigned height;
    unsigned frameskip;
    unsigned framecounter;
  } display;

  static void Enter();
  void add_clocks(unsigned clocks);
  void render_scanline();

};

#if defined(DEBUGGER)
  #include "debugger/debugger.hpp"
  extern PPUDebugger ppu;
#else
  extern PPU *ppu;
#endif

#endif
