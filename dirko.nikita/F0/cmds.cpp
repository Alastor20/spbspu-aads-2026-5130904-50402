#include "cmds.hpp"
#include <cstddef>
#include <random>
#include <stdexcept>
#include <string>
#include <boost/filesystem/operations.hpp>
#include "decoder.hpp"
#include "player.hpp"

using dirko::save_t;

void dirko::play(std::istream &in, std::ostream &out, playlists_t &, pl_iter_t &tr, pls_iter_t &pl, save_t &)
{
  std::string name;
  in >> name;
  tr = (*pl).second.getIter(name);
  out << "duration: " << (*tr).second.duration_ << '\n';
  double dur;
  in >> dur;
  if (dur > (*tr).second.duration_) {
    out << "<playing until end of track>\n";
    dur = (*tr).second.duration_;
  }
  PlayWav((*tr).second.track_, dur);
}
void dirko::next(std::istream &in, std::ostream &out, playlists_t &, pl_iter_t &tr, pls_iter_t &pl, save_t &)
{
  if (tr == (*pl).second.end()) {
    tr = (*pl).second.begin();
  }
  ++tr;
  if (tr == (*pl).second.end()) {
    tr = (*pl).second.begin();
  }
  out << "duration: " << (*tr).second.duration_ << '\n';
  double dur;
  in >> dur;
  if (dur > (*tr).second.duration_) {
    out << "<playing until end of track>\n";
    dur = (*tr).second.duration_;
  }
  PlayWav((*tr).second.track_, dur);
}
void dirko::prev(std::istream &in, std::ostream &out, playlists_t &, pl_iter_t &tr, pls_iter_t &pl, save_t &)
{
  if (tr == (*pl).second.begin()) {
    tr = (*pl).second.end();
  }
  --tr;
  out << "duration: " << (*tr).second.duration_ << '\n';
  double dur;
  in >> dur;
  if (dur > (*tr).second.duration_) {
    out << "<playing until end of track>\n";
    dur = (*tr).second.duration_;
  }
  PlayWav((*tr).second.track_, dur);
}
void dirko::add(std::istream &in, std::ostream &, playlists_t &db, pl_iter_t &, pls_iter_t &, save_t &saver)
{
  boost::filesystem::path path;
  std::string name;
  in >> path >> name;
  WavFile wav;
  Load(path.string(), wav);
  Track track(wav);
  db.get("All").add(name, track);
  saver.add(name, boost::filesystem::absolute(path));
}
void dirko::remove(std::istream &in, std::ostream &, playlists_t &db, pl_iter_t &, pls_iter_t &, save_t &saver)
{
  std::string name;
  in >> name;
  if (!db.get("All").has(name)) {
    throw std::invalid_argument("no such track");
  }
  for (std::pair< std::string, playlist_t > &v : db) {
    if (v.second.has(name)) {
      v.second.drop(name);
    }
  }
  saver.drop(name);
}
void dirko::rename(std::istream &in, std::ostream &, playlists_t &db, pl_iter_t &, pls_iter_t &, save_t &saver)
{
  std::string from, to;
  in >> from >> to;
  if (!db.get("All").has(from)) {
    throw std::invalid_argument("no such track");
  }
  for (std::pair< std::string, playlist_t > &v : db) {
    if (v.second.has(from)) {
      v.second.changeKey(from, to);
    }
  }
  saver.changeKey(from, to);
}
void dirko::loop(std::istream &in, std::ostream &, playlists_t &, pl_iter_t &tr, pls_iter_t &pl, save_t &)
{
  size_t times;
  in >> times;
  if (tr == (*pl).second.end()) {
    tr = (*pl).second.begin();
  }
  for (size_t i = 0; i < times; ++i) {
    PlayWav((*tr).second.track_, (*tr).second.duration_);
  }
}
void dirko::random(std::istream &in, std::ostream &out, playlists_t &, pl_iter_t &tr, pls_iter_t &pl, save_t &)
{
  if ((*pl).second.size() < 1) {
    return;
  }
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution< size_t > distrib(0, (*pl).second.size() - 1);
  tr = (*pl).second.begin();
  size_t rnd = distrib(gen);
  for (size_t i = 0; i < rnd; ++i) {
    ++tr;
  }
  out << "duration: " << (*tr).second.duration_ << '\n';
  double dur;
  in >> dur;
  if (dur > (*tr).second.duration_) {
    out << "<playing until end of track>\n";
    dur = (*tr).second.duration_;
  }
  PlayWav((*tr).second.track_, dur);
}
void dirko::list(std::istream &, std::ostream &out, playlists_t &, pl_iter_t &, pls_iter_t &pl, save_t &)
{
  for (const std::pair< std::string, Track > &v : (*pl).second) {
    out << v.first << '\n';
  }
}
void dirko::playlist_add(std::istream &in, std::ostream &, playlists_t &db, pl_iter_t &, pls_iter_t &, save_t &)
{
  std::string name;
  in >> name;
  db.add(name, playlist_t(5, .7));
}
void dirko::playlist_remove(std::istream &in, std::ostream &, playlists_t &db, pl_iter_t &, pls_iter_t &, save_t &)
{
  std::string name;
  in >> name;
  db.drop(name);
}
void dirko::playlist_rename(std::istream &in, std::ostream &, playlists_t &db, pl_iter_t &, pls_iter_t &, save_t &)
{
  std::string from, to;
  in >> from >> to;
  db.changeKey(from, to);
}
void dirko::playlist_add_track(std::istream &in, std::ostream &, playlists_t &db, pl_iter_t &, pls_iter_t &, save_t &)
{
  std::string playlist, track;
  in >> playlist >> track;
  db.get(playlist).add(track, db.get("All").get(track));
}
void dirko::playlist_remove_track(std::istream &in, std::ostream &, playlists_t &db, pl_iter_t &, pls_iter_t &,
                                  save_t &)
{
  std::string playlist, track;
  in >> playlist >> track;
  db.get(playlist).drop(track);
}
void dirko::playlist_get(std::istream &in, std::ostream &out, playlists_t &db, pl_iter_t &, pls_iter_t &, save_t &)
{
  std::string name;
  in >> name;
  for (const std::pair< std::string, Track > &v : db.get(name)) {
    out << v.first << '\n';
  }
}
void dirko::playlist_list(std::istream &, std::ostream &out, playlists_t &db, pl_iter_t &, pls_iter_t &, save_t &)
{
  for (const std::pair< std::string, playlist_t > &v : db) {
    out << v.first << '\n';
  }
}
void dirko::playlist_merge(std::istream &in, std::ostream &, playlists_t &db, pl_iter_t &, pls_iter_t &, save_t &)
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
void dirko::playlist_select(std::istream &in, std::ostream &, playlists_t &db, pl_iter_t &tr, pls_iter_t &pl, save_t &)
{
  std::string name;
  in >> name;
  pl = db.getIter(name);
  tr = (*pl).second.begin();
}
void dirko::playlist_diff(std::istream &in, std::ostream &, playlists_t &db, pl_iter_t &, pls_iter_t &, save_t &)
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
