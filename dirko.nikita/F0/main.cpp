#include <iostream>
#include <limits>
#include <string>
#include "cmds.hpp"
#include "player.hpp"
#include "robin.hpp"

int main()
{
  dirko::playlists_t db(5, .7);
  db.add("All", dirko::playlist_t(5, .7));
  dirko::RobinTable< std::string, dirko::cmd_t > cmds(20, .7);
  cmds.add("play", dirko::play);
  cmds.add("next", dirko::next);
  cmds.add("prev", dirko::prev);
  cmds.add("add", dirko::add);
  cmds.add("remove", dirko::remove);
  cmds.add("rename", dirko::rename);
  cmds.add("list", dirko::list);
  cmds.add("loop", dirko::loop);
  cmds.add("random", dirko::random);
  cmds.add("playlist-add", dirko::playlist_add);
  cmds.add("playlist-remove", dirko::playlist_remove);
  cmds.add("playlist-rename", dirko::playlist_rename);
  cmds.add("playlist-add-track", dirko::playlist_add_track);
  cmds.add("playlist-remove-track", dirko::playlist_remove_track);
  cmds.add("playlist-get", dirko::playlist_get);
  cmds.add("playlist-list", dirko::playlist_list);
  cmds.add("playlist-merge", dirko::playlist_merge);
  cmds.add("playlist-select", dirko::playlist_select);
  cmds.add("playlist-diff", dirko::playlist_diff);

  dirko::pls_iter_t playlist = db.begin();
  dirko::pl_iter_t track = (*playlist).second.begin();
  dirko::save_t saver(6, .7);

  dirko::load(saver, std::cout, db);

  std::string cmd;
  while (std::cin >> cmd) {
    try {
      if (std::cin.fail()) {
        std::cin.clear(std::cin.rdstate() & ~std::ios::failbit);
        std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
      }
      cmds.get(cmd)(std::cin, std::cout, db, track, playlist, saver);
    } catch (const std::exception &e) {
      std::cout << '<' << e.what() << ">\n";
      std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
    }
  }
  dirko::save(saver);
}
