# Chapter 3: Process Description and Control
*Operating Systems: Internals and Design Principles — William Stallings, 8th Edition*

---

## Background

- A computer platform consists of a collection of hardware resources
- Applications are developed to perform tasks, but writing them directly for hardware is inefficient
- The **OS** provides a convenient, feature-rich, secure, and consistent interface for applications
- The OS provides a uniform, abstract representation of resources that can be requested and accessed by applications

### OS Management of Application Execution
- Resources are made available to multiple applications
- The processor is switched among multiple applications so all will appear to be progressing
- The processor and I/O devices can be used efficiently

---

## What is a Process?

A process consists of two essential elements:
1. **Program code** — may be shared with other processes executing the same program
2. **A set of data** associated with that code

When the processor executes the code, this executing entity is called a **process**.

### Process Elements (while executing)
| Element | Element |
|---|---|
| Identifier | State |
| Priority | Program Counter |
| Memory Pointers | Context Data |
| I/O Status Information | Accounting Information |

---

## Process Control Block (PCB)

- Contains all the process elements
- Allows a running process to be interrupted and later resumed as if the interruption had not occurred
- Created and managed by the operating system
- Key tool that allows support for multiple processes

**PCB fields:** Identifier, State, Priority, Program Counter, Memory Pointers, Context Data, I/O Status Information, Accounting Information

---

## Process States

### Trace
- The behavior of an individual process described by listing the sequence of instructions that execute
- The behavior of the processor can be characterized by showing how the traces of various processes are interleaved

### Dispatcher
- A small program that switches the processor from one process to another

---

## Two-State Process Model

A process is either **Running** or **Not Running**.

```
Enter --> [Not Running] --Dispatch--> [Running] --> Exit
              ^                           |
              |----------Pause-----------|
```

In the queueing model, processes enter a queue, get dispatched to the processor, then either exit or pause back into the queue.

---

## Process Creation and Termination

### Reasons for Process Creation
| Reason | Description |
|---|---|
| New batch job | OS reads next sequence of job control commands |
| Interactive logon | User logs on to the system at a terminal |
| OS service | OS creates a process to perform a function on behalf of a user program |
| Spawned by existing process | A user program creates additional processes for modularity or parallelism |

### Process Spawning
- **Process spawning** occurs when the OS creates a process at the explicit request of another process
- **Parent process** — the original, creating process
- **Child process** — the new process

### Reasons for Process Termination
| Reason | Description |
|---|---|
| Normal completion | Process executes an OS service call indicating it has finished |
| Time limit exceeded | Process has run longer than the specified total time limit |
| Memory unavailable | Process requires more memory than the system can provide |
| Bounds violation | Process tries to access a memory location it is not allowed to access |
| Protection error | Process attempts to use a resource it is not allowed to use |
| Arithmetic error | Process tries a prohibited computation (e.g., division by zero) |
| Time overrun | Process waited longer than a specified maximum for an event |
| I/O failure | Error occurs during input or output |
| Invalid instruction | Process attempts to execute a nonexistent instruction |
| Privileged instruction | Process attempts to use an instruction reserved for the OS |
| Data misuse | A piece of data is the wrong type or is not initialized |
| Operator/OS intervention | Operator or OS terminates the process (e.g., deadlock exists) |
| Parent termination | OS may automatically terminate all offspring of a terminated parent |

---

## Five-State Process Model

```
[New] --Admit--> [Ready] --Dispatch--> [Running] --Release--> [Exit]
                    ^        <--Timeout--    |
                    |                        |
               Event Occurs            Event Wait
                    |                        |
                    +------- [Blocked] <-----+
```

**States:**
- **New** — Process has just been created
- **Ready** — Process is prepared to execute when given the opportunity
- **Running** — Process is currently being executed
- **Blocked** — Process cannot execute until some event occurs (e.g., I/O completion)
- **Exit** — Process has been released from the pool of executable processes

The 5-state queueing model uses a **Ready Queue** and one or more **Blocked Queues** (one per event type).

---

## Suspended Processes

### Swapping
- Involves moving part or all of a process from main memory to disk
- When no processes in main memory are in the Ready state, the OS **swaps** one blocked process out to disk into a **suspend queue**
- The suspend queue holds existing processes temporarily removed from main memory
- The OS then brings in another process from the suspend queue or honors a new-process request
- Swapping is an I/O operation — because disk I/O is generally the fastest I/O on a system, swapping usually enhances performance

### Characteristics of a Suspended Process
- Not immediately available for execution
- May or may not be waiting on an event
- Was placed in suspended state by an agent (itself, parent process, or OS)
- May not be removed from this state until the agent explicitly orders removal

### Reasons for Process Suspension
| Reason | Description |
|---|---|
| Swapping | OS needs to release sufficient main memory |
| Other OS reason | OS may suspend a background/utility process or a suspect process |
| Interactive user request | User wishes to suspend execution for debugging |
| Timing | Process executed periodically and suspended while waiting for next interval |
| Parent process request | Parent wishes to examine or modify the descendant process |

### 6-State and 7-State Models
- **6-State** adds one **Suspend** state (blocked processes can be suspended to disk)
- **7-State** adds two suspend states: **Ready/Suspend** and **Blocked/Suspend**
  - Allows new processes to be admitted directly into the Ready/Suspend state
  - Allows blocked/suspended processes to activate directly into Blocked or Ready

---

## Process Description

### OS Control Structures (Tables)
The OS maintains four categories of tables:

| Table Type | Purpose |
|---|---|
| **Memory Tables** | Track allocation of main and secondary memory to processes |
| **I/O Tables** | Manage I/O devices and channels; track status of I/O operations |
| **File Tables** | Track file existence, location on secondary memory, current status, and attributes |
| **Process Tables** | Manage processes; reference memory, I/O, and files |

### Memory Tables
Must include:
- Allocation of main memory to processes
- Allocation of secondary memory to processes
- Protection attributes of blocks of main or virtual memory
- Information needed to manage virtual memory

### Process Image
The collection of program, data, stack, and attributes is the **process image**.

| Component | Description |
|---|---|
| User Data | Modifiable part of user space; includes program data, user stack area, and modifiable programs |
| User Program | The program to be executed |
| Stack | LIFO stack(s) used to store parameters and calling addresses for procedure and system calls |
| Process Control Block | Data needed by the OS to control the process |

---

## Process Control Block (PCB) — Detailed

### Process Identification
- Unique numeric identifier for the process
- Identifier of the parent process
- User identifier
- Used to cross-reference memory, I/O, and file tables

### Processor State Information
Consists of contents of processor registers:
- **User-Visible Registers** — referenced by machine language; typically 8–32 registers
- **Control and Status Registers** — including:
  - *Program Counter* — address of next instruction to be fetched
  - *Condition Codes* — result of most recent arithmetic/logical operation
  - *Status Information* — interrupt enabled/disabled flags, execution mode
- **Stack Pointers** — point to top of LIFO system stack(s)

**Program Status Word (PSW)** contains condition codes plus other status information. The **EFLAGS** register is an example used by any OS on an x86 processor.

### Process Control Information
Additional information needed by the OS:
- **Scheduling and State Information** — process state, priority, scheduling-related info, event identity
- **Data Structuring** — pointers linking processes in queues, rings, or other structures; parent-child relationships
- **Interprocess Communication** — flags, signals, and messages between independent processes
- **Process Privileges** — memory that may be accessed, types of instructions that may be executed
- **Memory Management** — pointers to segment and/or page tables
- **Resource Ownership and Utilization** — resources controlled by the process; history of processor utilization

### Role of the PCB
- Most important data structure in an OS
- Read and/or modified by virtually every module in the OS
- Defines the state of the OS
- A bug in a single routine could damage PCBs and destroy the system's ability to manage affected processes

---

## Process Control

### Modes of Execution
| User Mode | System (Kernel) Mode |
|---|---|
| Less-privileged | More-privileged |
| User programs execute here | Also called control mode or kernel mode |
| — | Kernel of the OS executes here |

### Typical OS Kernel Functions
- **Process Management** — creation/termination, scheduling/dispatching, switching, synchronization, IPC, managing PCBs
- **Memory Management** — address space allocation, swapping, page and segment management
- **I/O Management** — buffer management, allocation of I/O channels and devices, interrupt handling, accounting/monitoring

### Steps for Process Creation
When the OS decides to create a new process, it:
1. Assigns a unique process identifier to the new process
2. Allocates space for the process
3. Initializes the process control block
4. Sets the appropriate linkages
5. Creates or expands other data structures

### Mechanisms for Interrupting a Process
| Mechanism | Cause | Use |
|---|---|---|
| **Interrupt** | External to the current instruction | Reaction to an asynchronous external event |
| **Trap** | Associated with the current instruction | Handling an error or exception condition |
| **Supervisor Call** | Explicit request | Call to an OS function |

### System Interrupts
- **Interrupt** — due to an event external to and independent of the currently running process (e.g., clock interrupt, I/O interrupt, memory fault)
- **Time Slice** — the maximum time a process can execute before being interrupted
- **Trap** — an error or exception generated within the currently running process; OS determines if fatal

### Mode Switching
- **No interrupts pending** → processor fetches the next instruction of the current process
- **Interrupt pending** → processor:
  1. Sets the program counter to the starting address of an interrupt handler
  2. Switches from user mode to kernel mode

### Process Switch — Steps for Change of Process State
1. Save the context of the processor
2. Update the PCB of the currently Running process
3. Move that PCB to the appropriate queue (Ready, Blocked, etc.)
4. Select another process for execution
5. Update the PCB of the selected process
6. Update memory management data structures
7. Restore the processor context to that of the selected process

---

## Execution of the Operating System

Three approaches (Figure 3.15):

| Approach | Description |
|---|---|
| **(a) Separate Kernel** | OS executes outside of all processes; kernel is a separate entity |
| **(b) OS within User Processes** | OS functions execute within the context of a user process; process switching functions are separate |
| **(c) OS as Processes** | OS functions execute as separate OS processes alongside user processes |

---

## Summary

| Topic | Key Concepts |
|---|---|
| Process | Program code + data + PCB |
| PCB | Most important OS data structure; contains all info to manage a process |
| Two-State Model | Running / Not Running |
| Five-State Model | New, Ready, Running, Blocked, Exit |
| Suspended Processes | 6-State and 7-State models; swapping to disk |
| OS Tables | Memory, I/O, File, Process tables |
| Process Image | User data, user program, stack, PCB |
| Modes | User mode vs. kernel mode |
| Process Creation | 5 steps: assign ID, allocate space, init PCB, set linkages, create data structures |
| Process Switch | 7 steps involving saving/restoring context and updating PCBs |
