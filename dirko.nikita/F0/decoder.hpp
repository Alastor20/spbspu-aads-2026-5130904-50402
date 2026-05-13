#ifndef DECODER_HPP
#define DECODER_HPP

#include <cstdint>
#include <string>
#include "../common/Vector.hpp"

namespace dirko
{

  struct WavFile
  {
    uint16_t audioFormat;
    uint16_t numChannels;
    uint32_t sampleRate;
    uint32_t byteRate;
    uint16_t blockAlign;
    uint16_t bitsPerSample;

    Vector< uint8_t > rawData;
  };

  class WavDecoder
  {
  public:
    void Load(const std::string &filename, WavFile &outWav);
  };
}
#endif
