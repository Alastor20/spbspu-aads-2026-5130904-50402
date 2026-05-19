#ifndef ROBIN_HPP
#define ROBIN_HPP

#include <cstddef>
#include <utility>
#include "../common/Vector.hpp"
#include "../common/hasher.hpp"
#include "../common/list.hpp"
namespace dirko
{
  template < class Key, class Value >
  struct RobinNode
  {
    std::pair< Key, Value > val;
    size_t psl;
    bool notEmpty;
  };
  template < class Key, class Value, class Hash, class Equal >
  class RTIter;
  template < class Key, class Value, class Hash, class Equal >
  class RTCIter;
  template < class Key, class Value, class Hash = dirko::SipHasher< Key >, class Equal = std::equal_to< Key > >
  class RobinTable
  {
  public:
    explicit RobinTable(size_t buckets, size_t buckets_cap);
    RobinTable(std::initializer_list< std::pair< Key, Value > > il);

    void add(Key k, Value v);
    void drop(Key k);
    Value &get(Key k);
    const Value get(Key k) const;
    bool has(Key k) const noexcept;
    void rehash(size_t slots);

    using HTIt = RTIter< Key, Value, Hash, Equal >;
    using HTCIt = RTCIter< Key, Value, Hash, Equal >;
    HTIt begin() noexcept;
    HTIt end() noexcept;
    HTCIt cbegin() const noexcept;
    HTCIt cend() const noexcept;

    void clear() noexcept;
    size_t size() const noexcept;
    bool empty() const noexcept;
    void swap(RobinTable &other) noexcept;

  private:
    Vector< RobinNode< Key, Value > > data_;
    Hash hasher_;
    Equal comparator_;
    size_t buckets_;
    size_t buckets_cap_;
    size_t elements_;
    List< std::pair< Key, Value > > overflow_;
    friend class RTIter< Key, Value, Hash, Equal >;
    friend class RTCIter< Key, Value, Hash, Equal >;
  };
  template < class Key, class Value, class Hash, class Equal >
  class RTIter
  {
  public:
    RTIter();
    RTIter(Vector< List< std::pair< Key, Value > > > *, size_t);

    RTIter &operator++();
    RTIter operator++(int);
    bool operator==(const RTIter &other) const noexcept;
    bool operator!=(const RTIter &other) const noexcept;
    std::pair< Key, Value > &operator*() noexcept;

  private:
    Vector< List< std::pair< Key, Value > > > *data_;
    size_t slot_;
    LIter< std::pair< Key, Value > > lit_, lend_;
    void next();
  };
  template < class Key, class Value, class Hash, class Equal >
  class RTCIter
  {
  public:
    RTCIter();
    RTCIter(Vector< List< std::pair< Key, Value > > > *, size_t);

    RTCIter &operator++();
    RTCIter operator++(int);
    bool operator==(const RTCIter &other) const noexcept;
    bool operator!=(const RTCIter &other) const noexcept;
    std::pair< Key, Value > &operator*() noexcept;

  private:
    Vector< List< std::pair< Key, Value > > > *data_;
    size_t slot_;
    LCIter< std::pair< Key, Value > > lit_, lend_;
    void next();
  };
}

#endif
