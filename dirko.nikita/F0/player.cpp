#include "player.hpp"
#include <fstream>
#include <ostream>
#include <stdexcept>
#include <string>
#include <utility>
#include "decoder.hpp"

dirko::Track::Track(dirko::WavFile wav):
  track_(wav),
  duration_(static_cast< double >(wav.rawData.getSize()) / wav.byteRate)
{}
void dirko::PlayWav(dirko::WavFile &wav, double dur)
{
  if (SDL_Init(SDL_INIT_AUDIO) != 0) {
    throw SDL_GetError();
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
    throw std::runtime_error("Unsupported bit depth");
  }

  spec.samples = 4096;
  spec.callback = nullptr;

  SDL_AudioDeviceID device = SDL_OpenAudioDevice(nullptr, 0, &spec, nullptr, 0);

  if (!device) {
    throw SDL_GetError();
  }

  SDL_QueueAudio(device, wav.rawData.getData(), wav.rawData.getSize());

  SDL_PauseAudioDevice(device, 0);

  SDL_Delay(static_cast< Uint32 >(dur * 1000));
  SDL_CloseAudioDevice(device);
  SDL_Quit();
}

void dirko::save(const save_t &saver)
{
  std::ofstream file("db.save");
  for (const std::pair< std::string, boost::filesystem::path > &v : saver) {
    file << v.first << ';' << v.second.string() << ';';
  }
}

void dirko::load(save_t &saver, std::ostream &out, playlists_t &playlists)
{
  std::ifstream file("db.save");
  if (!file.is_open()) {
    out << "No save file provided\n";
    return;
  }
  std::string name, path;
  WavFile buffer;
  playlist_t &defaultPl = playlists.get("All");
  while (std::getline(file, name, ';')) {
    std::getline(file, path, ';');
    try {
      Load(path, buffer);
    } catch (const std::invalid_argument &) {
      out << "failed to add " << name << '\n';
      continue;
    }
    defaultPl.add(name, Track(buffer));
    saver.add(name, boost::filesystem::path(path));
  }
}
