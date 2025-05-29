# Overview of Real-Time Data Pipeline Structures

| **Type**                   | **Mechanism**        | **Memory**            | **Concurrency**           | **Blocking** | **Used For**                       |
|----------------------------|----------------------|------------------------|----------------------------|--------------|------------------------------------|
| **Atomic Queue (SPSC)**    | Lock-free            | Dynamic                | 1 Producer + 1 Consumer    |    No         | Real-time, fastest path            |
| **Blocking Queue (SPSC)**  | Mutex + CondVar      | Dynamic or pooled      | 1 Producer + 1 Consumer    |    Yes        | General use, safe delivery         |
| **Atomic Ring Buffer (SPSC)** | Lock-free         | Fixed / Static         | 1 Producer + 1 Consumer    |    No         | High-frequency, low-latency streams|
| **Blocking Ring Buffer**   | Mutex + CondVar      | Fixed / Static         | 1P + N Consumers or M + N  |    Yes        | Real-time with correctness         |
