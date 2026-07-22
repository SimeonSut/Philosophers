*This project has been created as part of the 42 curriculum by ssutarmi.*

# Philosophers

## Description

Philosophers is an implementation of the classic **dining philosophers problem**. At 42 it serves as an introduction to concurrent programming: creating and joining threads, protecting shared resources with mutexes, and avoiding the two things that make concurrency hard — **data races** and **deadlocks**.

Several philosophers sit around a round table with a large bowl of spaghetti in the middle. Between each pair of neighbours lies a single fork, so there are exactly as many forks as philosophers. Each philosopher endlessly cycles through three states — *thinking*, *eating*, and *sleeping* — and must hold the two forks to their immediate left and right in order to eat. If a philosopher goes longer than `time_to_die` without starting a meal, they starve and the simulation ends immediately. Philosophers never communicate and have no knowledge of their neighbours' state, which is precisely what makes safely sharing the forks non-trivial.

This repository contains the **mandatory part**: every philosopher is represented as an independent thread, and every fork's state is guarded by its own mutex.

## Instructions

### Requirements

- A C compiler (`cc` / `gcc`) and `make`
- A POSIX system with the pthreads library (Linux or macOS)

### Compilation

From the project directory:

```sh
make
```

This produces an executable named `philo`. The available rules are:

| Rule      | Effect                                    |
| --------- | ----------------------------------------- |
| `make`    | Build the `philo` executable (default)    |
| `make all`| Same as `make`                            |
| `make clean` | Remove build output                    |
| `make fclean`| Remove build output (full clean)       |
| `make re` | Rebuild from scratch (`fclean` + `all`)   |

The project is compiled with `-Wall -Wextra -Werror -pthread`.

### Usage

```sh
./philo number_of_philosophers time_to_die time_to_eat time_to_sleep [number_of_times_each_philosopher_must_eat]
```

| Argument | Unit | Meaning |
| --- | --- | --- |
| `number_of_philosophers` | count | Number of philosophers, and also the number of forks. |
| `time_to_die` | ms | If a philosopher does not start eating within this delay since their last meal (or since the start), they die. |
| `time_to_eat` | ms | How long a philosopher spends eating, holding two forks. |
| `time_to_sleep` | ms | How long a philosopher spends sleeping. |
| `number_of_times_each_philosopher_must_eat` | count | *Optional.* Once every philosopher has eaten at least this many times, the simulation stops. If omitted, the simulation runs until a philosopher dies. |

All time values are given in **milliseconds**.

### Examples

```sh
# Nobody should ever die with these values
./philo 5 800 200 200

# Stops once every philosopher has eaten 7 times
./philo 5 800 200 200 7

# A single philosopher: only one fork is reachable, so they die
./philo 1 800 200 200

# Tight timing — a philosopher is expected to starve
./philo 4 310 200 100
```

### Output format

Every state change is logged on its own line as `timestamp_in_ms philosopher_id state`:

```
<timestamp_in_ms> <X> has taken a fork
<timestamp_in_ms> <X> is eating
<timestamp_in_ms> <X> is sleeping
<timestamp_in_ms> <X> is thinking
<timestamp_in_ms> <X> died
```

`timestamp_in_ms` is the number of milliseconds elapsed since the start of the simulation, and `X` is the philosopher's number (from `1` to `number_of_philosophers`). Once a philosopher dies, the simulation stops and no further state messages are printed.

## Technical choices

- **Data model — a circular linked list.** Philosophers are stored in a circular singly-linked list rather than an array. Each node carries its own fork, represented as a state flag (`UNLOCKED` / `LOCKED`) together with a dedicated mutex. A shared context structure holds the simulation parameters, the shared "death" flag, the start time, and the mutexes used for coordination and printing.

- **Fork protection — a mutex-guarded state flag.** Each fork's mutex is not held for the whole duration of a meal; instead it protects a boolean flag. A philosopher briefly locks the mutex to test-and-set the flag (take the fork), then releases the mutex immediately. This keeps the critical sections short and confines the shared state to a well-defined lock.

- **Deadlock avoidance — a "gate" mechanism.** The naive strategy where every philosopher grabs their left fork at the same instant deadlocks. To prevent this, adjacent philosophers are paired onto a shared *gate* mutex (an array of ⌈n/2⌉ gates). Each philosopher must pass its gate before entering the eat/sleep/think loop, which staggers the order in which neighbours reach for the forks and breaks the circular-wait condition.

- **Death detection — distributed monitoring.** Rather than a single dedicated monitor thread, each philosopher thread periodically re-checks whether its own elapsed time since the last meal has exceeded `time_to_die`. The first thread to detect a death raises a shared flag under a mutex, and every other thread observes it and stops. Terminal output is serialized through a separate mutex so that log lines never interleave.

- **Single-philosopher case.** With one philosopher there is only one reachable fork, so eating is impossible. This case is handled explicitly: the philosopher takes the single fork and dies after `time_to_die`.

- **Timing.** Time is measured with `gettimeofday` and kept internally in microseconds for precision, while log timestamps are printed in milliseconds. Waits during eating and sleeping are broken into short `usleep` slices so the simulation can react to a death well within the required 10 ms window.

### Project structure

| File | Responsibility |
| --- | --- |
| `main.c` | Entry point, argument-count check, orchestration, cleanup |
| `input_check.c` | Argument parsing/validation and structure initialization |
| `input_check_utils.c` | Small helpers (`ft_strlen`, `ft_strcmp`, `ft_atoi`) and additional setup |
| `threads.c` | Thread creation, joining, and the per-thread start routine |
| `routine.c` | The eat / sleep / think loop and fork handling |
| `routine_utils.c` | Gate management, state announcements, and eat-count tracking |
| `time_utils.c` | Time storage, death checking, single-philosopher death wait |
| `philosophers.h` | Structures, function prototypes, and constants |
| `Makefile` | Build rules |

## Resources

- The dining philosophers problem — E. W. Dijkstra's classic concurrency problem, and the standard descriptions of the circular-wait / deadlock conditions.
- POSIX threads manual pages: `pthread_create`, `pthread_join`, `pthread_detach`, `pthread_mutex_init`, `pthread_mutex_lock`, `pthread_mutex_unlock`, `pthread_mutex_destroy`.
- Time and scheduling manual pages: `gettimeofday`, `usleep`.
- General reading on mutexes, race conditions, and deadlock avoidance in multithreaded programs.

### Use of AI

AI was used as a **learning and documentation aid**, not as a generator for the graded logic:

- Discussing the theory behind the problem — the thread lifecycle (`pthread_create` / `pthread_join`) and how mutexes prevent data races on shared state.
- Comparing deadlock-avoidance strategies (staggering neighbours, resource ordering) before settling on the gate-based approach used here.
- Drafting and structuring this `README.md`.

> **Note:** revise this section so it reflects exactly how *you* used AI on this project. During peer evaluation you must be able to explain and defend every part of the code, including anything AI helped with.
