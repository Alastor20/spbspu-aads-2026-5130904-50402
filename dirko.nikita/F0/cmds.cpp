#include "cmds.hpp"
#include <cstddef>
#include <filesystem>
#include <random>
#include <stdexcept>
#include <string>
#include <utility>
#include "decoder.hpp"
#include "player.hpp"

dirko::save_t SAVER(16, .7);

void dirko::play(std::istream &in, std::ostream &out, playlists_t &db, playlist_iter &tr, playlists_iter &pl)
{
  std::string name;
  double dur;
  in >> name >> dur;
  tr = (*pl).second.getIter(name);
  if (dur > (*tr).second.duration_) {
    out << "playing until end of track\n";
    dur = (*tr).second.duration_;
  }
  PlayWav((*tr).second.track_, dur);
}
void dirko::next(std::istream &, std::ostream &, playlists_t &db, playlist_iter &tr, playlists_iter &pl)
{
  if (tr == (*pl).second.end()) {
    tr = (*pl).second.begin();
    return;
  }
  ++tr;
  PlayWav((*tr).second.track_, (*tr).second.duration_);
}
void dirko::prev(std::istream &, std::ostream &, playlists_t &db, playlist_iter &tr, playlists_iter &pl)
{
  if (tr == (*pl).second.begin()) {
    tr = (*pl).second.end();
  }
  --tr;
  PlayWav((*tr).second.track_, (*tr).second.duration_);
}
void dirko::add(std::istream &in, std::ostream &, playlists_t &db, playlist_iter &, playlists_iter &)
{
  std::string path, name;
  in >> path >> name;
  WavFile wav;
  Load(path, wav);
  Track track(wav);
  db.get("DEFAULT").add(name, track);
  SAVER.add(name, std::filesystem::absolute(std::filesystem::path(path)).string());
}
void dirko::remove(std::istream &in, std::ostream &, playlists_t &db, playlist_iter &, playlists_iter &)
{
  std::string name;
  in >> name;
  if (!db.get("DEFAULT").has(name)) {
    throw std::invalid_argument("no suck track");
  }
  for (std::pair< std::string, playlist_t > &v : db) {
    if (v.second.has(name)) {
      v.second.drop(name);
    }
  }
}
void dirko::rename(std::istream &in, std::ostream &, playlists_t &db, playlist_iter &, playlists_iter &)
{
  std::string from, to;
  in >> from >> to;
  if (!db.get("DEFAULT").has(from)) {
    throw std::invalid_argument("no suck track");
  }
  for (std::pair< std::string, playlist_t > &v : db) {
    if (v.second.has(from)) {
      v.second.changeKey(from, to);
    }
  }
}
void dirko::loop(std::istream &in, std::ostream &, playlists_t &db, playlist_iter &tr, playlists_iter &pl)
{
  size_t times;
  in >> times;
  for (size_t i = 0; i < times; ++i) {
    PlayWav((*tr).second.track_, (*tr).second.duration_);
  }
}
void dirko::random(std::istream &, std::ostream &, playlists_t &db, playlist_iter &tr, playlists_iter &pl)
{
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<> distrib(1, (*pl).second.size());
  tr = (*pl).second.begin();
  for (size_t i = 0; i < distrib(gen); ++i) {
    ++pl;
  }
  PlayWav((*tr).second.track_, (*tr).second.duration_);
}
void dirko::list(std::istream &, std::ostream &out, playlists_t &db, playlist_iter &, playlists_iter &)
{
  for (const std::pair< std::string, Track > &v : db.get("DEFAULT")) {
    out << v.first << '\n';
  }
}
void dirko::playlist_add(std::istream &in, std::ostream &, playlists_t &db, playlist_iter &, playlists_iter &)
{
  std::string name;
  in >> name;
  db.add(name, playlist_t());
}
void dirko::playlist_remove(std::istream &in, std::ostream &, playlists_t &db, playlist_iter &, playlists_iter &)
{
  std::string name;
  in >> name;
  db.drop(name);
}
void dirko::playlist_rename(std::istream &in, std::ostream &, playlists_t &db, playlist_iter &, playlists_iter &)
{
  std::string from, to;
  in >> from >> to;
  db.changeKey(from, to);
}
void dirko::playlist_add_track(std::istream &in, std::ostream &, playlists_t &db, playlist_iter &, playlists_iter &)
{
  std::string playlist, track;
  in >> playlist >> track;
  db.get(playlist).add(track, db.get("DEFAULT").get(track));
}
void dirko::playlist_remove_track(std::istream &in, std::ostream &, playlists_t &db, playlist_iter &, playlists_iter &)
{
  std::string playlist, track;
  in >> playlist >> track;
  db.get(playlist).drop(track);
}
void dirko::playlist_list(std::istream &in, std::ostream &out, playlists_t &db, playlist_iter &, playlists_iter &)
{
  std::string name;
  in >> name;
  for (const std::pair< std::string, Track > &v : db.get("name")) {
    out << v.first << '\n';
  }
}
void dirko::playlist_merge(std::istream &in, std::ostream &, playlists_t &db, playlist_iter &, playlists_iter &)
{
  std::string list1, list2, listRes;
  in >> list1 >> list2 >> listRes;
  if (db.has(listRes)) {
    throw std::invalid_argument("playlist woth this name already exitsts");
  }
  playlist_t newList(10, .7);
  for (const std::pair< std::string, Track > &v : db.get(list1)) {
    newList.add(v.first, v.second);
  }
  for (const std::pair< std::string, Track > &v : db.get(list2)) {
    if (!newList.has(v.first)) {
      newList.add(v.first, v.second);
    }
  }
  db.add(listRes, newList);
}
void dirko::playlist_select(std::istream &in, std::ostream &, playlists_t &db, playlist_iter &tr, playlists_iter &pl)
{
  std::string name;
  in >> name;
  pl = db.getIter(name);
}
void dirko::playlist_diff(std::istream &in, std::ostream &, playlists_t &db, playlist_iter &, playlists_iter &)
{
  std::string list1, list2, listRes;
  in >> list1 >> list2 >> listRes;
  if (db.has(listRes)) {
    throw std::invalid_argument("playlist woth this name already exitsts");
  }
  playlist_t newList(10, .7);
  for (const std::pair< std::string, Track > &v : db.get(list1)) {
    if (!db.get(list2).has(v.first)) {
      newList.add(v.first, v.second);
    }
  }
  for (const std::pair< std::string, Track > &v : db.get(list2)) {
    if (!db.get(list1).has(v.first)) {
      newList.add(v.first, v.second);
    }
  }
  db.add(listRes, newList);
}
