#ifndef ROBIN_HPP
#define ROBIN_HPP

#include <cstddef>
#include <memory>
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
    void swap(RobinNode &other);
  };
  template< class Key, class Value, class Hash, class Equal >
  class RTIter;
  template< class Key, class Value, class Hash, class Equal >
  class RTCIter;
  template< class Key, class Value, class Hash = dirko::SipHasher< Key >, class Equal = std::equal_to< Key > >
  class RobinTable
  {
  public:
    explicit RobinTable(size_t slots, float load);
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
    RTCIt begin() const noexcept;
    RTCIt end() const noexcept;
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
    size_t slots_;
    size_t elements_;
    float max_load_;
    friend class RTIter< Key, Value, Hash, Equal >;
    friend class RTCIter< Key, Value, Hash, Equal >;
  };
  template< class Key, class Value, class Hash, class Equal >
  class RTIter
  {
  public:
    RTIter();
    RTIter(Vector< RobinNode< Key, Value > > *, size_t, LIter< std::pair< Key, Value > >);

    RTIter &operator++();
    RTIter operator++(int);
    bool operator==(const RTIter &other) const noexcept;
    bool operator!=(const RTIter &other) const noexcept;
    std::pair< Key, Value > &operator*() noexcept;

  private:
    Vector< RobinNode< Key, Value > > *data_;
    size_t id_;
    void next();
  };
  template< class Key, class Value, class Hash, class Equal >
  class RTCIter
  {
  public:
    RTCIter();
    RTCIter(Vector< RobinNode< Key, Value > > *, size_t, LCIter< std::pair< Key, Value > >);

    RTCIter &operator++();
    RTCIter operator++(int);
    bool operator==(const RTCIter &other) const noexcept;
    bool operator!=(const RTCIter &other) const noexcept;
    std::pair< Key, Value > &operator*() noexcept;

  private:
    Vector< RobinNode< Key, Value > > *data_;
    size_t id_;
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
dirko::RobinTable< Key, Value, Hash, Equal >::RobinTable(size_t slots, float load):
  data_(),
  hasher_(Hash{}),
  comparator_(Equal{}),
  slots_(slots),
  elements_(0),
  max_load_(load)
{
  data_.reserve(slots);
  for (size_t i = 0; i < slots; ++i) {
    data_.pushBack(RobinNode< Key, Value >());
  }
}
template< class Key, class Value, class Hash, class Equal >
size_t dirko::RobinTable< Key, Value, Hash, Equal >::size() const noexcept
{
  return elements_;
}
template< class Key, class Value, class Hash, class Equal >
bool dirko::RobinTable< Key, Value, Hash, Equal >::empty() const noexcept
{
  return !elements_;
}
template< class Key, class Value, class Hash, class Equal >
void dirko::RobinTable< Key, Value, Hash, Equal >::swap(RobinTable &other) noexcept
{
  data_.swap(other.data_);
  std::swap(hasher_, other.hasher_);
  std::swap(comparator_, other.comparator_);
  std::swap(slots_, other.slots_);
  std::swap(elements_, other.elements_);
  std::swap(max_load_, other.max_load_);
}
template< class Key, class Value, class Hash, class Equal >
void dirko::RobinTable< Key, Value, Hash, Equal >::clear() noexcept
{
  data_.clear();
  elements_ = 0;
}

template< class Key, class Value, class Hash, class Equal >
dirko::RobinTable< Key, Value, Hash, Equal >::RobinTable(std::initializer_list< std::pair< Key, Value > > il):
  RobinTable(il.size(), 5)
{
  data_.reserve(il.size());
  for (const std::pair< Key, Value > &v : il) {
    add(v.first, v.second);
  }
}

template< class Key, class Value, class Hash, class Equal >
dirko::RTIter< Key, Value, Hash, Equal > dirko::RobinTable< Key, Value, Hash, Equal >::begin() noexcept
{
  return RTIter< Key, Value, Hash, Equal >(std::addressof(data_), 0);
}
template< class Key, class Value, class Hash, class Equal >
dirko::RTIter< Key, Value, Hash, Equal > dirko::RobinTable< Key, Value, Hash, Equal >::end() noexcept
{
  return RTIter< Key, Value, Hash, Equal >(std::addressof(data_), slots_);
}
template< class Key, class Value, class Hash, class Equal >
dirko::RTCIter< Key, Value, Hash, Equal > dirko::RobinTable< Key, Value, Hash, Equal >::begin() const noexcept
{
  return RTCIter< Key, Value, Hash, Equal >(std::addressof(data_), 0);
}
template< class Key, class Value, class Hash, class Equal >
dirko::RTCIter< Key, Value, Hash, Equal > dirko::RobinTable< Key, Value, Hash, Equal >::end() const noexcept
{
  return RTCIter< Key, Value, Hash, Equal >(std::addressof(data_), slots_);
}
template< class Key, class Value, class Hash, class Equal >
dirko::RTCIter< Key, Value, Hash, Equal > dirko::RobinTable< Key, Value, Hash, Equal >::cbegin() const noexcept
{
  return RTCIter< Key, Value, Hash, Equal >(std::addressof(data_), 0);
}
template< class Key, class Value, class Hash, class Equal >
dirko::RTCIter< Key, Value, Hash, Equal > dirko::RobinTable< Key, Value, Hash, Equal >::cend() const noexcept
{
  return RTCIter< Key, Value, Hash, Equal >(std::addressof(data_), slots_);
}

template< class Key, class Value, class Hash, class Equal >
void dirko::RobinTable< Key, Value, Hash, Equal >::rehash(size_t slots)
{
  RobinTable< Key, Value, Hash, Equal > cpy(slots, max_load_);
  for (const std::pair< Key, Value > &v : *this) {
    cpy.add(v.first, v.second);
  }
  swap(cpy);
}

template< class Key, class Value, class Hash, class Equal >
void dirko::RobinTable< Key, Value, Hash, Equal >::add(Key k, Value v)
{
  if (has(k)) {
    get(k) = v;
    return;
  }
  if (elements_ >= slots_ * max_load_) {
    rehash(empty() ? 16 : slots_ * 2);
  }
  size_t id = hasher_(k) % slots_;
  size_t psl = 0;
  while (true) {
    if (!data_[id].notEmpty_) {
      data_[id].val_ = {k, v};
      data_[id].psl_ = psl;
      data_[id].notEmpty_ = true;
      ++elements_;
      return;
    }
    if (data_[id].psl_ < psl) {
      std::swap(k, data_[id].val_.first);
      std::swap(v, data_[id].val_.second);
      std::swap(psl, data_[id].psl_);
    }
    id = (id + 1) % slots_;
    ++psl;
  }
}
#endif
