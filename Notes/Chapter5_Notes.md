# Chapter 5 - Concurrency: Mutual Exclusion and Synchronization
*Operating Systems: Internals and Design Principles, Eighth Edition — William Stallings*

---

## Multiple Processes

Operating System design is concerned with the management of processes and threads:
- Multiprogramming
- Multiprocessing
- Distributed Processing

---

## Concurrency Arises in Three Different Contexts

- **Multiple applications** — invented to allow processing time to be shared among active applications
- **Structured applications** — extension of modular design and structured programming
- **Operating System structure** — OS themselves implemented as a set of processes or threads

---

## Key Terms Related to Concurrency

| Term | Definition |
|------|------------|
| **Atomic operation** | A function or action implemented as a sequence of one or more instructions that appears to be indivisible; no other process can see an intermediate state or interrupt the operation. Atomicity guarantees isolation from concurrent processes. |
| **Critical section** | A section of code within a process that requires access to shared resources and that must not be executed while another process is in a corresponding section of code. |
| **Deadlock** | A situation in which two or more processes are unable to proceed because each is waiting for one of the others to do something. |
| **Livelock** | A situation in which two or more processes continuously change their states in response to changes in the other process(es) without doing any useful work. |
| **Mutual exclusion** | The requirement that when one process is in a critical section that accesses shared resources, no other process may be in a critical section that accesses any of those shared resources. |
| **Race condition** | A situation in which multiple threads or processes read and write a shared data item and the final result depends on the relative timing of their execution. |
| **Starvation** | A situation in which a runnable process is overlooked indefinitely by the scheduler; although it is able to proceed, it is never chosen. |

---

## Principles of Concurrency

- **Interleaving and overlapping** can be viewed as examples of concurrent processing — both present the same problems
- On a **uniprocessor**, the relative speed of execution of processes cannot be predicted; it depends on:
  - Activities of other processes
  - The way the OS handles interrupts
  - Scheduling policies of the OS

---

## Difficulties of Concurrency

- Sharing of global resources
- Difficult for the OS to manage the allocation of resources optimally
- Difficult to locate programming errors as results are not deterministic and reproducible

---

## Race Condition

- Occurs when multiple processes or threads read and write data items
- The final result depends on the order of execution
- The "loser" of the race is the process that updates last and will determine the final value of the variable

---

## Operating System Concerns

Design and management issues raised by the existence of concurrency. The OS must:
1. Be able to keep track of various processes
2. Allocate and de-allocate resources for each active process
3. Protect the data and physical resources of each process against interference by other processes
4. Ensure that the processes and outputs are independent of the processing speed

---

## Process Interaction

| Degree of Awareness | Relationship | Influence | Potential Control Problems |
|---|---|---|---|
| Processes unaware of each other | Competition | Results of one process independent of the actions of others; timing may be affected | Mutual exclusion, Deadlock (renewable resource), Starvation |
| Processes indirectly aware of each other (e.g., shared object) | Cooperation by sharing | Results of one process may depend on information obtained from others; timing may be affected | Mutual exclusion, Deadlock, Starvation, Data coherence |
| Processes directly aware of each other (have communication primitives) | Cooperation by communication | Results of one process may depend on information obtained from others; timing may be affected | Deadlock (consumable resource), Starvation |

---

## Resource Competition

Concurrent processes conflict when competing for use of the same resource (e.g., I/O devices, memory, processor time, clock). Three control problems must be faced:
1. The need for **mutual exclusion**
2. **Deadlock**
3. **Starvation**

---

## Mutual Exclusion — Illustration

```c
/* Process 1 */
void P1 {
    while (true) {
        /* preceding code */
        enter_critical(Ra);
        /* critical section */
        exit_critical(Ra);
        /* following code */
    }
}

/* Process 2 (and so on...) */
void P2 {
    while (true) {
        /* preceding code */
        enter_critical(Ra);
        /* critical section */
        exit_critical(Ra);
        /* following code */
    }
}
```

---

## Requirements for Mutual Exclusion

1. Must be enforced
2. A process that halts must do so without interfering with other processes
3. No deadlock or starvation
4. A process must not be denied access to a critical section when there is no other process using it
5. No assumptions are made about relative process speeds or number of processes
6. A process remains inside its critical section for a finite time only

---

## Mutual Exclusion: Hardware Support

### Interrupt Disabling
- Used in uniprocessor systems — disabling interrupts guarantees mutual exclusion
- **Disadvantages:**
  - Efficiency of execution could be noticeably degraded
  - Does not work in a multiprocessor architecture

### Test and Set Instruction
```c
boolean test_and_set(int i) {
    if (i == 0) {
        i = 1;
        return true;
    } else {
        return false;
    }
}

/* program mutualexclusion */
const int n; /* number of processes */
int bolt;
void P(int i) {
    while (true) {
        while (!test_and_set(&bolt))
            /* do nothing */;
        /* critical section */
        bolt = 0;
        /* remainder */
    }
}
void main() {
    bolt = 0;
    parbegin(P(1), P(2), ..., P(n));
}
```

### Compare and Swap Instruction
Also called a "compare and exchange instruction." A comparison is made between a memory value and a test value; if the values are the same, a swap occurs, carried out atomically.

```c
int compare_and_swap(mut, expected, new) {
    old = mut;
    if (mut == expected)
        mut = new;
    return old;
}
```

---

## Special Machine Instructions — Advantages and Disadvantages

**Advantages:**
- Applicable to any number of processes on either a single processor or multiple processors sharing main memory
- Simple and easy to verify
- Can support multiple critical sections (each defined by its own variable)

**Disadvantages:**
- **Busy-waiting** — a waiting process continues to consume processor time
- **Starvation** is possible when a process leaves a critical section and more than one process is waiting
- **Deadlock** is possible

---

## Common Concurrency Mechanisms

| Mechanism | Description |
|-----------|-------------|
| **Semaphore** | An integer value used for signaling among processes. Only three atomic operations: initialize, decrement (semWait), and increment (semSignal). Also known as a counting or general semaphore. |
| **Binary Semaphore** | A semaphore that takes on only the values 0 and 1. |
| **Mutex** | Similar to a binary semaphore. Key difference: the process that locks the mutex must be the one to unlock it. |
| **Condition Variable** | A data type used to block a process or thread until a particular condition is true. |
| **Monitor** | A programming language construct that encapsulates variables, access procedures, and initialization code. Only one process may be actively accessing the monitor at any one time. |
| **Event Flags** | A memory word used as a synchronization mechanism. Threads can wait for single or combined events by checking bits in a flag. |
| **Mailboxes/Messages** | A means for two processes to exchange information; may be used for synchronization. |
| **Spinlocks** | Mutual exclusion mechanism in which a process executes in an infinite loop waiting for a lock variable to indicate availability. |

---

## Semaphores

A variable that has an integer value upon which only three operations are defined:
1. May be initialized to a nonnegative integer value
2. The `semWait` operation decrements the value
3. The `semSignal` operation increments the value

> There is no way to inspect or manipulate semaphores other than these three operations.

### Consequences
- There is no way to know before a process decrements a semaphore whether it will block or not
- There is no way to know which process will continue immediately on a uniprocessor when two processes are running concurrently
- The number of unblocked processes may be zero or one

### Semaphore Primitives Definition

```c
struct semaphore {
    int count;
    queueType queue;
};

void semWait(semaphore &s) {
    s.count--;
    if (s.count < 0) {
        /* place this process in s.queue */
        /* block this process */
    }
}

void semSignal(semaphore &s) {
    s.count++;
    if (s.count <= 0) {
        /* remove a process P from s.queue */
        /* place process P on ready list */
    }
}
```

### Binary Semaphore Primitives

```c
struct binary_semaphore {
    enum {zero, one} value;
    queueType queue;
};

void semWaitB(binary_semaphore s) {
    if (s.value == one)
        s.value = zero;
    else {
        /* place this process in s.queue */
        /* block this process */
    }
}

void semSignalB(semaphore s) {
    if (s.queue is empty())
        s.value = one;
    else {
        /* remove a process P from s.queue */
        /* place process P on ready list */
    }
}
```

### Strong vs. Weak Semaphores

- **Strong semaphores** — the process that has been blocked the longest is released first (FIFO)
- **Weak semaphores** — the order in which processes are removed from the queue is not specified

### Mutual Exclusion Using Semaphores

```c
/* program mutual exclusion */
const int n = /* number of processes */;
semaphore s = 1;
void P(int i) {
    while (true) {
        semWait(s);
        /* critical section */
        semSignal(s);
        /* remainder */
    }
}
void main() {
    parbegin(P(1), P(2), ..., P(n));
}
```

---

## Producer/Consumer Problem

**General Statement:**
- One or more producers are generating data and placing it in a buffer
- A single consumer takes items out of the buffer one at a time
- Only one producer or consumer may access the buffer at any one time

**The Problem:** Ensure that the producer can't add data to a full buffer and the consumer can't remove data from an empty buffer.

### Solution Using Semaphores (Infinite Buffer)

```c
/* program producerconsumer */
semaphore n = 0, s = 1;
void producer() {
    while (true) {
        produce();
        semWait(s);
        append();
        semSignal(s);
        semSignal(n);
    }
}
void consumer() {
    while (true) {
        semWait(n);
        semWait(s);
        take();
        semSignal(s);
        consume();
    }
}
void main() {
    parbegin(producer, consumer);
}
```

### Solution Using Semaphores (Bounded Buffer)

```c
/* program bounded_buffer */
const int size_of_buffer = /* buffer size */;
semaphore s = 1, n = 0, e = size_of_buffer;
void producer() {
    while (true) {
        produce();
        sem_wait(e);
        sem_wait(s);
        append();
        sem_signal(s);
        sem_signal(n);
    }
}
void consumer() {
    while (true) {
        sem_wait(n);
        sem_wait(s);
        take();
        sem_signal(s);
        sem_signal(e);
        consume();
    }
}
void main() {
    parbegin(producer, consumer);
}
```

---

## Implementation of Semaphores

- `semWait` and `semSignal` operations must be implemented as **atomic primitives**
- Can be implemented in hardware or firmware
- Software schemes such as **Dekker's** or **Peterson's** algorithms can be used
- Hardware-supported schemes for mutual exclusion can be used

---

## Monitors

A programming language construct that provides equivalent functionality to semaphores and is easier to control.

- Implemented in languages including Concurrent Pascal, Pascal-Plus, Modula-2, Modula-3, and Java
- Has also been implemented as a program library
- A software module consisting of one or more procedures, an initialization sequence, and local data

**Characteristics:**
- Local data variables are accessible only by the monitor's procedures
- A process enters the monitor by invoking one of its procedures
- Only one process may be executing in the monitor at a time

### Synchronization with Condition Variables

- `cwait(c)` — suspend execution of the calling process on condition c
- `csignal(c)` — resume execution of some process blocked after a `cwait` on the same condition

### Monitor Solution to Bounded-Buffer Producer/Consumer

```c
monitor boundedbuffer;
char buffer[N];
int nextin, nextout, count;
cond notfull, notempty;

void append(char x) {
    if (count == N)
        cwait(notfull);   /* buffer full; avoid overflow */
    buffer[nextin] = x;
    nextin = (nextin + 1) % N;
    count++;
    csignal(notempty);
}

void take(char x) {
    if (count == 0)
        cwait(notempty);  /* buffer empty; avoid underflow */
    x = buffer[nextout];
    nextout = (nextout + 1) % N;
    count--;
    csignal(notfull);
}
/* initialization */
{ nextin = 0; nextout = 0; count = 0; }
```

---

## Message Passing

When processes interact, two fundamental requirements must be satisfied:
- **Synchronization** — to enforce mutual exclusion
- **Communication** — to exchange information

Works with distributed systems, shared memory multiprocessors, and uniprocessor systems.

### Primitives

```
send(destination, message)
receive(source, message)
```

### Synchronization Options

- **Blocking send, blocking receive** — both are blocked until the message is delivered (also called a *rendezvous*)
- **Nonblocking send, blocking receive** — most useful; sender continues while receiver waits
- **Nonblocking send, nonblocking receive** — neither party waits

### Addressing

- **Direct addressing** — the send primitive includes a specific identifier of the destination process
- **Indirect addressing** — messages are sent to shared queues called **mailboxes**, providing greater flexibility

### Message Format

A message typically contains a **header** (message type, destination ID, source ID, message length, control information) and a **body** (message contents).

---

## Readers/Writers Problem

A data area is shared among many processes — some only read (readers), some only write (writers).

**Conditions that must be satisfied:**
1. Any number of readers may simultaneously read the file
2. Only one writer at a time may write to the file
3. If a writer is writing, no reader may read

### Readers Have Priority

```c
int readcount;
semaphore x = 1, wsem = 1;
void reader() {
    while (true) {
        semWait(x);
        readcount++;
        if (readcount == 1) semWait(wsem);
        semSignal(x);
        READUNIT();
        semWait(x);
        readcount--;
        if (readcount == 0) semSignal(wsem);
        semSignal(x);
    }
}
void writer() {
    while (true) {
        semWait(wsem);
        WRITEUNIT();
        semSignal(wsem);
    }
}
```

### Writers Have Priority

Uses additional semaphores (`y`, `z`, `rsem`) to track write count and prevent reader starvation of writers.

---

## Summary

- Principles of concurrency: race conditions, OS concerns, process interaction
- Mutual exclusion requirements and hardware support (interrupt disabling, test-and-set, compare-and-swap)
- **Semaphores**: mutual exclusion, producer/consumer problem, implementation
- **Monitors**: signal-based and notify/broadcast models
- **Message passing**: synchronization, addressing, message format, queuing discipline, mutual exclusion
- **Readers/Writers problem**: readers-priority and writers-priority solutions
