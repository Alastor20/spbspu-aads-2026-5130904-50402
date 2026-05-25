#ifndef CMDS_HPP
#define CMDS_HPP
#include <iosfwd>
#include "player.hpp"
namespace dirko
{
  using cmd_t = void (*)(std::istream &, std::ostream &, playlists_t &, playlist_iter &, playlists_iter &);
  void play(std::istream &, std::ostream &, playlists_t &, playlist_iter &, playlists_iter &);
  void next(std::istream &, std::ostream &, playlists_t &, playlist_iter &, playlists_iter &);
  void prev(std::istream &, std::ostream &, playlists_t &, playlist_iter &, playlists_iter &);
  void add(std::istream &, std::ostream &, playlists_t &, playlist_iter &, playlists_iter &);
  void remove(std::istream &, std::ostream &, playlists_t &, playlist_iter &, playlists_iter &);
  void rename(std::istream &, std::ostream &, playlists_t &, playlist_iter &, playlists_iter &);
  void loop(std::istream &, std::ostream &, playlists_t &, playlist_iter &, playlists_iter &);
  void random(std::istream &, std::ostream &, playlists_t &, playlist_iter &, playlists_iter &);
  void list(std::istream &, std::ostream &, playlists_t &, playlist_iter &, playlists_iter &);
  void playlist_add(std::istream &, std::ostream &, playlists_t &, playlist_iter &, playlists_iter &);
  void playlist_remove(std::istream &, std::ostream &, playlists_t &, playlist_iter &, playlists_iter &);
  void playlist_rename(std::istream &, std::ostream &, playlists_t &, playlist_iter &, playlists_iter &);
  void playlist_add_track(std::istream &, std::ostream &, playlists_t &, playlist_iter &, playlists_iter &);
  void playlist_remove_track(std::istream &, std::ostream &, playlists_t &, playlist_iter &, playlists_iter &);
  void playlist_list(std::istream &, std::ostream &, playlists_t &, playlist_iter &, playlists_iter &);
  void playlist_merge(std::istream &, std::ostream &, playlists_t &, playlist_iter &, playlists_iter &);
  void playlist_select(std::istream &, std::ostream &, playlists_t &, playlist_iter &, playlists_iter &);
  void playlist_diff(std::istream &, std::ostream &, playlists_t &, playlist_iter &, playlists_iter &);
}
#endif
