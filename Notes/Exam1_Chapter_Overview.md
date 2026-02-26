# Exam 1 Chapter Overview

## Chapter 2: Operating System Overview

- Discuss the beautification principle (i.e. the operating system as a user/computer interface). What kind of services should the operating system provide? Describe the resource management principle (i.e. the operating system as an efficient resource manager). What is the kernel?
- Why do operating systems need to change over time? What affect does this have on design?
- Discuss how operating system have changed over the years. Describe serial processing. What was so bad about this model? Describe batch systems. What is a resident monitor? What is JCL? Describe multiprogrammed batch systems and how they were an improvement over simple batch systems in terms of efficiency. Describe time sharing.
- Discuss the major achievements in operating systems. In particularly, be able to describe processes and the problems they helped solve. What is process state?
- Discuss achievements in memory management. Discuss the five principle storage management responsibilities. What is a real address? A virtual address? Know what virtual memory is (but you do not have to know how it works; we'll cover that later).
- Know that advances for operating systems have occurred in information protection and security Be able to discuss areas of availability, confidentiality, data integrity, and authenticity.
- Discuss achievements scheduling and resource management. Discuss fairness, differential responsiveness, and efficiency.
- Discuss advances in system structure. Describe a layered architecture.  Discuss the difference between a monolithic kernel and a microkernel. Know the difference between a thread and a process.
- See Review Questions 2.1 through 2.11. I could ask you to do problems from Section 2.13 Problems like 2.1, 2.2, 2.3, 2.4 (8th edition):

2.1 What are three objectives of an OS design?
2.2 What is the kernel of an OS?
2.3 What is multiprogramming?
2.4 What is a process?
2.5 How is the execution context of a process used by the OS?
2.6 List and briefly explain five storage management responsibilities of a typical OS.
2.7 Explain the distinction between a real address and a virtual address.
2.8 Describe the round-robin scheduling technique.
2.9 Explain the difference between a monolithic kernel and a microkernel.
2.10 What is multithreading?
2.11 List the key design issues for an SMP operating system.

2.1 Suppose that we have a multiprogrammed computer in which each job has identical characteristics. In one computation period, T, for a job, half the time is spent in I/O and the other half in processor activity. Each job runs for a total of N periods. Assume that a simple round-robin scheduling is used, and that I/O operations can overlap with processor operation. Define the following quantities:
  * Turnaround time = actual time to complete a job
  * Throughput = average number of jobs completed per time period T
  * Processor utilization = percentage of time that the processor is active (not waiting)
- Compute these quantities for one, two, and four simultaneous jobs, assuming that the period T is distributed in each of the following ways:
  a. I/O first half, processor second half
  b. I/O first and fourth quarters, processor second and third quarter

2.2 An I/O-bound program is one that, if run alone, would spend more time waiting for
- I/O than using the processor. A processor-bound program is the opposite. Suppose a
- short-term scheduling algorithm favors those programs that have used little processor
- time in the recent past. Explain why this algorithm favors I/O-bound programs and
- yet does not permanently deny processor time to processor-bound programs.

2.3 Contrast the scheduling policies you might use when trying to optimize a time-sharing system with those you would use to optimize a multiprogrammed batch system.

2.4 What is the purpose of system calls, and how do system calls relate to the OS and to the concept of dual-mode (kernel-mode and user-mode) operation?

## Chapter 3: Process description and control

- What are the components of a process? What is a PCB? What kinds of things are found in a PCB?
- What is a dispatcher. Describe the interleaving of instructions in a multiprogramming OS. Describe the two state process model. What is the purpose of a ready queue?
- Describe process creation and termination. Describe the five state process model. Describe the running, ready, blocked, new, and exit process states. Be able to describe the transitions that a process can make in the five state process model. Be able to describe the queuing model for the five state process model.
- Describe the seven state process model. In particular, describe the ready/suspend and blocked/suspend state and why they are needed. Be able to describe the transitions that a process can make in the seven state process model.
- Discuss the different type of control structures that the OS can use to manage processes and resources (memory tables, I/O tables, file tables, and process tables).
- What is the process image? Be familiar with the elements of a PCB as shown on page 130. Know about the EFLAGS register of the i386 family of processors (but you don't have to know the individual flags).
- Understand the different modes of execution: user mode and kernel mode.
- Understand the steps of process creation. Discuss when process switching can occur (clock interrupt, I/O interrupt, memory fault, supervisor call, trap).
- Make sure that you understand the steps that occur during process switching.
- Describe what happens during process switching. Be able to answer the question "Is the operating system a process?" and discuss the issues involved. See the Review Questions 3.1 through 3.14. I could ask you to do problems like: 3.1, 3.3, 3.4, 3.10. (8th edition):

3.1 What is an instruction trace?

3.2 What common events lead to the creation of a process?

3.3 For the processing model of Figure 3.6, briefly define each state.

3.4 What does it mean to preempt a process?

3.5 What is swapping and what is its purpose?

3.6 Why does Figure 3.9b have two blocked states?

3.7 List four characteristics of a suspended process.

3.8 For what types of entities does the OS maintain tables of information for management purposes?

3.9 List three general categories of information in a process control block.

3.10 Why are two modes (user and kernel) needed?

3.11 What are the steps performed by an OS to create a new process?

3.12 What is the difference between an interrupt and a trap?

3.13 Give three examples of an interrupt.

3.14 What is the difference between a mode switch and a process switch?

3.1 The following state transition table is a simplified model of process management, with
the labels representing transitions between states of READY, RUN, BLOCKED, and
NONRESIDENT.

State Transitions

<img width="669" height="158" alt="image" src="https://github.com/user-attachments/assets/96133b17-e8fe-4c12-b925-570e563864bc" />

Give an example of an event that can cause each of the above transitions. Draw a
diagram if that helps.

3.3 Figure 3.9b contains seven states. In principle, one could draw a transition between
any two states, for a total of 42 different transitions.
a. List all of the possible transitions and give an example of what could cause each
transition.
b. List all of the impossible transitions and explain why.

3.4 For the seven-state process model of Figure 3.9b, draw a queueing diagram similar to
that of Figure 3.8b.

3.10 In a number of early computers, an interrupt caused the register values to be stored in
fixed locations associated with the given interrupt signal. Under what circumstances
is this a practical technique? Explain why it is inconvenient in general.
