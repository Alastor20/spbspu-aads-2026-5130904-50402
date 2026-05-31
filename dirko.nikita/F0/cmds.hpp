#ifndef CMDS_HPP
#define CMDS_HPP
#include <iosfwd>
#include "player.hpp"
namespace dirko
{
  using cmd_t = void (*)(std::istream &, std::ostream &, playlists_t &, pl_iter &, pls_iter &, save_t &);
  void play(std::istream &, std::ostream &, playlists_t &, pl_iter &, pls_iter &, save_t &);
  void next(std::istream &, std::ostream &, playlists_t &, pl_iter &, pls_iter &, save_t &);
  void prev(std::istream &, std::ostream &, playlists_t &, pl_iter &, pls_iter &, save_t &);
  void add(std::istream &, std::ostream &, playlists_t &, pl_iter &, pls_iter &, save_t &);
  void remove(std::istream &, std::ostream &, playlists_t &, pl_iter &, pls_iter &, save_t &);
  void rename(std::istream &, std::ostream &, playlists_t &, pl_iter &, pls_iter &, save_t &);
  void loop(std::istream &, std::ostream &, playlists_t &, pl_iter &, pls_iter &, save_t &);
  void random(std::istream &, std::ostream &, playlists_t &, pl_iter &, pls_iter &, save_t &);
  void list(std::istream &, std::ostream &, playlists_t &, pl_iter &, pls_iter &, save_t &);
  void playlist_add(std::istream &, std::ostream &, playlists_t &, pl_iter &, pls_iter &, save_t &);
  void playlist_remove(std::istream &, std::ostream &, playlists_t &, pl_iter &, pls_iter &, save_t &);
  void playlist_rename(std::istream &, std::ostream &, playlists_t &, pl_iter &, pls_iter &, save_t &);
  void playlist_add_track(std::istream &, std::ostream &, playlists_t &, pl_iter &, pls_iter &, save_t &);
  void playlist_remove_track(std::istream &, std::ostream &, playlists_t &, pl_iter &, pls_iter &, save_t &);
  void playlist_get(std::istream &, std::ostream &, playlists_t &, pl_iter &, pls_iter &, save_t &);
  void playlist_list(std::istream &, std::ostream &, playlists_t &, pl_iter &, pls_iter &, save_t &);
  void playlist_merge(std::istream &, std::ostream &, playlists_t &, pl_iter &, pls_iter &, save_t &);
  void playlist_select(std::istream &, std::ostream &, playlists_t &, pl_iter &, pls_iter &, save_t &);
  void playlist_diff(std::istream &, std::ostream &, playlists_t &, pl_iter &, pls_iter &, save_t &);
}
#endif
