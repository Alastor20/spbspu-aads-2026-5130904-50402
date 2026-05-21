#ifndef ROBIN_HPP
#define ROBIN_HPP

#include <cstddef>
#include <utility>
#include "../common/Vector.hpp"
#include "../common/hasher.hpp"
#include "../common/list.hpp"
namespace dirko
{
  template< class Key, class Value >
  struct RobinNode
  {
    std::pair< Key, Value > val_;
    size_t psl_;
    bool notEmpty_;
    RobinNode();
  };
  template< class Key, class Value, class Hash, class Equal >
  class RTIter;
  template< class Key, class Value, class Hash, class Equal >
  class RTCIter;
  template< class Key, class Value, class Hash = dirko::SipHasher< Key >, class Equal = std::equal_to< Key > >
  class RobinTable
  {
  public:
    explicit RobinTable(size_t buckets, size_t bucket_size);
    RobinTable(std::initializer_list< std::pair< Key, Value > > il);

    void add(Key k, Value v);
    void drop(Key k);
    Value &get(Key k);
    const Value get(Key k) const;
    bool has(Key k) const noexcept;
    void rehash(size_t slots);

    using RTIt = RTIter< Key, Value, Hash, Equal >;
    using RTCIt = RTCIter< Key, Value, Hash, Equal >;
    RTIt begin() noexcept;
    RTIt end() noexcept;
    RTCIt cbegin() const noexcept;
    RTCIt cend() const noexcept;

    void clear() noexcept;
    size_t size() const noexcept;
    bool empty() const noexcept;
    void swap(RobinTable &other) noexcept;

  private:
    Vector< RobinNode< Key, Value > > data_;
    Hash hasher_;
    Equal comparator_;
    size_t buckets_;
    size_t bucket_size_;
    size_t elements_;
    List< std::pair< Key, Value > > overflow_;
    friend class RTIter< Key, Value, Hash, Equal >;
    friend class RTCIter< Key, Value, Hash, Equal >;
  };
  template< class Key, class Value, class Hash, class Equal >
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
    Vector< RobinNode< Key, Value > > *data_;
    size_t id_;
    LIter< std::pair< Key, Value > > overflow_;
    void next();
  };
  template< class Key, class Value, class Hash, class Equal >
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
    Vector< RobinNode< Key, Value > > *data_;
    size_t id_;
    LCIter< std::pair< Key, Value > > overflow_;
    void next();
  };
}

template< class Key, class Value >
dirko::RobinNode< Key, Value >::RobinNode():
  val_(),
  psl_(0),
  notEmpty_(false)
{}

template< class Key, class Value, class Hash, class Equal >
dirko::RobinTable< Key, Value, Hash, Equal >::RobinTable(size_t buckets, size_t bucket_size):
  data_(),
  hasher_(Hash{}),
  comparator_(Equal{}),
  buckets_(buckets),
  bucket_size_(bucket_size),
  elements_(0),
  overflow_()
{
  data_.reserve(bucket_size * buckets);
  for (size_t i = 0; i < buckets * bucket_size; ++i) {
    data_.pushBack(RobinNode< Key, Value >());
  }
}
#endif
