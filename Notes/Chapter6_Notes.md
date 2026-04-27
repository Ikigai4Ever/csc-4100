# Chapter 6 — Concurrency: Deadlock and Starvation
*Operating Systems: Internals and Design Principles, Eighth Edition — William Stallings*

---

## Deadlock

- The **permanent blocking** of a set of processes that either compete for system resources or communicate with each other
- A set of processes is deadlocked when each process in the set is blocked awaiting an event that can only be triggered by another blocked process in the set
- It is **permanent** — there is no efficient solution

---

## Resource Categories

**Reusable Resources**
- Can be safely used by only one process at a time and is not depleted by that use
- Examples: processors, I/O channels, main and secondary memory, devices, and data structures such as files, databases, and semaphores

**Consumable Resources**
- One that can be created (produced) and destroyed (consumed)
- Examples: interrupts, signals, messages, and information in I/O buffers

---

## Deadlock Examples

### Example 1: Competition for Reusable Resources

| Process P | | Process Q | |
|---|---|---|---|
| Step | Action | Step | Action |
| p0 | Request(D) | q0 | Request(T) |
| p1 | Lock(D) | q1 | Lock(T) |
| p2 | Request(T) | q2 | Request(D) |
| p3 | Lock(T) | q3 | Lock(D) |
| p4 | Perform function | q4 | Perform function |
| p5 | Unlock(D) | q5 | Unlock(T) |
| p6 | Unlock(T) | q6 | Unlock(D) |

Deadlock occurs if P locks D and Q locks T, then each requests what the other holds.

### Example 2: Memory Request

200 KB available; the following sequence occurs:

```
P1:  Request 80 KB  →  Request 60 KB
P2:  Request 70 KB  →  Request 80 KB
```

Deadlock occurs if both processes progress to their second request (80 + 70 = 150 KB allocated, neither can satisfy the second request).

### Example 3: Consumable Resources

```
P1:  Receive(P2) → Send(P2, M1)
P2:  Receive(P1) → Send(P1, M2)
```

Deadlock occurs if the `Receive` is blocking — each process waits for the other to send first.

---

## Conditions for Deadlock

All four conditions must hold simultaneously for a deadlock to occur:

1. **Mutual Exclusion** — only one process may use a resource at a time
2. **Hold-and-Wait** — a process may hold allocated resources while awaiting assignment of others
3. **No Pre-emption** — no resource can be forcibly removed from a process holding it
4. **Circular Wait** — a closed chain of processes exists such that each process holds at least one resource needed by the next process in the chain

---

## Resource Allocation Graphs

Used to visualize resource allocation and detect deadlock:

- **Process → Resource** (arrow pointing to resource): process is *requesting* the resource
- **Resource → Process** (arrow pointing to process): resource is *held by* the process
- A **circular wait** in the graph (with single-instance resources) indicates deadlock

Key graph patterns:
- **(a) Resource is requested** — P1 → Ra
- **(b) Resource is held** — Ra → P1
- **(c) Circular wait** — deadlock condition
- **(d) No deadlock** — multiple resource instances prevent deadlock even with a cycle

---

## Dealing with Deadlock

Three general approaches:

1. **Deadlock Prevention** — adopt a policy that eliminates one of the four necessary conditions
2. **Deadlock Avoidance** — make dynamic choices based on the current state of resource allocation
3. **Deadlock Detection** — detect the presence of deadlock and take recovery action

---

## Deadlock Prevention

Design the system so that the possibility of deadlock is excluded. Two main methods:

- **Indirect** — prevent the occurrence of one of the three necessary conditions (mutual exclusion, hold-and-wait, no preemption)
- **Direct** — prevent the occurrence of a circular wait

### Preventing Each Condition

**Mutual Exclusion**
- Cannot generally be eliminated — if access to a resource requires mutual exclusion, the OS must support it

**Hold and Wait**
- Require that a process request *all* of its required resources at one time, blocking until all can be granted simultaneously

**No Preemption**
- If a process holding resources is denied a further request, it must release its original resources and request again
- The OS may preempt the second process and require it to release its resources

**Circular Wait**
- Define a **linear ordering of resource types** — a process may only request resources in increasing order, preventing cycles

---

## Deadlock Avoidance

A decision is made dynamically whether the current resource allocation request will, if granted, potentially lead to a deadlock. Requires knowledge of future process requests.

### Two Approaches

- **Process Initiation Denial** — do not start a process if its demands might lead to deadlock
- **Resource Allocation Denial** — do not grant an incremental resource request if this allocation might lead to deadlock (the **Banker's Algorithm**)

### The Banker's Algorithm (Resource Allocation Denial)

- The **state** of the system reflects current allocation of resources to processes
- A **safe state** is one where there is at least one sequence of resource allocations that does not result in deadlock
- An **unsafe state** is any state that is not safe

#### Safe State Determination Algorithm

```
loop:
  choose a process Pi that is not marked
  check V[j] - (C[j] - A[j]) for Pi
  if resources can be allocated to Pi:
    mark Pi and return A[i] to V
until no process can be marked

if all Pi marked → state is SAFE
```

Where:
- **C** = Claim matrix (maximum resource needs)
- **A** = Allocation matrix (currently allocated)
- **R** = Resource vector (total resources)
- **V** = Available vector

#### Banker's Algorithm Code

```c
/* global data structures */
struct state {
    int resource[m];
    int available[m];
    int claim[n][m];
    int alloc[n][m];
};

/* resource allocation algorithm */
if (alloc[i,*] + request[*] > claim[i,*])
    <error>; /* total request > claim */
else if (request[*] > available[*])
    <suspend process>;
else {
    /* define new state */
    alloc[i,*] = alloc[i,*] + request[*];
    available[*] = available[*] - request[*];
}

if (safe(newstate))
    <carry out allocation>;
else {
    <restore original state>;
    <suspend process>;
}

/* test for safety (banker's algorithm) */
boolean safe(state S) {
    int currentavail[m];
    process rest[<number of processes>];
    currentavail = available;
    rest = {all processes};
    possible = true;
    while (possible) {
        find a process Pk in rest such that
            claim[k,*] - alloc[k,*] <= currentavail;
        if (found) {
            currentavail = currentavail + alloc[k,*];
            rest = rest - {Pk};
        } else possible = false;
    }
    return (rest == null);
}
```

### Deadlock Avoidance: Advantages and Restrictions

**Advantages:**
- Not necessary to preempt and rollback processes (unlike deadlock detection)
- Less restrictive than deadlock prevention

**Restrictions:**
- Processes must be independent with no synchronization requirements
- No process may exit while holding resources
- Maximum resource requirements must be stated in advance
- There must be a fixed number of resources to allocate

---

## Deadlock Detection

Resource requests are granted whenever possible; periodically check for deadlock.

### Detection Algorithm

```
1. Mark each process that has an all-zero row in the Allocation matrix
2. Initialize temporary vector W = Available vector
3. Find an unmarked process i where row i of Q (request matrix) ≤ W
4. If no such i is found, STOP
5. If found: mark process i and add row i of Allocation matrix to W
6. Goto 3
```

A deadlock exists for any process that remains **unmarked** at the end.

**Advantages:**
- Never delays process initiation
- Facilitates online handling
- Algorithm is relatively simple and leads to early detection

**Disadvantages:**
- Frequent checks consume considerable processor time
- Inherent preemption losses during recovery

---

## Deadlock Recovery Strategies

Once a deadlock is detected, one of the following approaches can be used:

1. **Abort all deadlocked processes**
2. **Back up** each deadlocked process to a previously defined checkpoint and restart all processes
3. **Successively abort** deadlocked processes one at a time until deadlock no longer exists
4. **Successively preempt resources** until deadlock no longer exists

---

## Summary of Approaches

| Approach | Resource Allocation Policy | Schemes | Major Advantages | Major Disadvantages |
|---|---|---|---|---|
| **Prevention** | Conservative; undercommits resources | Request all resources at once | Works well for burst-activity processes; no preemption needed | Inefficient; delays process initiation; future needs must be known |
| | | Preemption | Convenient for resources with saveable state | Preempts more often than necessary |
| | | Resource ordering | Enforceable at compile time; no run-time computation | Disallows incremental resource requests |
| **Avoidance** | Midway | Find at least one safe path | No preemption necessary | Future needs must be known by OS; processes can be blocked long periods |
| **Detection** | Very liberal; grant requests when possible | Invoke periodically to test for deadlock | Never delays process initiation; facilitates online handling | Inherent preemption losses |

---

## Dining Philosophers Problem

A classic synchronization problem used to illustrate deadlock and starvation.

**Setup:** Five philosophers sit at a round table with one fork between each pair. A philosopher needs two forks to eat.

**Constraints:**
- No two philosophers can use the same fork at the same time (**mutual exclusion**)
- No philosopher must starve to death (**avoid deadlock and starvation**)

### Solution 1 — Semaphores (can deadlock)

```c
/* program diningphilosophers */
semaphore fork[5] = {1};

void philosopher(int i) {
    while (true) {
        think();
        wait(fork[i]);
        wait(fork[(i+1) % 5]);
        eat();
        signal(fork[(i+1) % 5]);
        signal(fork[i]);
    }
}

void main() {
    parbegin(philosopher(0), philosopher(1),
             philosopher(2), philosopher(3), philosopher(4));
}
```

> **Problem:** If all philosophers pick up their left fork simultaneously, deadlock occurs.

### Solution 2 — Room Semaphore (prevents deadlock)

```c
/* program diningphilosophers */
semaphore fork[5] = {1};
semaphore room = {4};  /* at most 4 philosophers in the room at once */

void philosopher(int i) {
    while (true) {
        think();
        wait(room);
        wait(fork[i]);
        wait(fork[(i+1) % 5]);
        eat();
        signal(fork[(i+1) % 5]);
        signal(fork[i]);
        signal(room);
    }
}

void main() {
    parbegin(philosopher(0), philosopher(1),
             philosopher(2), philosopher(3), philosopher(4));
}
```

> **Fix:** Limiting occupancy to 4 ensures at least one philosopher can always eat.

### Solution 3 — Monitor Solution

```c
monitor dining_controller;
cond ForkReady[5];           /* condition variables */
boolean fork[5] = {true};   /* availability of each fork */

void get_forks(int pid) {
    int left = pid;
    int right = (++pid) % 5;
    if (!fork[left])
        cwait(ForkReady[left]);
    fork[left] = false;
    if (!fork[right])
        cwait(ForkReady[right]);
    fork[right] = false;
}

void release_forks(int pid) {
    int left = pid;
    int right = (++pid) % 5;
    if (empty(ForkReady[left]))
        fork[left] = true;
    else
        csignal(ForkReady[left]);
    if (empty(ForkReady[right]))
        fork[right] = true;
    else
        csignal(ForkReady[right]);
}

void philosopher[k = 0 to 4] {
    while (true) {
        <think>;
        get_forks(k);
        <eat spaghetti>;
        release_forks(k);
    }
}
```
