#ifndef PLAYER_TYPES_HPP
#define PLAYER_TYPES_HPP
#include <string>
#include "decoder.hpp"
#include "robin.hpp"

namespace dirko
{
  struct Track
  {
    WavFile track_;
    double duration_;
    Track() = default;
    Track(WavFile file);
  };
  void PlayWav(dirko::WavFile &wav, double dur);
  using playlist_t = RobinTable< std::string, Track >;
  using playlist_iter = RTIter< std::string, Track >;
  using playlists_t = RobinTable< std::string, playlist_t >;
  using playlists_iter = RTIter< std::string, playlist_t >;
  using save_t = RobinTable< std::string, std::string >;
}
#endif
