#include <iostream>
#include <sstream>
#include <boost/test/unit_test.hpp>
#include "cmds.hpp"
#include "player.hpp"

using namespace dirko;

BOOST_AUTO_TEST_SUITE(PlayerTests)
BOOST_AUTO_TEST_CASE(test_add)
{
  save_t saver;
  pls_t db;
  db.add("All", pl_t{});
  pls_iter_t pl = db.begin();
  pl_iter_t tr = db.get("All").begin();
  std::istringstream in("./dirko.nikita/F0/tr1.wav t1");
  add(in, std::cout, db, tr, pl, saver);
  BOOST_CHECK(db.get("All").has("t1"));
  BOOST_CHECK(saver.has("t1"));
}
BOOST_AUTO_TEST_CASE(test_remove)
{
  save_t saver;
  pls_t db;
  db.add("All", pl_t{});
  pls_iter_t pl = db.begin();
  pl_iter_t tr = db.get("All").begin();
  std::istringstream in("./dirko.nikita/F0/tr1.wav t1");
  add(in, std::cout, db, tr, pl, saver);
  in.str("t1");
  in.clear();
  remove(in, std::cout, db, tr, pl, saver);
  BOOST_CHECK(!db.get("All").has("t1"));
  BOOST_CHECK(!saver.has("t1"));
}
BOOST_AUTO_TEST_CASE(test_rename)
{
  save_t saver;
  pls_t db;
  db.add("All", pl_t{});
  pls_iter_t pl = db.begin();
  pl_iter_t tr = db.get("All").begin();
  std::istringstream in("./dirko.nikita/F0/tr1.wav t1 t1 rename");
  add(in, std::cout, db, tr, pl, saver);
  rename(in, std::cout, db, tr, pl, saver);
  BOOST_CHECK(!db.get("All").has("t1"));
  BOOST_CHECK(db.get("All").has("rename"));
}
BOOST_AUTO_TEST_CASE(test_play)
{
  save_t saver;
  pls_t db;
  db.add("All", pl_t{});
  pls_iter_t pl = db.begin();
  pl_iter_t tr = db.get("All").begin();
  std::ostringstream out("");
  std::istringstream in("./dirko.nikita/F0/tr2.wav t1 t1 1 t1 200");
  add(in, std::cout, db, tr, pl, saver);
  play(in, out, db, tr, pl, saver);
  BOOST_CHECK(out.str() == "<duration: 6.24327>\n");
  out.str("");
  play(in, out, db, tr, pl, saver);
  BOOST_CHECK(out.str() == "<duration: 6.24327>\n<playing until end of track>\n");
}
BOOST_AUTO_TEST_CASE(test_next)
{
  save_t saver;
  pls_t db;
  db.add("All", pl_t{});
  pls_iter_t pl = db.begin();
  pl_iter_t tr = db.get("All").begin();
  std::ostringstream out("");
  std::istringstream in("./dirko.nikita/F0/tr1.wav t1 ./dirko.nikita/F0/tr2.wav t2 1");
  add(in, std::cout, db, tr, pl, saver);
  add(in, std::cout, db, tr, pl, saver);
  next(in, out, db, tr, pl, saver);
  BOOST_CHECK(tr == ++(db.get("All").begin()));
}
BOOST_AUTO_TEST_CASE(test_prev)
{
  save_t saver;
  pls_t db;
  db.add("All", pl_t{});
  pls_iter_t pl = db.begin();
  pl_iter_t tr = db.get("All").begin();
  std::ostringstream out("");
  std::istringstream in("./dirko.nikita/F0/tr1.wav t1 ./dirko.nikita/F0/tr2.wav t2 1");
  add(in, std::cout, db, tr, pl, saver);
  add(in, std::cout, db, tr, pl, saver);
  prev(in, out, db, tr, pl, saver);
  BOOST_CHECK(tr == --(db.get("All").end()));
}
BOOST_AUTO_TEST_CASE(test_list)
{
  save_t saver;
  pls_t db;
  db.add("All", pl_t{});
  pls_iter_t pl = db.begin();
  pl_iter_t tr = db.get("All").begin();
  std::ostringstream out("");
  std::istringstream in("./dirko.nikita/F0/tr1.wav t1 ./dirko.nikita/F0/tr2.wav t2");
  add(in, std::cout, db, tr, pl, saver);
  add(in, std::cout, db, tr, pl, saver);
  list(in, out, db, tr, pl, saver);
  BOOST_CHECK(out.str() == "t1\nt2\n");
}
BOOST_AUTO_TEST_CASE(test_pl_add_remove_and_list)
{
  save_t saver;
  pls_t db;
  db.add("All", pl_t{});
  pls_iter_t pl = db.begin();
  pl_iter_t tr = db.get("All").begin();
  std::ostringstream out("");
  std::istringstream in("playlist playlist");
  pl_add(in, std::cout, db, tr, pl, saver);
  pl_list(in, out, db, tr, pl, saver);
  BOOST_CHECK(out.str() == "All\nplaylist\n");
  out.str("");
  pl_remove(in, std::cout, db, tr, pl, saver);
  pl_list(in, out, db, tr, pl, saver);
  BOOST_CHECK(out.str() == "All\n");
}
BOOST_AUTO_TEST_CASE(test_pl_rename)
{
  save_t saver;
  pls_t db;
  db.add("All", pl_t{});
  pls_iter_t pl = db.begin();
  pl_iter_t tr = db.get("All").begin();
  std::ostringstream out("");
  std::istringstream in("playlist playlist p");
  pl_add(in, std::cout, db, tr, pl, saver);
  pl_list(in, out, db, tr, pl, saver);
  BOOST_CHECK(out.str() == "All\nplaylist\n");
  out.str("");
  pl_rename(in, std::cout, db, tr, pl, saver);
  pl_list(in, out, db, tr, pl, saver);
  BOOST_CHECK(out.str() == "p\nAll\n");
}
BOOST_AUTO_TEST_CASE(test_pl_add_remove_and_get_track)
{
  save_t saver;
  pls_t db;
  db.add("All", pl_t{});
  pls_iter_t pl = db.begin();
  pl_iter_t tr = db.get("All").begin();
  std::ostringstream out("");
  std::istringstream in("./dirko.nikita/F0/tr1.wav t1 p p t1 p p t1 p");
  add(in, std::cout, db, tr, pl, saver);
  pl_add(in, std::cout, db, tr, pl, saver);
  pl_add_track(in, std::cout, db, tr, pl, saver);
  pl_get(in, out, db, tr, pl, saver);
  BOOST_CHECK(out.str() == "t1\n");
  out.str("");
  pl_remove_track(in, std::cout, db, tr, pl, saver);
  pl_get(in, out, db, tr, pl, saver);
  BOOST_CHECK(out.str() == "");
}
BOOST_AUTO_TEST_CASE(test_pl_select)
{
  save_t saver;
  pls_t db;
  db.add("All", pl_t{});
  pls_iter_t pl = db.begin();
  pl_iter_t tr = db.get("All").begin();
  std::ostringstream out("");
  std::istringstream in("./dirko.nikita/F0/tr1.wav t1 ./dirko.nikita/F0/tr2.wav t2 p p t1 p");
  add(in, std::cout, db, tr, pl, saver);
  add(in, std::cout, db, tr, pl, saver);
  pl_add(in, std::cout, db, tr, pl, saver);
  pl_add_track(in, std::cout, db, tr, pl, saver);
  pl_select(in, std::cout, db, tr, pl, saver);
  list(in, out, db, tr, pl, saver);
  BOOST_CHECK(out.str() == "t1\n");
}
BOOST_AUTO_TEST_CASE(test_pl_merge)
{
  save_t saver;
  pls_t db;
  db.add("All", pl_t{});
  pls_iter_t pl = db.begin();
  pl_iter_t tr = db.get("All").begin();
  std::ostringstream out("");
  std::istringstream in("./dirko.nikita/F0/tr1.wav t1 ./dirko.nikita/F0/tr2.wav t2 ./dirko.nikita/F0/tr3.wav t3 p1 p2 "
                        "p1 t1 p1 t2 p2 t2 p2 t3 p1 p2 p3 p3");
  add(in, std::cout, db, tr, pl, saver);
  add(in, std::cout, db, tr, pl, saver);
  add(in, std::cout, db, tr, pl, saver);
  pl_add(in, std::cout, db, tr, pl, saver);
  pl_add(in, std::cout, db, tr, pl, saver);
  pl_add_track(in, std::cout, db, tr, pl, saver);
  pl_add_track(in, std::cout, db, tr, pl, saver);
  pl_add_track(in, std::cout, db, tr, pl, saver);
  pl_add_track(in, std::cout, db, tr, pl, saver);
  pl_merge(in, std::cout, db, tr, pl, saver);
  pl_get(in, out, db, tr, pl, saver);
  BOOST_CHECK(db.has("p3"));
  BOOST_CHECK(out.str() == "t2\nt3\nt1\n");
}
BOOST_AUTO_TEST_CASE(test_pl_diff)
{
  save_t saver;
  pls_t db;
  db.add("All", pl_t{});
  pls_iter_t pl = db.begin();
  pl_iter_t tr = db.get("All").begin();
  std::ostringstream out("");
  std::istringstream in("./dirko.nikita/F0/tr1.wav t1 ./dirko.nikita/F0/tr2.wav t2 ./dirko.nikita/F0/tr3.wav t3 p1 p2 "
                        "p1 t1 p1 t2 p2 t2 p2 t3 p1 p2 p3 p3");
  add(in, std::cout, db, tr, pl, saver);
  add(in, std::cout, db, tr, pl, saver);
  add(in, std::cout, db, tr, pl, saver);
  pl_add(in, std::cout, db, tr, pl, saver);
  pl_add(in, std::cout, db, tr, pl, saver);
  pl_add_track(in, std::cout, db, tr, pl, saver);
  pl_add_track(in, std::cout, db, tr, pl, saver);
  pl_add_track(in, std::cout, db, tr, pl, saver);
  pl_add_track(in, std::cout, db, tr, pl, saver);
  pl_diff(in, std::cout, db, tr, pl, saver);
  pl_get(in, out, db, tr, pl, saver);
  BOOST_CHECK(db.has("p3"));
  BOOST_CHECK(out.str() == "t3\nt1\n");
}
BOOST_AUTO_TEST_SUITE_END()
