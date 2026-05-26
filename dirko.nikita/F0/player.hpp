#ifndef PLAYER_TYPES_HPP
#define PLAYER_TYPES_HPP
#include <filesystem>
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

  using playlist_t = RobinTable< std::string, Track >;
  using pl_iter = RTIter< std::string, Track >;
  using playlists_t = RobinTable< std::string, playlist_t >;
  using pls_iter = RTIter< std::string, playlist_t >;
  using save_t = RobinTable< std::string, std::filesystem::path >;

  void PlayWav(dirko::WavFile &wav, double dur);
  void save(const save_t &);
  void load(save_t &, std::ostream &);
}
#endif
