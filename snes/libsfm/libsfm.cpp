#include "libsfm.h"
#include <snes/snes.hpp>

#include <nall/snes/cartridge.hpp>
using namespace nall;

struct sfm_file
{
  BML::Document metadata;
  uint8 apuram[65536];
  uint8 dspram[128];
  array<uint8> portdata;
};

sfm_file * sfm_load_state(uint8_t const* state, size_t size) {
  if (size < (8 + 65536 + 128)) return 0;
  if (memcmp(state, "SFM1", 4) != 0) return 0;
  uint32 metadata_size = state[4] + state[5] * 256 + state[6] * 65536 + state[7] * 16777216;
  if (size < (8 + 65536 + 128 + metadata_size)) return 0;
  sfm_file * file = new sfm_file;
  char * document = new char[metadata_size + 1];
  memcpy(document, state + 8, metadata_size);
  document[metadata_size] = '\0';
  bool loaded = file->metadata.load(document);
  delete [] document;
  if (!loaded) {
    delete file;
    return 0;
  }
  memcpy(file->apuram, state + 8 + metadata_size, 65536);
  memcpy(file->dspram, state + 8 + metadata_size + 65536, 128);
  size_t portdata_size = size - (8 + metadata_size + 65536 + 128);
  file->portdata.resize(portdata_size);
  memcpy(&file->portdata[0], state + 8 + metadata_size + 65536 + 128, portdata_size);
  return file;
}

static void sfm_serialize_metadata(sfm_file const* file, string & metadata)
{
  metadata.append("smp\n");
  metadata.append("  test:", file->metadata["smp/test"], "\n");
  metadata.append("  iplrom:", file->metadata["smp/iplrom"], "\n");
  metadata.append("  dspaddr:", file->metadata["smp/dspaddr"], "\n");
  metadata.append("  ram:", file->metadata["smp/ram"], "\n");
auto timer = [&](unsigned timer) {
  string path, temp;
  path.append("smp/timer[", timer, "]/");
  metadata.append("  timer\n");
  temp = "";
  temp.append(path, "enable");
  metadata.append("    enable:", file->metadata[temp], "\n");
  temp = "";
  temp.append(path, "target");
  metadata.append("    target:", file->metadata[temp], "\n");
  temp = "";
  temp.append(path, "stage");
  metadata.append("    stage:", file->metadata[temp], "\n");
  temp = "";
  temp.append(path, "line");
  metadata.append("    line:", file->metadata[temp], "\n");
};
  timer(0);
  timer(1);
  timer(2);
  metadata.append("\n");

  metadata.append("dsp\n");
  metadata.append("  echohistaddr:", file->metadata["dsp/echohistaddr"], "\n");
  metadata.append("  echohistdata:", file->metadata["dsp/echohistdata"], "\n");
  metadata.append("  sample:", file->metadata["dsp/sample"], "\n");
  metadata.append("  kon:", file->metadata["dsp/kon"], "\n");
  metadata.append("  noise:", file->metadata["dsp/noise"], "\n");
  metadata.append("  counter:", file->metadata["dsp/counter"], "\n");
  metadata.append("  echooffset:", file->metadata["dsp/echooffset"], "\n");
  metadata.append("  echolength:", file->metadata["dsp/echolength"], "\n");
  metadata.append("  koncache:", file->metadata["dsp/koncache"], "\n");
  metadata.append("  endx:", file->metadata["dsp/endx"], "\n");
  metadata.append("  envx:", file->metadata["dsp/envx"], "\n");
  metadata.append("  outx:", file->metadata["dsp/outx"], "\n");
  metadata.append("  pmon:", file->metadata["dsp/pmon"], "\n");
  metadata.append("  non:", file->metadata["dsp/non"], "\n");
  metadata.append("  eon:", file->metadata["dsp/eon"], "\n");
  metadata.append("  dir:", file->metadata["dsp/dir"], "\n");
  metadata.append("  koff:", file->metadata["dsp/koff"], "\n");
  metadata.append("  brrnext:", file->metadata["dsp/brrnext"], "\n");
  metadata.append("  adsr0:", file->metadata["dsp/adsr0"], "\n");
  metadata.append("  brrheader:", file->metadata["dsp/brrheader"], "\n");
  metadata.append("  brrdata:", file->metadata["dsp/brrdata"], "\n");
  metadata.append("  srcn:", file->metadata["dsp/srcn"], "\n");
  metadata.append("  esa:", file->metadata["dsp/esa"], "\n");
  metadata.append("  echodisable:", file->metadata["dsp/echodisable"], "\n");
  metadata.append("  diraddr:", file->metadata["dsp/diraddr"], "\n");
  metadata.append("  pitch:", file->metadata["dsp/pitch"], "\n");
  metadata.append("  output:", file->metadata["dsp/output"], "\n");
  metadata.append("  looped:", file->metadata["dsp/looped"], "\n");
  metadata.append("  echoaddr:", file->metadata["dsp/echoaddr"], "\n");
  metadata.append("  mainout:", file->metadata["dsp/mainout"], "\n");
  metadata.append("  echoout:", file->metadata["dsp/echoout"], "\n");
  metadata.append("  echoin:", file->metadata["dsp/echoin"], "\n");
auto voice = [&](unsigned voice) {
  string path, temp;
  path.append("dsp/voice[", voice, "]/");
  metadata.append("  voice\n");
  temp = "";
  temp.append(path, "brrhistaddr");
  metadata.append("    brrhistaddr:", file->metadata[temp], "\n");
  temp = "";
  temp.append(path, "brrhistdata");
  metadata.append("    brrhistdata:", file->metadata[temp], "\n");
  temp = "";
  temp.append(path, "interpaddr");
  metadata.append("    interpaddr:", file->metadata[temp], "\n");
  temp = "";
  temp.append(path, "brraddr");
  metadata.append("    brraddr:", file->metadata[temp], "\n");
  temp = "";
  temp.append(path, "brroffset");
  metadata.append("    brroffset:", file->metadata[temp], "\n");
  temp = "";
  temp.append(path, "vbit");
  metadata.append("    vbit:", file->metadata[temp], "\n");
  temp = "";
  temp.append(path, vidx);
  metadata.append("    vidx:", file->metadata[temp], "\n");
  temp = "";
  temp.append(path, "kondelay");
  metadata.append("    kondelay:", file->metadata[temp], "\n");
  temp = "";
  temp.append(path, "envmode");
  metadata.append("    envmode:", file->metadata[temp], "\n");
  temp = "";
  temp.append(path, "env");
  metadata.append("    env:", file->metadata[temp], "\n");
  temp = "";
  temp.append(path, "envxout");
  metadata.append("    envxout:", file->metadata[temp], "\n");
  temp = "";
  temp.append(path, "envcache");
  metadata.append("    envcache:", file->metadata[temp], "\n");
};
  voice(0);
  voice(1);
  voice(2);
  voice(3);
  voice(4);
  voice(5);
  voice(6);
  voice(7);
  metadata.append("\n");
}

size_t sfm_get_state_size(sfm_file const* file)
{
  string metadata;
  sfm_serialize_metadata(file, metadata);
  return 8 + metadata.length() + 65536 + 128 + file->portdata.size();
}

void sfm_save_state(sfm_file const* file, uint8_t * out, size_t out_size)
{
  if (out_size >= 4) {
    out[0] = 'S'; out[1] = 'F'; out[2] = 'M'; out[3] = '1';
    if (out_size >= 8) {
      string metadata;
      sfm_serialize_metadata(file, metadata);
      size_t metadata_size = metadata.length();
      out[4] = metadata_size & 255;
      out[5] = (metadata_size >> 8) & 255;
      out[6] = (metadata_size >> 16) & 255;
      out[7] = (metadata_size >> 24) & 255;
      if (out_size >= 8 + metadata_size) {
        memcpy(out + 8, &metadata[0], metadata_size);
        if (out_size >= 8 + metadata_size + 65536) {
          memcpy(out + 8 + metadata_size, file->apuram, 65536);
          if (out_size >= 8 + metadata_size + 65536 + 128) {
            memcpy(out + 8 + metadata_size + 65536, file->dspram, 128);
            if (out_size >= 8 + metadata_size + 65536 + 128 + file->portdata.size()) {
              memcpy(out + 8 + metadata_size + 65536 + 128, &file->portdata[0], file->portdata.size());
            }
          }
        }
      }
    }
  }
}

void retro_init(void) {
  SNES::interface = &interface;
  SNES::system.init();
  SNES::input.connect(SNES::Controller::Port1, SNES::Input::Device::Joypad);
  SNES::input.connect(SNES::Controller::Port2, SNES::Input::Device::Joypad);
}

void retro_deinit(void) {
  SNES::system.term();
}

void retro_reset(void) {
  SNES::system.reset();
}

void retro_run(void) {
  SNES::system.run();
}

size_t retro_serialize_size(void) {
  return SNES::system.serialize_size;
}

bool retro_serialize(void *data, size_t size) {
  SNES::system.runtosave();
  serializer s = SNES::system.serialize();
  if(s.size() > size) return false;
  memcpy(data, s.data(), s.size());
  return true;
}

bool retro_unserialize(const void *data, size_t size) {
  serializer s((const uint8_t*)data, size);
  return SNES::system.unserialize(s);
}

struct CheatList {
  bool enable;
  string code;
  CheatList() : enable(false) {}
};

static linear_vector<CheatList> cheatList;

void retro_cheat_reset(void) {
  cheatList.reset();
  interface.setCheats();
}

void retro_cheat_set(unsigned index, bool enable, const char *code) {
  cheatList[index].enable = enable;
  cheatList[index].code = code;
  lstring list;
  for(unsigned n = 0; n < cheatList.size(); n++) {
    if(cheatList[n].enable) list.append(cheatList[n].code);
  }
  interface.setCheats(list);
}

void retro_get_system_info(struct retro_system_info *info) {
  static string version("v085 (", SNES::Info::Profile, ")");
  info->library_name     = "bSNES";
  info->library_version  = version;
  info->valid_extensions = 0;
  info->need_fullpath    = false;
}

void retro_get_system_av_info(struct retro_system_av_info *info) {
  struct retro_system_timing timing = { 0.0, 32040.5 };
  timing.fps = retro_get_region() == RETRO_REGION_NTSC ? 21477272.0 / 357366.0 : 21281370.0 / 425568.0;

  if (!interface.penviron(RETRO_ENVIRONMENT_GET_OVERSCAN, &interface.overscan))
     interface.overscan = false;

  struct retro_game_geometry geom = { 256, interface.overscan ? 239 : 224, 512, interface.overscan ? 478 : 448 };

  info->timing   = timing;
  info->geometry = geom;
}

static bool snes_load_cartridge_normal(
  const char *rom_xml, const uint8_t *rom_data, unsigned rom_size
) {
  if(rom_data) SNES::cartridge.rom.copy(rom_data, rom_size);
  string xmlrom = (rom_xml && *rom_xml) ? string(rom_xml) : SnesCartridge(rom_data, rom_size).markup;
  SNES::cartridge.load(SNES::Cartridge::Mode::Normal, xmlrom);
  SNES::system.power();
  return true;
}

static bool snes_load_cartridge_bsx_slotted(
  const char *rom_xml, const uint8_t *rom_data, unsigned rom_size,
  const char *bsx_xml, const uint8_t *bsx_data, unsigned bsx_size
) {
  if(rom_data) SNES::cartridge.rom.copy(rom_data, rom_size);
  string xmlrom = (rom_xml && *rom_xml) ? string(rom_xml) : SnesCartridge(rom_data, rom_size).markup;
  if(bsx_data) SNES::bsxflash.memory.copy(bsx_data, bsx_size);
  string xmlbsx = (bsx_xml && *bsx_xml) ? string(bsx_xml) : SnesCartridge(bsx_data, bsx_size).markup;
  SNES::cartridge.load(SNES::Cartridge::Mode::BsxSlotted, xmlrom);
  SNES::system.power();
  return true;
}

static bool snes_load_cartridge_bsx(
  const char *rom_xml, const uint8_t *rom_data, unsigned rom_size,
  const char *bsx_xml, const uint8_t *bsx_data, unsigned bsx_size
) {
  if(rom_data) SNES::cartridge.rom.copy(rom_data, rom_size);
  string xmlrom = (rom_xml && *rom_xml) ? string(rom_xml) : SnesCartridge(rom_data, rom_size).markup;
  if(bsx_data) SNES::bsxflash.memory.copy(bsx_data, bsx_size);
  string xmlbsx = (bsx_xml && *bsx_xml) ? string(bsx_xml) : SnesCartridge(bsx_data, bsx_size).markup;
  SNES::cartridge.load(SNES::Cartridge::Mode::Bsx, xmlrom);
  SNES::system.power();
  return true;
}

static bool snes_load_cartridge_sufami_turbo(
  const char *rom_xml, const uint8_t *rom_data, unsigned rom_size,
  const char *sta_xml, const uint8_t *sta_data, unsigned sta_size,
  const char *stb_xml, const uint8_t *stb_data, unsigned stb_size
) {
  if(rom_data) SNES::cartridge.rom.copy(rom_data, rom_size);
  string xmlrom = (rom_xml && *rom_xml) ? string(rom_xml) : SnesCartridge(rom_data, rom_size).markup;
  if(sta_data) SNES::sufamiturbo.slotA.rom.copy(sta_data, sta_size);
  string xmlsta = (sta_xml && *sta_xml) ? string(sta_xml) : SnesCartridge(sta_data, sta_size).markup;
  if(stb_data) SNES::sufamiturbo.slotB.rom.copy(stb_data, stb_size);
  string xmlstb = (stb_xml && *stb_xml) ? string(stb_xml) : SnesCartridge(stb_data, stb_size).markup;
  SNES::cartridge.load(SNES::Cartridge::Mode::SufamiTurbo, xmlrom);
  SNES::system.power();
  return true;
}

static bool snes_load_cartridge_super_game_boy(
  const char *rom_xml, const uint8_t *rom_data, unsigned rom_size,
  const char *dmg_xml, const uint8_t *dmg_data, unsigned dmg_size
) {
  if(rom_data) SNES::cartridge.rom.copy(rom_data, rom_size);
  string xmlrom = (rom_xml && *rom_xml) ? string(rom_xml) : SnesCartridge(rom_data, rom_size).markup;
  if(dmg_data) {
    //GameBoyCartridge needs to modify dmg_data (for MMM01 emulation); so copy data
    uint8_t *data = new uint8_t[dmg_size];
    memcpy(data, dmg_data, dmg_size);
    string xmldmg = (dmg_xml && *dmg_xml) ? string(dmg_xml) : GameBoyCartridge(data, dmg_size).markup;
    GameBoy::cartridge.load(GameBoy::System::Revision::SuperGameBoy, xmldmg, data, dmg_size);
    delete[] data;
  }
  SNES::cartridge.load(SNES::Cartridge::Mode::SuperGameBoy, xmlrom);
  SNES::system.power();
  return true;
}

bool retro_load_game(const struct retro_game_info *info) {
  retro_cheat_reset();
  if (info->path) {
    interface.basename = info->path;
    char *dot = strrchr(interface.basename(), '.');
    if (dot)
       *dot = '\0';
  }

  return snes_load_cartridge_normal(info->meta, (const uint8_t*)info->data, info->size);
}

bool retro_load_game_special(unsigned game_type,
      const struct retro_game_info *info, size_t num_info) {

  retro_cheat_reset();
  if (info[0].path) {
    interface.basename = info[0].path;
    char *dot = strrchr(interface.basename(), '.');
    if (dot)
       *dot = '\0';
  }

  switch (game_type) {
     case RETRO_GAME_TYPE_BSX:
       return num_info == 2 && snes_load_cartridge_bsx(info[0].meta, (const uint8_t*)info[0].data, info[0].size,
             info[1].meta, (const uint8_t*)info[1].data, info[1].size);
       
     case RETRO_GAME_TYPE_BSX_SLOTTED:
       return num_info == 2 && snes_load_cartridge_bsx_slotted(info[0].meta, (const uint8_t*)info[0].data, info[0].size,
             info[1].meta, (const uint8_t*)info[1].data, info[1].size);

     case RETRO_GAME_TYPE_SUPER_GAME_BOY:
       return num_info == 2 && snes_load_cartridge_super_game_boy(info[0].meta, (const uint8_t*)info[0].data, info[0].size,
             info[1].meta, (const uint8_t*)info[1].data, info[1].size);

     case RETRO_GAME_TYPE_SUFAMI_TURBO:
       return num_info == 3 && snes_load_cartridge_sufami_turbo(info[0].meta, (const uint8_t*)info[0].data, info[0].size,
             info[1].meta, (const uint8_t*)info[1].data, info[1].size,
             info[2].meta, (const uint8_t*)info[2].data, info[2].size);

     default:
       return false;
  }
}

void retro_unload_game(void) {
  SNES::cartridge.unload();
}

unsigned retro_get_region(void) {
  return SNES::system.region.i == SNES::System::Region::NTSC ? 0 : 1;
}

void* retro_get_memory_data(unsigned id) {
  if(SNES::cartridge.loaded() == false) return 0;

  switch(id) {
    case RETRO_MEMORY_SAVE_RAM:
      return SNES::cartridge.ram.data();
    case RETRO_MEMORY_RTC:
      if(SNES::cartridge.has_srtc()) return SNES::srtc.rtc;
      if(SNES::cartridge.has_spc7110rtc()) return SNES::spc7110.rtc;
      return 0;
    case RETRO_MEMORY_SNES_BSX_RAM:
      if(SNES::cartridge.mode.i != SNES::Cartridge::Mode::Bsx) break;
      return SNES::bsxcartridge.sram.data();
    case RETRO_MEMORY_SNES_BSX_PRAM:
      if(SNES::cartridge.mode.i != SNES::Cartridge::Mode::Bsx) break;
      return SNES::bsxcartridge.psram.data();
    case RETRO_MEMORY_SNES_SUFAMI_TURBO_A_RAM:
      if(SNES::cartridge.mode.i != SNES::Cartridge::Mode::SufamiTurbo) break;
      return SNES::sufamiturbo.slotA.ram.data();
    case RETRO_MEMORY_SNES_SUFAMI_TURBO_B_RAM:
      if(SNES::cartridge.mode.i != SNES::Cartridge::Mode::SufamiTurbo) break;
      return SNES::sufamiturbo.slotB.ram.data();
    case RETRO_MEMORY_SNES_GAME_BOY_RAM:
      if(SNES::cartridge.mode.i != SNES::Cartridge::Mode::SuperGameBoy) break;
      return GameBoy::cartridge.ramdata;

    case RETRO_MEMORY_SYSTEM_RAM:
      return SNES::cpu.wram;
  }

  return 0;
}

size_t retro_get_memory_size(unsigned id) {
  if(SNES::cartridge.loaded() == false) return 0;
  size_t size = 0;

  switch(id) {
    case RETRO_MEMORY_SAVE_RAM:
      size = SNES::cartridge.ram.size();
      break;
    case RETRO_MEMORY_RTC:
      if(SNES::cartridge.has_srtc() || SNES::cartridge.has_spc7110rtc()) size = 20;
      break;
    case RETRO_MEMORY_SNES_BSX_RAM:
      if(SNES::cartridge.mode.i != SNES::Cartridge::Mode::Bsx) break;
      size = SNES::bsxcartridge.sram.size();
      break;
    case RETRO_MEMORY_SNES_BSX_PRAM:
      if(SNES::cartridge.mode.i != SNES::Cartridge::Mode::Bsx) break;
      size = SNES::bsxcartridge.psram.size();
      break;
    case RETRO_MEMORY_SNES_SUFAMI_TURBO_A_RAM:
      if(SNES::cartridge.mode.i != SNES::Cartridge::Mode::SufamiTurbo) break;
      size = SNES::sufamiturbo.slotA.ram.size();
      break;
    case RETRO_MEMORY_SNES_SUFAMI_TURBO_B_RAM:
      if(SNES::cartridge.mode.i != SNES::Cartridge::Mode::SufamiTurbo) break;
      size = SNES::sufamiturbo.slotB.ram.size();
      break;
    case RETRO_MEMORY_SNES_GAME_BOY_RAM:
      if(SNES::cartridge.mode.i != SNES::Cartridge::Mode::SuperGameBoy) break;
      size = GameBoy::cartridge.ramsize;
      break;

    case RETRO_MEMORY_SYSTEM_RAM:
      size = 128 * 1024;
      break;
  }

  if(size == -1U) size = 0;
  return size;
}

