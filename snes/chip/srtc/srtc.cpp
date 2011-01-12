#include <snes/snes.hpp>
#include <limits>
#include <time.h>

#define SRTC_CPP
using std::numeric_limits;
namespace SNES {

SRTC srtc;

#include "serialization.cpp"

const unsigned SRTC::months[12] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

void SRTC::init() {
}

void SRTC::enable() {
}

void SRTC::power() {
  reset();
}

void SRTC::reset() {
  rtc_mode = RtcRead;
  rtc_index = -1;
  update_time();
}

void SRTC::update_time() {
  time_t rtc_time
  = (memory::cartrtc.read(16) <<  0)
  | (memory::cartrtc.read(17) <<  8)
  | (memory::cartrtc.read(18) << 16)
  | (memory::cartrtc.read(19) << 24);
  time_t current_time = time(0);

  //sizeof(time_t) is platform-dependent; though memory::cartrtc needs to be platform-agnostic.
  //yet platforms with 32-bit signed time_t will overflow every ~68 years. handle this by
  //accounting for overflow at the cost of 1-bit precision (to catch underflow). this will allow
  //memory::cartrtc timestamp to remain valid for up to ~34 years from the last update, even if
  //time_t overflows. calculation should be valid regardless of number representation, time_t size,
  //or whether time_t is signed or unsigned.
  time_t diff
  = (current_time >= rtc_time)
  ? (current_time - rtc_time)
  : (numeric_limits<time_t>::max() - rtc_time + current_time + 1);  //compensate for overflow
  if(diff > numeric_limits<time_t>::max() / 2) diff = 0;            //compensate for underflow

  if(diff > 0) {
    unsigned second  = memory::cartrtc.read( 0) + memory::cartrtc.read( 1) * 10;
    unsigned minute  = memory::cartrtc.read( 2) + memory::cartrtc.read( 3) * 10;
    unsigned hour    = memory::cartrtc.read( 4) + memory::cartrtc.read( 5) * 10;
    unsigned day     = memory::cartrtc.read( 6) + memory::cartrtc.read( 7) * 10;
    unsigned month   = memory::cartrtc.read( 8);
    unsigned year    = memory::cartrtc.read( 9) + memory::cartrtc.read(10) * 10 + memory::cartrtc.read(11) * 100;
    unsigned weekday = memory::cartrtc.read(12);

    day--;
    month--;
    year += 1000;

    second += diff;
    while(second >= 60) {
      second -= 60;

      minute++;
      if(minute < 60) continue;
      minute = 0;

      hour++;
      if(hour < 24) continue;
      hour = 0;

      day++;
      weekday = (weekday + 1) % 7;
      unsigned days = months[month % 12];
      if(days == 28) {
        bool leapyear = false;
        if((year % 4) == 0) {
          leapyear = true;
          if((year % 100) == 0 && (year % 400) != 0) leapyear = false;
        }
        if(leapyear) days++;
      }
      if(day < days) continue;
      day = 0;

      month++;
      if(month < 12) continue;
      month = 0;

      year++;
    }

    day++;
    month++;
    year -= 1000;

    memory::cartrtc.write( 0, second % 10);
    memory::cartrtc.write( 1, second / 10);
    memory::cartrtc.write( 2, minute % 10);
    memory::cartrtc.write( 3, minute / 10);
    memory::cartrtc.write( 4, hour % 10);
    memory::cartrtc.write( 5, hour / 10);
    memory::cartrtc.write( 6, day % 10);
    memory::cartrtc.write( 7, day / 10);
    memory::cartrtc.write( 8, month);
    memory::cartrtc.write( 9, year % 10);
    memory::cartrtc.write(10, (year / 10) % 10);
    memory::cartrtc.write(11, year / 100);
    memory::cartrtc.write(12, weekday % 7);
  }

  memory::cartrtc.write(16, current_time >>  0);
  memory::cartrtc.write(17, current_time >>  8);
  memory::cartrtc.write(18, current_time >> 16);
  memory::cartrtc.write(19, current_time >> 24);
}

//returns day of week for specified date
//eg 0 = Sunday, 1 = Monday, ... 6 = Saturday
//usage: weekday(2008, 1, 1) returns weekday of January 1st, 2008
unsigned SRTC::weekday(unsigned year, unsigned month, unsigned day) {
  unsigned y = 1900, m = 1;  //epoch is 1900-01-01
  unsigned sum = 0;          //number of days passed since epoch

  year = max(1900, year);
  month = max(1, min(12, month));
  day = max(1, min(31, day));

  while(y < year) {
    bool leapyear = false;
    if((y % 4) == 0) {
      leapyear = true;
      if((y % 100) == 0 && (y % 400) != 0) leapyear = false;
    }
    sum += leapyear ? 366 : 365;
    y++;
  }

  while(m < month) {
    unsigned days = months[m - 1];
    if(days == 28) {
      bool leapyear = false;
      if((y % 4) == 0) {
        leapyear = true;
        if((y % 100) == 0 && (y % 400) != 0) leapyear = false;
      }
      if(leapyear) days++;
    }
    sum += days;
    m++;
  }

  sum += day - 1;
  return (sum + 1) % 7;  //1900-01-01 was a Monday
}

uint8 SRTC::mmio_read(unsigned addr) {
  addr &= 0xffff;

  if(addr == 0x2800) {
    if(rtc_mode != RtcRead) return 0x00;

    if(rtc_index < 0) {
      update_time();
      rtc_index++;
      return 0x0f;
    } else if(rtc_index > 12) {
      rtc_index = -1;
      return 0x0f;
    } else {
      return memory::cartrtc.read(rtc_index++);
    }
  }

  return cpu.regs.mdr;
}

void SRTC::mmio_write(unsigned addr, uint8 data) {
  addr &= 0xffff;

  if(addr == 0x2801) {
    data &= 0x0f;  //only the low four bits are used

    if(data == 0x0d) {
      rtc_mode = RtcRead;
      rtc_index = -1;
      return;
    }

    if(data == 0x0e) {
      rtc_mode = RtcCommand;
      return;
    }

    if(data == 0x0f) return;  //unknown behavior

    if(rtc_mode == RtcWrite) {
      if(rtc_index >= 0 && rtc_index < 12) {
        memory::cartrtc.write(rtc_index++, data);

        if(rtc_index == 12) {
          //day of week is automatically calculated and written
          unsigned day   = memory::cartrtc.read( 6) + memory::cartrtc.read( 7) * 10;
          unsigned month = memory::cartrtc.read( 8);
          unsigned year  = memory::cartrtc.read( 9) + memory::cartrtc.read(10) * 10 + memory::cartrtc.read(11) * 100;
          year += 1000;

          memory::cartrtc.write(rtc_index++, weekday(year, month, day));
        }
      }
    } else if(rtc_mode == RtcCommand) {
      if(data == 0) {
        rtc_mode = RtcWrite;
        rtc_index = 0;
      } else if(data == 4) {
        rtc_mode = RtcReady;
        rtc_index = -1;
        for(unsigned i = 0; i < 13; i++) memory::cartrtc.write(i, 0);
      } else {
        //unknown behavior
        rtc_mode = RtcReady;
      }
    }
  }
}

SRTC::SRTC() {
}

}
