#include "cmds.hpp"
#include <cstddef>
#include <random>
#include <stdexcept>
#include <string>
#include "decoder.hpp"
#include "player.hpp"

using dirko::save_t;

namespace
{
  void playImpl(dirko::pl_iter_t &track, std::istream &in, std::ostream &out)
  {
    out << "<duration: " << (*track).second.duration_ << ">\n";
    double duration;
    in >> duration;
    if (duration > (*track).second.duration_) {
      out << "<playing until end of track>\n";
      duration = (*track).second.duration_;
    }
    PlayWav((*track).second.track_, duration);
  }
}

void dirko::play(std::istream &in, std::ostream &out, pls_t &, pl_iter_t &tr, pls_iter_t &pl, save_t &)
{
  std::string name;
  in >> name;
  tr = (*pl).second.getIter(name);
  playImpl(tr, in, out);
}
void dirko::next(std::istream &in, std::ostream &out, pls_t &, pl_iter_t &tr, pls_iter_t &pl, save_t &)
{
  if (tr == (*pl).second.end()) {
    tr = (*pl).second.begin();
  }
  ++tr;
  if (tr == (*pl).second.end()) {
    tr = (*pl).second.begin();
  }
  playImpl(tr, in, out);
}
void dirko::prev(std::istream &in, std::ostream &out, pls_t &, pl_iter_t &tr, pls_iter_t &pl, save_t &)
{
  if (tr == (*pl).second.begin()) {
    tr = (*pl).second.end();
  }
  --tr;
  playImpl(tr, in, out);
}
void dirko::add(std::istream &in, std::ostream &, pls_t &db, pl_iter_t &, pls_iter_t &, save_t &saver)
{
  std::string path;
  std::string name;
  in >> path >> name;
  WavFile wav;
  Load(path, wav);
  Track track(wav);
  db.get("All").add(name, track);
  saver.add(name, path);
}
void dirko::remove(std::istream &in, std::ostream &, pls_t &db, pl_iter_t &, pls_iter_t &, save_t &saver)
{
  std::string name;
  in >> name;
  if (!db.get("All").has(name)) {
    throw std::invalid_argument("no such track");
  }
  saver.drop(name);
  for (std::pair< std::string, pl_t > &v : db) {
    if (v.second.has(name)) {
      v.second.drop(name);
    }
  }
}
void dirko::rename(std::istream &in, std::ostream &, pls_t &db, pl_iter_t &, pls_iter_t &, save_t &saver)
{
  std::string from, to;
  in >> from >> to;
  if (!db.get("All").has(from)) {
    throw std::invalid_argument("no such track");
  }
  saver.changeKey(from, to);
  for (std::pair< std::string, pl_t > &v : db) {
    if (v.second.has(from)) {
      v.second.changeKey(from, to);
    }
  }
}
void dirko::loop(std::istream &in, std::ostream &, pls_t &, pl_iter_t &tr, pls_iter_t &pl, save_t &)
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
void dirko::random(std::istream &in, std::ostream &out, pls_t &, pl_iter_t &tr, pls_iter_t &pl, save_t &)
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
  playImpl(tr, in, out);
}
void dirko::list(std::istream &, std::ostream &out, pls_t &, pl_iter_t &, pls_iter_t &pl, save_t &)
{
  for (const std::pair< std::string, Track > &v : (*pl).second) {
    out << v.first << '\n';
  }
}
void dirko::pl_add(std::istream &in, std::ostream &, pls_t &db, pl_iter_t &, pls_iter_t &, save_t &)
{
  std::string name;
  in >> name;
  db.add(name, pl_t(5, .7));
}
void dirko::pl_remove(std::istream &in, std::ostream &, pls_t &db, pl_iter_t &, pls_iter_t &, save_t &)
{
  std::string name;
  in >> name;
  db.drop(name);
}
void dirko::pl_rename(std::istream &in, std::ostream &, pls_t &db, pl_iter_t &, pls_iter_t &, save_t &)
{
  std::string from, to;
  in >> from >> to;
  db.changeKey(from, to);
}
void dirko::pl_add_track(std::istream &in, std::ostream &, pls_t &db, pl_iter_t &, pls_iter_t &, save_t &)
{
  std::string playlist, track;
  in >> playlist >> track;
  db.get(playlist).add(track, db.get("All").get(track));
}
void dirko::pl_remove_track(std::istream &in, std::ostream &, pls_t &db, pl_iter_t &, pls_iter_t &, save_t &)
{
  std::string playlist, track;
  in >> playlist >> track;
  db.get(playlist).drop(track);
}
void dirko::pl_get(std::istream &in, std::ostream &out, pls_t &db, pl_iter_t &, pls_iter_t &, save_t &)
{
  std::string name;
  in >> name;
  for (const std::pair< std::string, Track > &v : db.get(name)) {
    out << v.first << '\n';
  }
}
void dirko::pl_list(std::istream &, std::ostream &out, pls_t &db, pl_iter_t &, pls_iter_t &, save_t &)
{
  for (const std::pair< std::string, pl_t > &v : db) {
    out << v.first << '\n';
  }
}
void dirko::pl_merge(std::istream &in, std::ostream &, pls_t &db, pl_iter_t &, pls_iter_t &, save_t &)
{
  std::string list1, list2, listRes;
  in >> list1 >> list2 >> listRes;
  if (db.has(listRes)) {
    throw std::invalid_argument("playlist with this name already exitsts");
  }
  pl_t newList(10, .7);
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
void dirko::pl_select(std::istream &in, std::ostream &, pls_t &db, pl_iter_t &tr, pls_iter_t &pl, save_t &)
{
  std::string name;
  in >> name;
  pl = db.getIter(name);
  tr = (*pl).second.begin();
}
void dirko::pl_diff(std::istream &in, std::ostream &, pls_t &db, pl_iter_t &, pls_iter_t &, save_t &)
{
  std::string list1, list2, listRes;
  in >> list1 >> list2 >> listRes;
  if (db.has(listRes)) {
    throw std::invalid_argument("playlist with this name already exitsts");
  }
  pl_t newList(10, .7);
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
