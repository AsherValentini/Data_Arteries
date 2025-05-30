#pragma once

#include <atomic>
#include <cstddef>
#include <type_traits>
#include <optional>
#include <new> // for placement new

template<typename T, std::size_t Capacity>
class RawRingBuffer {
  static_assert(Capacity > 1, "Ring buffer capacity must be greater than one.");

public:
  RawRingBuffer() : head_(0), tail_(0) {}

  ~RawRingBuffer() {
    // Explicitly destroy all constructed elements
    while (pop()) {}
  }

  bool push(const T& item) {
    const std::size_t head = head_.load(std::memory_order_relaxed);
    const std::size_t next = increment(head);

    if (next == tail_.load(std::memory_order_acquire)) {
      return false; // buffer is full
    }

    void* slot = static_cast<void*>(&buffer_[head]);
    new (slot) T(item); // placement-new
    head_.store(next, std::memory_order_release);
    return true;
  }

  std::optional<T> pop() {
    const std::size_t tail = tail_.load(std::memory_order_relaxed);
    if (tail == head_.load(std::memory_order_acquire)) {
      return std::nullopt; // buffer is empty
    }

    T* obj_ptr = reinterpret_cast<T*>(&buffer_[tail]);
    T result = std::move(*obj_ptr); // move/copy from slot
    obj_ptr->~T(); // manual destructor call

    tail_.store(increment(tail), std::memory_order_release);
    return result;
  }

private:
  std::size_t increment(std::size_t index) const {
    return (index + 1) % Capacity;
  }

  // Raw memory storage
  typename std::aligned_storage<sizeof(T), alignof(T)>::type buffer_[Capacity];

  std::atomic<std::size_t> head_;
  std::atomic<std::size_t> tail_;
};
