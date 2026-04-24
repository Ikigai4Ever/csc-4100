# Chapter 2 Notes

## Operating System

An **operating system** is a program that controls the execution of application programs, and is also an interface between applications and hardware.

A computer is a set of resources for the movement, storage, and processing of data, where the OS is responsible for managing these resources.

### OS Structure

<img width="420" height="248" alt="image" src="https://github.com/user-attachments/assets/1427181d-3159-44c8-af54-be67e2ac4a11" />

### OS Services

- Program development
- Program execution
- Access to I/O devices
- Controlled access to files
- System access
- Error detection and response
- Accounting

## Operating System as Software

The role of the OS is to be responsible for managing the resources for the computer (movement, storage, and processing of data).

Functions in the same way as ordinary computer software.

The program, or suite of programs, executed by the processor, and frequently relinquishes control and must depend on the processor to allow it to regain control.

## OS as a Resource Manager

<img width="463" height="440" alt="image" src="https://github.com/user-attachments/assets/2aea1e11-3b31-4c63-bdeb-1a3897e5e182" />

## Serial Processing

In the earliest computers, there were no operating systems. Programmers used to interact directly with the computer hardware, unlike today, when they interact through the OS. Computers were also run from a console with display lights, toggles, and others.

Some problems with this were with scheduling. Most installations used a hardcopy sign-up sheet to reserve computer time. Also, time allocations could run short or long.

## Simple Batch Systems

Early computers used to be very expensive because they needed to maximize processor utilization.

There is a monitor that the user no longer has direct access to the processor. In this, the job is submitted to a computer operator who batches them together and places them on an input device. The program will then branch back to the monitor when finished.

### Monitor Point of View

The **monitor** controls the sequence of events. The **resident monitor** is software always in memory. Then the monitor reads in the **job** and gives control, which then the job returns control to the monitor.

<img width="307" height="338" alt="image" src="https://github.com/user-attachments/assets/79f35882-bc24-4825-9658-c3043877384f" />

## Processor's Point of View

The processor executes instructions from the memory containing the monitor, which executes the instructions in the user program until it encounters an ending or error condition.

**control is passed to a job**: means the processor is fetching and executing instructions in a user program.

**control is returned to the monitor**: the processor is fetching and executing instructions from the monitor program.

## Desirable Hardware Features 

