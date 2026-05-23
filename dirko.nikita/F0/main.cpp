#include <iostream>
#include <SDL2/SDL.h>
#include "decoder.hpp"

#define MUS_PATH "C1.wav"

bool PlayWav(dirko::WavFile &wav, double dur)
{
  if (SDL_Init(SDL_INIT_AUDIO) != 0) {
    std::cout << SDL_GetError() << "\n";
    return false;
  }

  SDL_AudioSpec spec{};
  spec.freq = wav.sampleRate;
  spec.channels = wav.numChannels;

  switch (wav.bitsPerSample) {
  case 8:
    spec.format = AUDIO_U8;
    break;

  case 16:
    spec.format = AUDIO_S16LSB;
    break;

  case 32:
    spec.format = AUDIO_S32LSB;
    break;

  default:
    std::cout << "Unsupported bit depth\n";
    return false;
  }

  spec.samples = 4096;
  spec.callback = nullptr;

  SDL_AudioDeviceID device = SDL_OpenAudioDevice(nullptr, 0, &spec, nullptr, 0);

  if (!device) {
    std::cout << SDL_GetError() << "\n";
    return false;
  }

  SDL_QueueAudio(device, wav.rawData.getData(), wav.rawData.getSize());

  SDL_PauseAudioDevice(device, 0);

  double duration = static_cast< double >(wav.rawData.getSize()) / wav.byteRate;
  if (dur > duration) {
    return false;
  }
  SDL_Delay(static_cast< Uint32 >(dur * 1000));

  SDL_CloseAudioDevice(device);
  SDL_Quit();

  return true;
}
int main()
{
  dirko::WavFile wav;
  Load(MUS_PATH, wav);

  double duration = static_cast< double >(wav.rawData.getSize()) / wav.byteRate;
  std::cout << duration << '\n';
  double dur = 0;
  std::cin >> dur;
  PlayWav(wav, dur);
}
