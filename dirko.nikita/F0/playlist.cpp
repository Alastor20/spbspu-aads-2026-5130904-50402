#include "playlist.hpp"

void dirko::remove(playlists_t &playlists, catalog_t &tracks, std::string &toRemove)
{
  tracks.drop(toRemove);
  for (std::pair< std::string, playlist_t > &v : playlists) {
    if (v.second.has(toRemove)) {
      v.second.drop(toRemove);
    }
  }
}

void dirko::rename(playlists_t &playlists, catalog_t &tracks, std::string &from, std::string &to)
{
  tracks.changeKey(from, to);
  for (std::pair< std::string, playlist_t > &v : playlists) {
    if (v.second.has(from)) {
      v.second.changeKey(from, to);
    }
  }
}
