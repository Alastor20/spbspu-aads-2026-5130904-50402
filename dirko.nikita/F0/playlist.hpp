#ifndef PLAYLIST_HPP
#define PLAYLIST_HPP
#include <cstddef>
#include <string>
#include "decoder.hpp"
#include "robin.hpp"

namespace dirko
{
  struct Track
  {
    WavFile track_;
    size_t duration_;
    Track(WavFile file);
  };
  using catalog_t = RobinTable< std::string, Track >;
  using playlist_t = RobinTable< std::string, Track & >;
  using playlists_t = RobinTable< std::string, playlist_t >;
  using save_t = RobinTable< std::string, std::string >;
  void remove(playlists_t &, catalog_t &, std::string &);
  void rename(playlists_t &, catalog_t &, std::string &, std::string &);
}
#endif
