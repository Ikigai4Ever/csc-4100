# Chapter 2: Operating System Overview
*CSC 4100 Operating Systems — Stallings, 8th Edition*

---

## What is an Operating System?

A program that controls the execution of application programs and acts as an interface between applications and hardware.

**Main Objectives:**
- Convenience
- Efficiency
- Ability to be updated

---

## System Structure

The computer system is organized in layers:

- **Software layer:** Application programs → Libraries/utilities → Operating system
- **Hardware layer:** Execution hardware → System interconnect (bus) + Memory translation → I/O devices + Main memory

Key interfaces between layers:
- **ISA (Instruction Set Architecture)** — boundary between hardware and software
- **ABI (Application Binary Interface)** — interface for compiled programs
- **API (Application Programming Interface)** — interface for application programmers

---

## OS Services

The OS provides the following services to applications:
- Program development
- Program execution
- Access to I/O devices
- Controlled access to files
- System access
- Error detection and response
- Accounting

---

## Role of the OS

- A computer is a set of resources for the **movement, storage, and processing of data**
- The OS is responsible for **managing these resources**
- The OS is itself software — it runs on the processor just like any other program, but frequently relinquishes control and depends on the processor to regain it

---

## Evolution of Operating Systems

OS development progressed through four major stages:

### 1. Serial Processing
- **No operating system** — programmers interacted directly with hardware
- Computers ran from a console with display lights and toggle switches
- **Problems:**
  - *Scheduling:* Sign-up sheets for reserving time; allocations could run short or long
  - *Setup time:* Loading compilers, programs, and data manually was slow

### 2. Simple Batch Systems
- Early computers were expensive → maximize processor utilization
- A **Monitor** (resident software) automated job sequencing
  - User submits job to operator → operator batches jobs → monitor runs them in sequence
  - Program branches back to monitor when finished
- **Memory layout:** Monitor occupied one portion; user programs ran in the other
  - Monitor components: Interrupt Processing, Device Drivers, Job Sequencing, Control Language Interpreter

**Job Control Language (JCL):** A special language used to give the monitor instructions (e.g., what compiler to use, what data to use).

**Desirable hardware features to support batch systems:**

| Feature | Purpose |
|---|---|
| Memory protection | Prevents user programs from overwriting the monitor |
| Timer | Prevents a job from monopolizing the system |
| Privileged instructions | Can only be executed by the monitor |
| Interrupts | Gives OS more flexibility in controlling user programs |

**Modes of Operation:**

| User Mode | Kernel Mode |
|---|---|
| User programs execute here | Monitor executes here |
| Certain memory areas are protected | Can access protected memory |
| Certain instructions cannot be executed | Privileged instructions can execute |

**Overhead:** The monitor uses some main memory and some processor time, but overall system utilization improves.

---

### 3. Multiprogrammed Batch Systems

**The problem with simple batch systems:**
- The processor is often idle even with automatic job sequencing
- I/O devices are much slower than the processor
- Example: Reading/writing a record takes 30 µs; executing 100 instructions takes only 1 µs → CPU utilization = 1/31 ≈ **3.2%**

**Uniprogramming:** Processor executes until it hits an I/O instruction, then waits — completely idle during I/O.

**Multiprogramming (Multitasking):** Keep multiple jobs in memory; when one job waits for I/O, the processor switches to another job.

**Resource utilization comparison (example with 3 jobs):**

| Metric | Uniprogramming | Multiprogramming |
|---|---|---|
| Processor use | 20% | 40% |
| Memory use | 33% | 67% |
| Disk use | 33% | 67% |
| Elapsed time | 30 min | 15 min |
| Throughput | 6 jobs/hr | 12 jobs/hr |
| Mean response time | 18 min | 10 min |

---

### 4. Time-Sharing Systems

- Handles **multiple interactive users** simultaneously
- Processor time is shared among multiple users via **terminals**
- OS interleaves execution of each user program in short bursts called **quanta**

**Batch Multiprogramming vs. Time Sharing:**

| | Batch Multiprogramming | Time Sharing |
|---|---|---|
| Principal objective | Maximize processor use | Minimize response time |
| Directives source | JCL commands submitted with job | Commands entered at terminal |

**CTSS (Compatible Time-Sharing System):**
- Developed at MIT (Project MAC)
- 32,000 words of main memory; monitor used 5,000 words
- System clock generated interrupts every ~0.2 seconds
- At each interrupt, OS regained control and could assign processor to another user
- Preempted user programs were written out to disk and restored when their turn came

---

## Major OS Achievements

Operating systems are among the **most complex software ever developed**. Major advances include:

### Processes
A process can be defined as:
- A program in execution
- An instance of a running program
- An entity that can be assigned to and executed on a processor
- A unit of activity with a single sequential thread of execution, a current state, and associated system resources

**Three components of a process:**
1. An executable program
2. Associated data (variables, workspace, buffers)
3. **Execution context** ("process state") — the internal data the OS uses to supervise and control the process; includes register contents, process priority, and I/O wait status

**Causes of errors in multiprogramming environments:**
- **Improper synchronization** — signaling flaws cause data loss or duplication
- **Failed mutual exclusion** — multiple programs access a shared resource simultaneously
- **Deadlocks** — two or more programs hang waiting for each other
- **Nondeterminate program operation** — scheduling order affects program outcomes

---

### Memory Management

The OS has five principal storage management responsibilities:
1. Process isolation
2. Automatic allocation and management
3. Support of modular programming
4. Protection and access control
5. Long-term storage

**Virtual Memory:** Allows programs to address memory logically without regard to physically available RAM. Enables multiple jobs to reside in memory concurrently.

**Paging:** Divides processes into fixed-size blocks called **pages**.
- Program references memory via a **virtual address** (page number + offset)
- Pages can be located anywhere in physical memory
- A **Memory Management Unit (MMU)** translates virtual addresses to real (physical) addresses

---

### Information Protection and Security

Main security concerns:
- **Availability** — system resources are accessible when needed
- **Confidentiality** — unauthorized users cannot read data
- **Data integrity** — data cannot be altered without authorization
- **Authenticity** — identities of communicating parties are verified

---

### Scheduling and Resource Management

Resource allocation policies must balance:
- **Fairness** — give each process equal and fair access
- **Efficiency** — maximize system throughput
- **Differential responsiveness** — differentiate among classes of jobs with different requirements

---

## Modern OS Design Approaches

### Multithreading
- **Thread:** Dispatchable unit of work; executes sequentially and is interruptible
- **Process:** A collection of one or more threads and associated system resources
- Gives programmers greater control over modularity and timing

### Symmetric Multiprocessing (SMP)
- Multiple processors share the same main memory and I/O facilities
- All processors can perform the same functions; transparent to the user
- OS handles scheduling and synchronization across processors
- **Advantages:** Performance, Availability, Incremental growth, Scaling

### Microkernel Architecture
Assigns only essential functions to the kernel:
- Address spaces
- Interprocess communication (IPC)
- Basic scheduling

Other services run in user space as separate modules.

### Distributed Operating Systems
- Provides the illusion of a single unified system across multiple machines
- Single main memory space, single secondary storage space, unified access
- Still lags uniprocessor/SMP OS in maturity

### Object-Oriented Design
- Adds modular extensions to a small kernel
- Enables OS customization without disrupting system integrity
- Eases development of distributed tools and systems

---

## Summary

| Topic | Key Points |
|---|---|
| OS objectives | Convenience, efficiency, updatability |
| OS as interface | Between user applications and hardware |
| OS as resource manager | Manages processor, memory, I/O |
| Evolution | Serial → Simple Batch → Multiprogrammed Batch → Time-Sharing |
| Major achievements | Processes, memory management, security, scheduling |
| Modern approaches | Multithreading, SMP, microkernel, distributed, OO design |
