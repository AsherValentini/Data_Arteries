## Data Pipeline Architecture Comparison

| Feature              | Blocking Queue + Shared Ptr + Reuse Pool       | Lock-Free Ring Buffer + Raw Data + Stack Memory |
|----------------------|--------------------------------------------------|--------------------------------------------------|
| **Queue Type**        | Thread-safe, blocking (mutex + cond_var)        | Lock-free SPSC ring buffer                      |
| **Data Type**         | `std::shared_ptr<T>`                            | Raw `T`, constructed via placement-new          |
| **Memory Allocation** | Reuse Pool (heap, recycled)                     | Preallocated raw storage (`aligned_storage`)     |
| **Thread Model**      | 1P + N C / M + N                                | 1 Producer + 1 Consumer (SPSC)                  |
| **Safety**            | RAII, shared ownership, safe shutdown           | Manual lifetime control                         |
| **Latency Profile**   | Soft real-time, blocks when full/empty          | Hard real-time, zero-blocking                   |
| **Flexibility**       | High — safe for general systems                 | Medium — requires tight control                 |
| **Use Case**          | Strategy queues, logs, general pipeline flows   | Sensor feeds, tick streams, audio, rendering    |

**Choose Blocking Queue When...**
- You need shared ownership across threads
- You want easy, RAII-safe shutdown handling    
- You’re willing to block producers or consumers
- You expect dynamic producer/consumer topologies

**Choose Lock-Free Ring Buffer When...**
- You need **fixed latency**
- You control **exactly one producer and one consumer**
- You’re optimizing for embedded, AR, audio, or HFT pipelines
- You want zero dynamic allocations

## Queue Architecture Rationale

## Queue Selection Checklist

> Use these questions to guide your implementation choice:

- Do I need multiple producers or consumers?
- Do I need blocking behavior to absorb bursts or apply backpressure?
- Is object lifetime shared or does each consumer own its copy?
- Can I tolerate dynamic allocations?
- Do I need to guarantee no data loss (vs allow dropping)?
- Is the queue accessed from soft real-time or hard real-time code?
- Do I care more about safety or throughput?


## When to Use a Producer-Consumer Queue

These design cues imply you need a queue-style memory structure:

- Data is produced asynchronously from one thread and consumed in another
- You must **decouple producers from consumers**
- The consumer must **see all data** in order (FIFO)
- The system is **streaming**, not request/response
- The producer should not be tightly coupled to downstream logic
- You’re building a **pipeline**, not a batch processor
- You want to enable **modular dispatching** or **fan-out (e.g. logs, analytics)**

If you're saying:
> “A component generates ticks, frames, commands, or logs that must be handled *elsewhere* asynchronously”  
→ You're in **producer-consumer territory**.


