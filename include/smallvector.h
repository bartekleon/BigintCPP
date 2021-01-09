#pragma once

#include <stdexcept>

namespace BigMath {

template<size_t T_size>
[[nodiscard]] constexpr size_t get_size_of_n(const size_t count) {
  constexpr bool overflow_is_possible = T_size > 1;

  if constexpr (overflow_is_possible) {
    constexpr size_t max_possible = static_cast<size_t>(-1) / T_size;
    if (count > max_possible) {
      throw std::out_of_range("bad array new length");// multiply overflow
    }
  }

  return count * T_size;
}


// FUNCTION TEMPLATE _Allocate_for_op_delete
template<class T>
T *allocate_for_delete(size_t Count) {
  // allocates space for _Count copies of _Ty, which will be freed with scalar delete
  if (Count == 0) {
    return nullptr;
  }

  const size_t Bytes = get_size_of_n<sizeof(T)>(Count);
#ifdef __cpp_aligned_new
  constexpr bool Extended_alignment = alignof(T) > __STDCPP_DEFAULT_NEW_ALIGNMENT__;
  if constexpr (Extended_alignment) {
    return static_cast<T *>(::operator new (Bytes, std::align_val_t{ alignof(T) }));
  } else
#endif// __cpp_aligned_new
  {
    return static_cast<T *>(::operator new(Bytes));
  }
}

template<class T>
class SmallVector {// store array with various indexing options
public:
  SmallVector() = default;// construct empty ValArr
  explicit SmallVector(size_t count) {
    _size = 0;
    _capacity = count;

    grow(count);
  }
  SmallVector(const T &val, size_t count) {
    _size = count;
    _capacity = count;

    grow(count, &val);
  }
  SmallVector(const T *ptr, size_t count) {
    _size = count;
    _capacity = count;

    grow(count, ptr, 1);
  }
  SmallVector(const SmallVector &right) {
    _size = right._size;
    _capacity = right._size;

    grow(right._size, right._ptr, 1);
  }
  SmallVector(SmallVector &&right) noexcept {
    init();
    assign_rv(std::move(right));
  }

  SmallVector &operator=(SmallVector &&right) noexcept {
    assign_rv(std::move(right));
    return *this;
  }

  void assign_rv(SmallVector &&right) {
    if (this != std::addressof(right)) {// clear this and steal from right
      deallocate();
      _ptr = right._ptr;
      _size = right._size;
      _capacity = right._capacity;
      right.init();
    }
  }

  void swap(SmallVector &right) noexcept {
    if (this != std::addressof(right)) {
      std::swap(_ptr, right._ptr);
      std::swap(_size, right._size);
    }
  }

  ~SmallVector() noexcept {
    deallocate();
  }

  SmallVector &operator=(const SmallVector &right) {
    if (this != std::addressof(right)) {
      _size = right._size;
      _capacity = right._size;

      grow(right._size, right._ptr, 1);
    }

    return *this;
  }

  void resize(size_t newsize) {// determine new length, filling with _Ty() elements
    resize(newsize, 0);
  }

  void resize(size_t newsize, T val) {// determine new length, filling with _Val elements
    if (newsize > _size) {
      if (newsize > _capacity) {
        T *_temp = allocate_for_delete<T>(newsize);

        for (size_t Idx = 0; Idx < _size; ++Idx) {
          _temp[Idx] = _ptr[Idx];
        }
        _ptr = nullptr;
        _ptr = std::move(_temp);

        _capacity = newsize;
      }
      for (size_t Idx = _size; Idx < newsize; ++Idx) {
        _ptr[Idx] = val;
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
      reserve(_capacity == 0 ? 8 : _capacity * 2);
    }
    _ptr[_size++] = val;
  }

  void pop_back() {
    if (_size > 0) {
      _size--;
    }
  }

  void reserve(size_t capacity) {
    if (_capacity < capacity) {
      _capacity = capacity;
      T *_temp = allocate_for_delete<T>(capacity);

      for (size_t Idx = 0; Idx < _size; ++Idx) {
        _temp[Idx] = _ptr[Idx];
      }
      _ptr = _temp;
    }
  }

  void clear() {
    _size = 0;
  }

  [[nodiscard]] bool empty() const {
    return _size == 0;
  }

  [[nodiscard]] size_t capacity() const {
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
  void grow(size_t newsize) {// allocate space for _Count elements and fill with default values
    _ptr = allocate_for_delete<T>(newsize);
    for (size_t Idx = 0; Idx < newsize; ++Idx) {
      _ptr[Idx] = 0;
    }
  }

  void grow(size_t newsize, const T *ptr, size_t inc = 0) {
    _ptr = allocate_for_delete<T>(newsize);
    for (size_t Idx = 0; Idx < newsize; ++Idx, ptr += inc) {
      _ptr[Idx] = *ptr;
    }
  }

  void init() noexcept {
    _size = 0;
    _capacity = 0;
    _ptr = nullptr;
  }

  void deallocate() noexcept {
    delete _ptr;

    init();
  }

  T *_ptr = nullptr;// current storage reserved for array
  size_t _size = 0;// current length of sequence
  size_t _capacity = 0;// current capacity of sequence
};

}// namespace BigMath
