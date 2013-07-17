#-------------------------------------------------
#
# Project created by QtCreator 2013-06-28T12:59:20
#
#-------------------------------------------------

QT       -= core gui

TARGET = sfmplayer
TEMPLATE = lib
CONFIG += staticlib

DEFINES += PROFILE_ACCURACY

INCLUDEPATH += ../..

SOURCES += \
    ../../snes/dsp/voice.cpp \
    ../../snes/dsp/serialization.cpp \
    ../../snes/dsp/misc.cpp \
    ../../snes/dsp/gaussian.cpp \
    ../../snes/dsp/envelope.cpp \
    ../../snes/dsp/echo.cpp \
    ../../snes/dsp/dsp.cpp \
    ../../snes/dsp/counter.cpp \
    ../../snes/dsp/brr.cpp \
    ../../snes/interface/interface.cpp \
    ../../snes/libsfm/libsfm.cpp \
    ../../snes/memory/memory.cpp \
    ../../snes/random/random.cpp \
    ../../snes/smp/smp.cpp \
    ../../snes/smp/serialization.cpp \
    ../../snes/smp/iplrom.cpp \
    ../../snes/smp/core/smp_core.cpp \
    ../../snes/smp/core/serialization.cpp \
    ../../snes/smp/core/opcodes.cpp \
    ../../snes/smp/core/algorithms.cpp \
    ../../snes/smp/memory/memory.cpp \
    ../../snes/smp/timing/timing.cpp \
    ../../snes/system/video.cpp \
    ../../snes/system/system.cpp \
    ../../snes/system/serialization.cpp \
    ../../snes/system/input.cpp \
    ../../snes/system/audio.cpp \
    ../../nall/libmalloc.c

HEADERS += \
    ../../snes/dsp/dsp.hpp \
    ../../snes/interface/interface.hpp \
    ../../snes/libsfm/libsfm.h \
    ../../snes/memory/memory.hpp \
    ../../snes/memory/memory-inline.hpp \
    ../../snes/random/random.hpp \
    ../../snes/smp/smp.hpp \
    ../../snes/smp/core/registers.hpp \
    ../../snes/smp/core/memory.hpp \
    ../../snes/smp/core/core.hpp \
    ../../snes/smp/memory/memory.hpp \
    ../../snes/smp/timing/timing.hpp \
    ../../snes/system/system.hpp \
    ../../snes/system/audio.hpp \
    ../../snes/snes.hpp \
    ../../snes/profile-accuracy.hpp \
    ../../nall/vector.hpp \
    ../../nall/varint.hpp \
    ../../nall/utility.hpp \
    ../../nall/utf8.hpp \
    ../../nall/ups.hpp \
    ../../nall/string.hpp \
    ../../nall/stdint.hpp \
    ../../nall/static.hpp \
    ../../nall/sort.hpp \
    ../../nall/sha256.hpp \
    ../../nall/serializer.hpp \
    ../../nall/serial.hpp \
    ../../nall/reference_array.hpp \
    ../../nall/random.hpp \
    ../../nall/public_cast.hpp \
    ../../nall/property.hpp \
    ../../nall/priorityqueue.hpp \
    ../../nall/platform.hpp \
    ../../nall/moduloarray.hpp \
    ../../nall/memory_debug.hpp \
    ../../nall/lzss.hpp \
    ../../nall/input.hpp \
    ../../nall/function.hpp \
    ../../nall/foreach.hpp \
    ../../nall/filemap.hpp \
    ../../nall/file.hpp \
    ../../nall/endian.hpp \
    ../../nall/dsp.hpp \
    ../../nall/dl.hpp \
    ../../nall/directory.hpp \
    ../../nall/dictionary.hpp \
    ../../nall/detect.hpp \
    ../../nall/crc32.hpp \
    ../../nall/config.hpp \
    ../../nall/concept.hpp \
    ../../nall/C++98.hpp \
    ../../nall/bit.hpp \
    ../../nall/base64.hpp \
    ../../nall/array.hpp \
    ../../nall/algorithm.hpp \
    ../../nall/dsp/settings.hpp \
    ../../nall/dsp/core.hpp \
    ../../nall/dsp/buffer.hpp \
    ../../nall/dsp/resample/sinc.hpp \
    ../../nall/dsp/resample/nearest.hpp \
    ../../nall/dsp/resample/linear.hpp \
    ../../nall/dsp/resample/hermite.hpp \
    ../../nall/dsp/resample/cubic.hpp \
    ../../nall/dsp/resample/cosine.hpp \
    ../../nall/dsp/resample/average.hpp \
    ../../nall/dsp/resample/lib/sinc.hpp \
    ../../nall/snes/smp.hpp \
    ../../nall/snes/cpu.hpp \
    ../../nall/snes/cartridge.hpp \
    ../../nall/string/xml.hpp \
    ../../nall/string/wrapper.hpp \
    ../../nall/string/variadic.hpp \
    ../../nall/string/utility.hpp \
    ../../nall/string/trim.hpp \
    ../../nall/string/strpos.hpp \
    ../../nall/string/strl.hpp \
    ../../nall/string/split.hpp \
    ../../nall/string/replace.hpp \
    ../../nall/string/math.hpp \
    ../../nall/string/match.hpp \
    ../../nall/string/filename.hpp \
    ../../nall/string/cstring.hpp \
    ../../nall/string/core.hpp \
    ../../nall/string/convert.hpp \
    ../../nall/string/compare.hpp \
    ../../nall/string/cast.hpp \
    ../../nall/string/bsv.hpp \
    ../../nall/string/bml.hpp \
    ../../nall/string/base.hpp
unix:!symbian {
    maemo5 {
        target.path = /opt/usr/lib
    } else {
        target.path = /usr/lib
    }
    INSTALLS += target
}

OTHER_FILES +=
