#ifndef DECODER_HPP
#define DECODER_HPP

#include <cstdint>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>
namespace dirko
{
  class Audio
  {
  public:
    // Constructors
    Audio(std::string str);
    Audio() {};

    // Read / Write files
    void load_wav(std::string str);

    // Get functions
    // Subchunk2Size = NumSamples * NumChannels * BitsPerSample / 8 <==> NumSamples = Subchunk2Size /
    // (NumChannels*(BitsPerSample / 8))
    unsigned get_size()
    {
      return data.size();
    }
    unsigned get_sample_rate()
    {
      return SampleRate;
    }
    unsigned get_n_channels()
    {
      return NumChannels;
    }

    // Set Functions
    void set_sample_rate(int n)
    {
      SampleRate = n;
    }
    void set_n_channels(int n)
    {
      if (n != 1 && n != 2)
        throw std::invalid_argument("n can only be 1 (MONO) or 2 (STEREO)!");
      NumChannels = n;
    }

    // Overloaded Operators
    std::pair< short, short > &operator[](unsigned i)
    {
      return data[i];
    }

  protected:
    char type[5];
    char format[5];
    char Subchunk1ID[5];
    char Subchunk2ID[5];

    int32_t ChunkSize;
    int32_t Subchunk1Size;
    int32_t SampleRate;
    int32_t ByteRate;
    int32_t Subchunk2Size;

    int16_t AudioFormat;
    int16_t NumChannels;
    int16_t BlockAlign;
    int16_t BitsPerSample;

    // utility
    uint32_t NumSamples;

    std::vector< std::pair< int16_t, int16_t > > data;
  };
}
#endif
