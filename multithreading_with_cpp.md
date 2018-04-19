#  Multithreading in c++

**author**: Rainer Grimm

## The memory model

## Multithreading

`<thread>`
threads use function to execute
wait for end `join()`, else in deamon thread `detach()`. If not one of this in thread destructor will call exception `std::terminate`, thread is called **joinable**.

arguments gets by lambda function by copy or reference. If by reference and detach may get undefined behavior

```cpp
std::string s("str")
std::thread t([&] {std::cout << s << std::endl});
t.detach();
```

### Methods

```cpp
std::this_thread::sleep_for
std::this_thread::sleep_until
t.get_id()
t.joinable()
```

`std::cout` belongs to main thread it can cause errors
`volitile` **is not multithreading semantic**
`std::this_thread::sleep_for(std::chrono)`

main thread terminates all child threads (joined and detached, and joinable will give exception)

## Shared data
- Data race - at least 2 threads accessing variable and 1 is a write

`mutex::try_lock` - i want to access critical region, but if it is lock elswere i will do anything else
`std::shared_timed_mutex` c++14, `shared_lock` for read operations and `lock_guard` for write. Need `<shared_mutex>`. If i have a lot of readers we can wait 
`std::shared_lock` c++17 can organize read lock and write lock (read can many, but write one)

### Deadlocks
mutex should be more **global** than local
never envoke function if using `mutex::lock` with `mutex::unlock` (save results of function in local variable and then in lock region only assign)

Lock in a different sequence???? wats a problem (look uniqueLock.cpp)
dont use mutexes - use locks: `lock_guard`, `unique_lock`

`std::cout` is like global variable and it may cause some  interleaving(mess), so protect `std::cout` by mutex
`std::cout` problem is not DataRace, because it is thread safe?? wtf

## Locks

`std::lock_guard`, `std::unique_lock` - RAII for mutexes, need `<mutex>`

```cpp
std::mutex myMutex;
{
std::lock_guard<std::mutex> myLock(myMutex);
sharedData = getVar()
}
```

`std::unique_lock` is more powerfull. Can lock unlock, and work without mutex
`unique_lock::release` - stops controlling mutex without unlocking, returns mutex
`std::lock(...)` - lock many locks in atomically. it is invariadic template

in c++14 `shared_lock`, with `shared_timed_mutex`,  c++17 `scoped_lock`

lock shared data between `std::defer_lock` and `std::lock(_locks_)` will help in different sequence deadlock (uniqueLock.cpp)

readerWriterLock.cpp datarace problem if we try to read from map by index operation before wee add data we will get result 0, while map is inserting if key was not in it. solution in index operator

if variable is const we just need too sure that it is initialized no need to use mutexes to read

### Thread safe init

 - static init - fastest way
 - call_once
 - constexpr
 
For more look [thread-safe singleton init](http://www.modernescpp.com/index.php/thread-safe-initialization-of-a-singleton)
 
`std::call_once` with `std::once_flag` garantee that data wiil be executed ones also when we have many threads. `std::call_once` is faster then using `atomic`.

**Thread safe singleton**

Look threadsafe singleton.cpp

**double-checked locking pattern** now is antipattern it is not thread safe. Look [explanation](http://modernescpp.com/index.php/thread-safe-initialization-of-data)

**async example:**
```cpp
int gettime()
{
    //do someting
}

auto fut1= std::async(std::launch::async,getTime);
auto fut2= std::async(std::launch::async,getTime);
auto fut3= std::async(std::launch::async,getTime);
auto fut4= std::async(std::launch::async,getTime);

auto total= fut1.get() + fut2.get() + fut3.get() + fut4.get();
```

`thread_local` mostly used to make multithreaded app from singlethreaded

### Problems of condition variables

**spurious wakeup** problem while using condition variable (`notify_one`, `notify_all`) - not clock ring waked you up in the morning but just a cat. Look 59 slide (using predicate to avoid this problem)
**lost wakeup** - also 59 slide

**Using predicate**
`condVar.wait(lck, []{ return dataReady; });` is the same as

```cpp
while (the_condition == 0) {
  cv.wait(lock);
}
```

### Tasks

`<future>` header
`std::async` - function that will execute asynchronous

in c++20 we will get extended futures with **when** and others

`std::async` can be launched with a _launch policy_:
- `std::launch::async` (eager) in another thread
- `std::launch::deferred` (lazy) in the same thread - function callable executes only when future.get() or future.wait() is called. As needed evaluation (Look asyncLazyEagle.cpp)

## Final and See more

http://ru.cppreference.com/w/cpp/atomic/memory_order

mutexes for processors not exists

[Rainer Grimm's site](http://modernescpp.com/)

`std::atomic_flag` and simple **SpinLock**
`std::atomic<bool>` and creating condition variable (12, 13 slide). Condition variables are *push* mechanism and with `std::atomic<bool>` we can do *pull* mechanism
memory_order for `std::atomic::load`
memory models
**CppMem** tool
HPX framework







