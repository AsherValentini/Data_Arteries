#pragma once

#include <atomic>
#include <cstdint>
#include <array>

template<typename T, std::size_t Capacity>
class AtomicRingBuffer{
	static_assert(Capacity>1, "Ring buffer must be greater than one");

public:
	AtomicRingBuffer()
		: head_(0), tail_(0) {}

	// push returns false if buffer is full
	bool push(std::shared_ptr<T> item) {

		const std::size_t head = head_.load(std::memory_order_relaxed);
		const std::size_t next = increment(head);

		if(next == tail_.load(std::memory_order_aquire)) {
			return false;
		}

		buffer_head[head] = std::move(item);

		head_.store(next, std::memory_order_release);
		return true;
	}

	std::optional<std::shared_ptr<T>> pop() {
		const size_t tail = tail_.load(std::memory_order_relaxed);
		if (tail==head_.load(memory_order_aquire) {
			return std::nullopt;
		}

		std::shared_ptr<T> item = std::move(buffer_[tail]);
		tail_.store(increment(tail), std::memory_order_release);
		return item;
	}


private:
	std::size_t increment(std::size_t index) const {
		return (index+1)%Capacity;
	}

	std::array<std::shared_ptr<T>, Capacity> buffer_;
	std::atomic<std::size_t> head_;
	std::atomic<std::size_t> tail_;

};