#ifndef CMDS_HPP
#define CMDS_HPP
#include <iosfwd>
#include "playlist.hpp"
namespace dirko
{
  void play(std::istream &, std::ostream &, playlists_t &);
  void next(std::istream &, std::ostream &, playlists_t &);
  void prev(std::istream &, std::ostream &, playlists_t &);
  void add(std::istream &, std::ostream &, playlists_t &);
  void remove(std::istream &, std::ostream &, playlists_t &);
  void rename(std::istream &, std::ostream &, playlists_t &);
  void loop(std::istream &, std::ostream &, playlists_t &);
  void random(std::istream &, std::ostream &, playlists_t &);
  void list(std::istream &, std::ostream &, playlists_t &);
  void playlist_add(std::istream &, std::ostream &, playlists_t &);
  void playlist_remove(std::istream &, std::ostream &, playlists_t &);
  void playlist_rename(std::istream &, std::ostream &, playlists_t &);
  void playlist_add_track(std::istream &, std::ostream &, playlists_t &);
  void playlist_remove_track(std::istream &, std::ostream &, playlists_t &);
  void playlist_list(std::istream &, std::ostream &, playlists_t &);
  void playlist_merge(std::istream &, std::ostream &, playlists_t &);
  void playlist_select(std::istream &, std::ostream &, playlists_t &);
  void playlist_diff(std::istream &, std::ostream &, playlists_t &);
}
#endif
