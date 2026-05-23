#ifndef DECODER_HPP
#define DECODER_HPP

#include <string>
#include <SDL2/SDL.h>
#include "../common/Vector.hpp"

namespace dirko
{

  struct WavFile
  {
    Uint16 audioFormat;
    Uint16 numChannels;
    Uint32 sampleRate;
    Uint32 byteRate;
    Uint16 blockAlign;
    Uint16 bitsPerSample;

    Vector< Uint8 > rawData;
  };

  void Load(const std::string &filename, WavFile &outWav);
}
#endif
