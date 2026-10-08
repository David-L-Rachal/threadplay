# ThreadPlay

A small historical C++ experiment exploring native Windows multithreading, shared state, and synchronization.

This project comes from my early Visual C++ work and is preserved as an example of lower-level systems programming I was experimenting with alongside production C++ development.

## What It Demonstrates

The sample creates multiple native worker threads using `_beginthreadex` and coordinates access to shared state using Win32 synchronization primitives.

Concepts demonstrated include:

- Native Windows thread creation
- Concurrent execution
- Shared mutable state
- `CRITICAL_SECTION`
- `EnterCriticalSection` / `LeaveCriticalSection`
- Thread termination signaling
- Waiting for worker threads
- Windows handle management and cleanup

## Why I'm Keeping It

This isn't intended to be a modern threading reference implementation.

It's a small snapshot of the type of lower-level Windows and C++ programming I was exploring early in my software engineering career.

Modern C++ provides significantly better abstractions for many of these problems, including `std::thread`, `std::mutex`, atomics, RAII-based locking, and higher-level concurrency libraries.

The original implementation is preserved primarily for historical and portfolio purposes.
