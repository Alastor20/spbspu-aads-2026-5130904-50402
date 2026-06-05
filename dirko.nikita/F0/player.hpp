#ifndef PLAYER_TYPES_HPP
#define PLAYER_TYPES_HPP
#include <string>
#include "decoder.hpp"
#include "robin.hpp"

#define USE_FAKE_SDL
namespace dirko
{
  struct Track
  {
    WavFile track_;
    double duration_;
    Track() = default;
    Track(WavFile file);
  };

  using playlist_t = RobinTable< std::string, Track >;
  using pl_iter_t = RTIter< std::string, Track >;
  using playlists_t = RobinTable< std::string, playlist_t >;
  using pls_iter_t = RTIter< std::string, playlist_t >;
  using save_t = RobinTable< std::string, std::string >;

  void PlayWav(dirko::WavFile &wav, double dur);
  void save(const save_t &saver);
  void load(save_t &saver, std::ostream &out, playlists_t &playlists);
}
#endif
