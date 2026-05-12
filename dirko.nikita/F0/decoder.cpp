#include "decoder.hpp"
#include <cstdint>
#include <cstring>
#include <fstream>
#include <stdexcept>
#include "fstream"

using namespace dirko;

Audio::Audio(std::string str)
{
  if (str.substr(str.size() - 4) != ".wav")
    throw std::invalid_argument("Can only read WAV files!");
  load_wav(str);
}

void Audio::load_wav(std::string str)
{
  std::ifstream file(str, std::ios::binary);
  if (!file.is_open()) {
    throw std::runtime_error("Cant read file");
  }
  // Chunk
  file.read(type, 4);
  if (strcmp(type, "RIFF"))
    throw std::runtime_error("Not a RIFF file!");

  file.read(reinterpret_cast< char * >(&ChunkSize), sizeof(ChunkSize));
  file.read(format, 4);
  if (strcmp(format, "WAVE"))
    throw std::runtime_error("Not a WAVE format!");

  // 1st Subchunk
  file.read(Subchunk1ID, 4);
  if (strcmp(Subchunk1ID, "fmt "))
    throw std::runtime_error("Missing fmt header!");

  file.read(reinterpret_cast< char * >(&Subchunk1Size), sizeof(Subchunk1Size));
  file.read(reinterpret_cast< char * >(&AudioFormat), sizeof(AudioFormat));
  file.read(reinterpret_cast< char * >(&NumChannels), sizeof(NumChannels));
  file.read(reinterpret_cast< char * >(&SampleRate), sizeof(SampleRate));
  file.read(reinterpret_cast< char * >(&ByteRate), sizeof(ByteRate));
  file.read(reinterpret_cast< char * >(&BlockAlign), sizeof(BlockAlign));
  file.read(reinterpret_cast< char * >(&BitsPerSample), sizeof(BitsPerSample));

  // 2nd Subchunk
  file.read(Subchunk2ID, 4);
  if (strcmp(Subchunk2ID, "data") != 0)
    throw std::runtime_error("Missing data header!");
  file.read(reinterpret_cast< char * >(&Subchunk2Size), sizeof(Subchunk2Size));
  // Data

  // Subchunk2Size = NumSamples * NumChannels * BitsPerSample/8
  int NumSamples = Subchunk2Size / (NumChannels * (BitsPerSample / 8));

  data = std::vector< std::pair< int16_t, int16_t > >(NumSamples);
  for (int i = 0; i < NumSamples; i++) {
    file.read(reinterpret_cast< char * >(&data[i].first), sizeof(int16_t));
    file.read(reinterpret_cast< char * >(&data[i].second), sizeof(int16_t));
  }
  file.close();
}
