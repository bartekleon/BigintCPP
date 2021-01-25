#pragma once

#include <stdexcept>

namespace BigMath {

template<size_t T_size>
[[nodiscard]] constexpr size_t get_size_of_n(const size_t count) {
  constexpr bool overflow_is_possible{ T_size > 1 };

  if constexpr (overflow_is_possible) {
    constexpr size_t max_possible{ static_cast<size_t>(-1) / T_size };
    if (count > max_possible) {
      throw std::out_of_range("bad array new length");// multiply overflow
    }
  }

  return count * T_size;
}


// FUNCTION TEMPLATE allocate_for_delete
template<class T>
T *allocate_for_delete(size_t count) {
  // allocates space for count copies of T, which will be freed with scalar delete
  if (count == 0) {
    return nullptr;
  }

  const size_t bytes{ get_size_of_n<sizeof(T)>(count) };

  return static_cast<T *>(::operator new(bytes));
}

template<class T>
class SmallVector {// store array with various indexing options
public:
  SmallVector() = default;// construct empty ValArr
  explicit SmallVector(size_t count) : _capacity(count) {
    grow(count);
  }
  SmallVector(const T &val, size_t count) : _size(count), _capacity(count) {
    grow(count, &val);
  }
  SmallVector(const T *ptr, size_t count) : _size(count), _capacity(count) {
    grow(count, ptr, 1);
  }
  SmallVector(const SmallVector &right) : _size(right._size), _capacity(right._size) {
    grow(right._size, right._ptr, 1);
  }
  SmallVector(SmallVector &&right) noexcept {
    assign_rv(std::move(right));
  }

  SmallVector &operator=(SmallVector &&right) noexcept {
    assign_rv(std::move(right));
    return *this;
  }

  void assign_rv(SmallVector &&right) noexcept {
    if (this != std::addressof(right)) {// clear this and steal from right
      delete _ptr;
      _ptr = right._ptr;
      _size = right._size;
      _capacity = right._capacity;
      right._size = 0;
      right._capacity = 0;
      right._ptr = nullptr;
    }
  }

  void swap(SmallVector &right) noexcept {
    if (this != std::addressof(right)) {
      std::swap(_ptr, right._ptr);
      std::swap(_size, right._size);
    }
  }

  ~SmallVector() noexcept {
    delete _ptr;
  }

  SmallVector &operator=(const SmallVector &right) {
    if (this != std::addressof(right)) {
      _size = right._size;
      _capacity = right._size;

      grow(right._capacity, right._ptr, 1);
    }

    return *this;
  }

  void copy(const SmallVector &right) {
    if (this != std::addressof(right)) {
      _size = right._size;
      if (right._size >= _capacity) {
        _capacity = right._size;
        grow(right._capacity, right._ptr, 1);
      } else {
        for (size_t idx{ 0 }; idx < _size; ++idx) {
          _ptr[idx] = right._ptr[idx];
        }
      }
    }
  }

  void resize(const size_t newsize) {// determine new length, filling with _Ty() elements
    resize(newsize, 0);
  }

  void resize(const size_t newsize, T val) {// determine new length, filling with _Val elements
    if (newsize > _size) {
      if (newsize > _capacity) {
        T *_temp{ allocate_for_delete<T>(newsize) };

        for (size_t idx{ 0 }; idx < _size; ++idx) {
          _temp[idx] = _ptr[idx];
        }
        delete _ptr;
        _ptr = std::move(_temp);

        _capacity = newsize;
      }
      for (size_t idx{ _size }; idx < newsize; ++idx) {
        _ptr[idx] = val;
      }
      _size = newsize;
    }
  }

  [[nodiscard]] size_t size() const noexcept /* strengthened */ {
    return _size;
  }

  [[nodiscard]] const T &operator[](size_t off) const noexcept /* strengthened */ {
    return _ptr[off];
  }

  [[nodiscard]] T &operator[](size_t off) noexcept /* strengthened */ {
    return _ptr[off];
  }

  void push_back(T val) {
    if (_size == _capacity) {
      reserve(_capacity <= 4 ? 8 : _capacity * 2);
    }
    _ptr[_size++] = val;
  }

  void pop_back() noexcept {
    if (_size > 0) {
      _size--;
    }
  }

  void reserve(size_t capacity) {
    if (_capacity < capacity) {
      _capacity = capacity;
      T *_temp{ allocate_for_delete<T>(capacity) };

      for (size_t idx{ 0 }; idx < _size; ++idx) {
        _temp[idx] = _ptr[idx];
      }
      delete _ptr;
      _ptr = _temp;
    }
  }

  void clear() noexcept {
    _size = 0;
  }

  [[nodiscard]] bool empty() const noexcept {
    return _size == 0;
  }

  [[nodiscard]] size_t capacity() const noexcept {
    return _capacity;
  }

  [[nodiscard]] T back() const {
    return _ptr[_size - 1];
  }

  [[nodiscard]] T *begin() const {
    return &_ptr[0];
  }

  [[nodiscard]] T *end() const {
    return &_ptr[0] + _size;
  }

private:
  void grow(const size_t new_capacity) {// allocate space for _Count elements and fill with default values
    delete _ptr;
    _ptr = allocate_for_delete<T>(new_capacity);
    for (size_t idx{ 0 }; idx < new_capacity; ++idx) {
      _ptr[idx] = 0;
    }
  }

  void grow(const size_t new_capacity, const T *ptr, const size_t inc) {
    delete _ptr;
    _ptr = allocate_for_delete<T>(new_capacity);
    for (size_t idx = 0; idx < new_capacity; ++idx, ptr += inc) {
      _ptr[idx] = *ptr;
    }
  }

  T *_ptr{ nullptr };// current storage reserved for array
  size_t _size{ 0 };// current length of sequence
  size_t _capacity{ 0 };// current capacity of sequence
};

}// namespace BigMath
