# Chapter 9: Uniprocessor Scheduling
*Operating Systems: Internals and Design Principles — William Stallings, 8th Edition*

---

## Types of Scheduling

| Scheduling Type | Description |
|---|---|
| **Long-term scheduling** | The decision to add to the pool of processes to be executed |
| **Medium-term scheduling** | The decision to add to the number of processes that are partially or fully in main memory |
| **Short-term scheduling** | The decision as to which available process will be executed by the processor |
| **I/O scheduling** | The decision as to which process's pending I/O request shall be handled by an available I/O device |

**Goal:** Assign processes to be executed by the processor in a way that meets system objectives such as response time, throughput, and processor efficiency.

---

## Scheduling and Process State Transitions

- **Long-term scheduling** moves processes from New → Ready/Suspend or Ready
- **Medium-term scheduling** moves processes from Ready/Suspend → Ready, and Blocked/Suspend → Blocked
- **Short-term scheduling** moves processes from Ready → Running

---

## Long-Term Scheduling

- Determines which programs are admitted to the system for processing
- Controls the **degree of multiprogramming**
  - More processes created = smaller percentage of time each process can execute
  - May limit processes to provide satisfactory service to the current set
- Must decide:
  - **When** the OS can take on one or more additional processes
  - **Which** jobs to accept and turn into processes

Criteria for job selection include: first come first served, priority, expected execution time, I/O requirements.

---

## Medium-Term Scheduling

- Part of the **swapping function**
- Swapping-in decisions are based on the need to manage the degree of multiprogramming
- Considers the memory requirements of swapped-out processes

---

## Short-Term Scheduling (Dispatcher)

- Executes **most frequently** of all scheduling types
- Makes fine-grained decisions about which process to execute next
- Invoked when an event occurs that may block the current process or provide an opportunity to preempt it

Events that trigger the dispatcher:
- Clock interrupts
- I/O interrupts
- Operating system calls
- Signals (e.g., semaphores)

---

## Short-Term Scheduling Criteria

### User-Oriented vs. System-Oriented

| Orientation | Focus |
|---|---|
| **User-oriented** | Behavior as perceived by individual users/processes (e.g., response time in an interactive system) |
| **System-oriented** | Effective and efficient utilization of the processor (e.g., rate at which processes are completed) |

### Performance-Related vs. Non-Performance-Related

| Type | Characteristics | Examples |
|---|---|---|
| **Performance-related** | Quantitative, easily measured | Response time, throughput |
| **Non-performance-related** | Qualitative, hard to measure | Predictability |

### Scheduling Criteria (Table 9.2)

**User-Oriented, Performance-Related:**
- **Turnaround time** — interval between process submission and completion; includes execution time plus waiting time; good for batch jobs
- **Response time** — time from request submission until response begins to be received; better measure than turnaround for interactive processes
- **Deadlines** — scheduling should maximize the percentage of deadlines met

**User-Oriented, Other:**
- **Predictability** — a given job should run in about the same amount of time regardless of system load

**System-Oriented, Performance-Related:**
- **Throughput** — maximize number of processes completed per unit of time
- **Processor utilization** — percentage of time the processor is busy

**System-Oriented, Other:**
- **Fairness** — processes should be treated equally; no process should suffer starvation
- **Enforcing priorities** — scheduling policy should favor higher-priority processes

---

## Selection Function and Decision Mode

### Selection Function
Determines which ready process is selected next. May be based on:
- Priority
- Resource requirements
- Execution characteristics

Key variables when based on execution characteristics:
- **w** = time spent in system so far, waiting
- **e** = time spent in execution so far
- **s** = total service time required (must be estimated or provided by user)

### Decision Mode
Specifies when the selection function is exercised. Two categories:

| Mode | Description |
|---|---|
| **Nonpreemptive** | Once a process is Running, it continues until it terminates or blocks itself for I/O |
| **Preemptive** | Running process may be interrupted and moved to Ready by the OS; preemption can occur when a new process arrives, on an interrupt, or periodically |

---

## Scheduling Algorithms

### Process Example Used in Comparisons

| Process | Arrival Time | Service Time |
|---|---|---|
| A | 0 | 3 |
| B | 2 | 6 |
| C | 4 | 4 |
| D | 6 | 5 |
| E | 8 | 2 |

---

### First-Come-First-Served (FCFS)

- **Also known as:** FIFO (first-in-first-out)
- **Decision mode:** Nonpreemptive
- **Selection function:** max[w] — longest waiting process selected
- When the current process stops, the process that has been in the Ready queue the longest is selected next
- Performs better for **long processes** than short ones
- Tends to **favor processor-bound processes** over I/O-bound processes

**Drawbacks:** Can cause high turnaround time when there is large variance in process execution times.

---

### Round Robin (RR)

- **Also known as:** Time slicing
- **Decision mode:** Preemptive (at time quantum)
- Uses preemption based on a clock — each process is given a **time quantum (slice)** before being preempted
- Key design issue: choosing the right length for the time quantum
  - Too small → overhead from frequent context switching, low throughput
  - Too large → degenerates toward FCFS behavior
- Particularly effective in **general-purpose time-sharing** or **transaction processing** systems
- Drawback: unequal treatment of processor-bound vs. I/O-bound processes

**Virtual Round Robin** — improvement that adds an auxiliary queue for processes completing I/O, giving them higher priority to compensate for processor time lost during I/O waits.

---

### Shortest Process Next (SPN)

- **Decision mode:** Nonpreemptive
- **Selection function:** min[s] — minimum expected service time
- Process with the **shortest expected processing time** is selected next
- Short processes jump ahead in the queue
- Risk of **starvation** for longer processes
- Difficulty: requires knowing (or estimating) the required processing time
  - If estimate is substantially under actual time, system may abort the job

---

### Shortest Remaining Time (SRT)

- **Decision mode:** Preemptive (at arrival of new process)
- **Selection function:** min[s – e] — minimum remaining service time
- Preemptive version of SPN
- Scheduler always chooses the process with the **shortest expected remaining processing time**
- A newly arrived short job can preempt a currently running longer job
- Risk of **starvation** for longer processes
- Gives **superior turnaround time** compared to SPN

---

### Highest Response Ratio Next (HRRN)

- **Decision mode:** Nonpreemptive
- **Selection function:** max[(w + s) / s]

**Ratio formula:**

```
ratio = (time_spent_waiting + expected_service_time) / expected_service_time
```

- Chooses the process with the **greatest ratio**
- Accounts for the **age** of the process (aging)
- Shorter jobs are favored, but as a longer process waits, its ratio increases — it will eventually get past competing shorter jobs
- Good balance between favoring short jobs and preventing starvation

---

### Feedback Scheduling

- **Decision mode:** Preemptive (at time quantum)
- Uses **multiple priority queues** (RQ0, RQ1, ..., RQn)
- New processes enter at the highest priority queue (RQ0)
- If a process uses its full time quantum without completing, it is moved down to the next lower-priority queue
- Processes in lower queues receive longer time quanta
- Favors **short and I/O-bound** processes (they tend to complete quickly and not get demoted)
- Long-running processes gradually sink to lower queues

---

## Comparison of Scheduling Policies

| Policy | Selection Function | Decision Mode | Throughput | Response Time | Overhead | Effect |
|---|---|---|---|---|---|---|
| **FCFS** | max[w] | Nonpreemptive | Not emphasized | May be high | Minimum | Penalizes short & I/O-bound |
| **RR (q=1)** | Constant | Preemptive (quantum) | May be low if q too small | Good for short | Minimum | Fair treatment |
| **SPN** | min[s] | Nonpreemptive | High | Good for short | Can be high | Penalizes long processes |
| **SRT** | min[s-e] | Preemptive (arrival) | High | Good | Can be high | Penalizes long |
| **HRRN** | max[(w+s)/s] | Nonpreemptive | High | Good | Can be high | Good balance |
| **Feedback** | (see text) | Preemptive (quantum) | — | Not emphasized | Can be high | May favor I/O-bound |

### Performance Data (Mean Turnaround Time Tr/Ts)

| Policy | A | B | C | D | E | Mean Tr |
|---|---|---|---|---|---|---|
| FCFS | 3 | 7 | 9 | 12 | 12 | 8.6 |
| RR q=1 | 4 | 16 | 13 | 14 | 7 | 10.8 |
| RR q=4 | 3 | 15 | 7 | 14 | 11 | 10 |
| SPN | 3 | 7 | 11 | 14 | 3 | 7.6 |
| SRT | 3 | 13 | 4 | 14 | 2 | 7.2 |
| HRRN | 3 | 7 | 9 | 14 | 7 | 8 |
| FB q=1 | 4 | 18 | 12 | 13 | 3 | 10 |

---

## Performance Comparison Formula

For any scheduling discipline that chooses the next process **independent of service time**:

```
Tr / Ts = 1 / (1 - ρ)
```

Where:
- **Tr** = turnaround/residence time (total time in system: waiting + execution)
- **Ts** = average service time (average time spent in Running state)
- **ρ** = processor utilization

---

## Fair-Share Scheduling

- Scheduling decisions are based on **process sets** (user groups)
- Each user is assigned a **share of the processor**
- Objective: monitor usage and give fewer resources to users who have had more than their fair share, and more to those who have had less
- Prevents any one user's processes from monopolizing the processor at the expense of other users' processes

---

## Summary

| Topic | Key Points |
|---|---|
| Long-term scheduling | Controls degree of multiprogramming; decides which jobs enter the system |
| Medium-term scheduling | Swapping function; manages processes between main memory and disk |
| Short-term scheduling | Dispatcher; most frequent; decides which ready process runs next |
| FCFS | Simple, nonpreemptive; favors long/processor-bound; can starve short jobs |
| Round Robin | Preemptive by clock; time slicing; fair but I/O-bound disadvantaged |
| SPN | Nonpreemptive; shortest job first; can starve long jobs |
| SRT | Preemptive SPN; best turnaround; starvation risk |
| HRRN | Nonpreemptive; uses aging ratio to balance short-job preference and fairness |
| Feedback | Multi-level queue; demotes long-running jobs; favors short/I/O-bound |
| Fair-share | Allocates processor shares by user group; prevents monopolization |
