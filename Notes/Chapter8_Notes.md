# Chapter 8: Virtual Memory
*Operating Systems: Internals and Design Principles — Eighth Edition, William Stallings*

---

## Virtual Memory Terminology

| Term | Description |
|------|-------------|
| **Virtual memory** | A storage allocation scheme in which secondary memory can be addressed as though it were part of main memory. The addresses a program may use to reference memory are distinguished from the addresses the memory system uses to identify physical storage sites, and program-generated addresses are translated automatically to the corresponding machine addresses. The size of virtual storage is limited by the addressing scheme of the computer system and by the amount of secondary memory available, and not by the actual number of main storage locations. |
| **Virtual address** | The address assigned to a location in virtual memory to allow that location to be accessed as though it were part of main memory. |
| **Virtual address space** | The virtual storage assigned to a process. |
| **Address space** | The range of memory addresses available to a process. |
| **Real address** | The address of a storage location in main memory. |

---

## Hardware and Control Structures

Two characteristics fundamental to memory management:

1. All memory references are logical addresses that are dynamically translated into physical addresses at run time
2. A process may be broken up into a number of pieces that don't need to be contiguously located in main memory during execution

If these two characteristics are present, it is not necessary that all of the pages or segments of a process be in main memory during execution.

---

## Execution of a Process

### Part 1
- Operating system brings into main memory a few pieces of the program
- **Resident set** — portion of process that is in main memory
- An interrupt is generated when an address is needed that is not in main memory
- Operating system places the process in a blocking state

### Part 2
- The piece of the process that contains the logical address is brought into main memory
  - Operating system issues a disk I/O Read request
  - Another process is dispatched to run while the disk I/O takes place
  - An interrupt is issued when disk I/O is complete, which causes the operating system to place the affected process in the Ready state

---

## Implications

- More processes may be maintained in main memory
  - Only load in some of the pieces of each process
  - With so many processes in main memory, it is very likely a process will be in the Ready state at any particular time
- A process may be larger than all of main memory

---

## Real and Virtual Memory

| Type | Description |
|------|-------------|
| **Real memory** | Main memory — the actual RAM |
| **Virtual memory** | Memory on disk — allows for effective multiprogramming and relieves the user of tight constraints of main memory |

---

## Characteristics of Paging and Segmentation

| Method | Partitioning | Partitioner | Internal Frag | External Frag | Data Structure | Free List | Storage | Replacement |
|--------|-------------|-------------|---------------|---------------|----------------|-----------|---------|-------------|
| **Simple Paging** | Main memory partitioned into small fixed-size chunks called frames | Program broken into pages by the compiler or memory management system | Internal fragmentation within frames | No external fragmentation | OS must maintain a page table for each process showing which frame each page occupies | OS must maintain a free frame list | All pages of a process must be in main memory for process to run, unless overlays are used | N/A |
| **Virtual Memory Paging** | Same as simple paging | Same as simple paging | Same as simple paging | Same as simple paging | Same as simple paging | Same as simple paging | Not all pages need be in main memory; pages may be read in as needed | Reading a page into main memory may require writing a page out to disk |
| **Simple Segmentation** | Main memory not partitioned | Program segments specified by the programmer to the compiler | No internal fragmentation | External fragmentation | OS must maintain a segment table for each process showing load address and length | OS must maintain a list of free holes in main memory | All segments of a process must be in main memory for process to run, unless overlays are used | N/A |
| **Virtual Memory Segmentation** | Same as simple segmentation | Same as simple segmentation | Same as simple segmentation | Same as simple segmentation | Same as simple segmentation | Same as simple segmentation | Not all segments need be in main memory; segments may be read in as needed | Reading a segment into main memory may require writing one or more segments out to disk |

---

## Thrashing

A state in which the system spends most of its time swapping process pieces rather than executing instructions. To avoid this, the operating system tries to guess, based on recent history, which pieces are least likely to be used in the near future.

---

## Principle of Locality

- Program and data references within a process tend to cluster
- Only a few pieces of a process will be needed over a short period of time
- Therefore, it is possible to make intelligent guesses about which pieces will be needed in the future
- Avoids thrashing

---

## Support Needed for Virtual Memory

For virtual memory to be practical and effective:

- Hardware must support paging and segmentation
- Operating system must include software for managing the movement of pages and/or segments between secondary memory and main memory

---

## Paging

- The term *virtual memory* is usually associated with systems that employ paging
- Use of paging to achieve virtual memory was first reported for the Atlas computer
- Each process has its own page table
  - Each page table entry contains the frame number of the corresponding page in main memory

### Memory Management Formats

Three formats for virtual address translation:

**Paging only** — Virtual address contains: Page Number | Offset. Page Table Entry contains: P | M | Other Control Bits | Frame Number.

**Segmentation only** — Virtual address contains: Segment Number | Offset. Segment Table Entry contains: P | M | Other Control Bits | Length | Segment Base.

**Combined segmentation and paging** — Virtual address contains: Segment Number | Page Number | Offset. Segment Table Entry contains Control Bits, Length, and Segment Base. Page Table Entry contains: P | M | Other Control Bits | Frame Number. *(P = present bit, M = modified bit)*

---

## Translation Lookaside Buffer (TLB)

Each virtual memory reference can cause two physical memory accesses:
- One to fetch the page table entry
- One to fetch the data

To overcome the effect of doubling the memory access time, most virtual memory schemes make use of a special high-speed cache called a **Translation Lookaside Buffer**.

### Associative Mapping

- The TLB only contains some of the page table entries, so we cannot simply index into the TLB based on page number
  - Each TLB entry must include the page number as well as the complete page table entry
- The processor is equipped with hardware that allows it to interrogate simultaneously a number of TLB entries to determine if there is a match on page number

### Paging Flowchart (Summary)

1. CPU checks the TLB
2. If page table entry is in TLB (TLB hit) → update TLB → CPU generates physical address
3. If not in TLB (TLB miss) → access page table
   - If page is in main memory → update TLB → CPU generates physical address
   - If page is NOT in main memory → **Page Fault** handling routine:
     - OS instructs CPU to read the page from disk
     - CPU activates I/O hardware
     - Page transferred from disk to main memory
     - If memory is full → perform page replacement → update page tables
     - Return to faulted instruction

---

## Page Size

- The smaller the page size, the lesser the amount of internal fragmentation
  - However, more pages are required per process
  - More pages per process means larger page tables
  - For large programs in a heavily multiprogrammed environment, some portion of the page tables of active processes must be in virtual memory instead of main memory
- The physical characteristics of most secondary-memory devices favor a larger page size for more efficient block transfer of data

### Example Page Sizes

| Processor | Standard Page Size | Larger Page Sizes |
|-----------|-------------------|-------------------|
| **Intel** | 4 KB | 2 MB, 1 GB |
| **ARM64** | 4 KB | 16 KB, 64 KB |

---

## Address Translation: Combined Segmentation and Paging

In a combined segmentation/paging system, the virtual address contains three parts: Segment # | Page # | Offset. Address translation proceeds through two levels — first the segment table (using seg #) to find the page table base, then the page table (using page #) to find the frame number, which is combined with the offset to produce the final physical address.

---

## Operating System Software

The design of the memory management portion of an operating system depends on three fundamental areas of choice:

- Whether or not to use virtual memory techniques
- The use of paging or segmentation or both
- The algorithms employed for various aspects of memory management

---

## Fetch Policy

Determines when a page should be brought into memory.

| Policy | Description |
|--------|-------------|
| **Demand Paging** | Only brings pages into main memory when a reference is made to a location on the page. Many page faults occur when a process is first started; the principle of locality suggests page faults should drop to a very low level as more pages are brought in. |
| **Prepaging** | Pages other than the one demanded by a page fault are brought in. Exploits the characteristics of most secondary memory devices — if pages are stored contiguously in secondary memory, it is more efficient to bring in several at once. Ineffective if extra pages are not referenced. Should not be confused with "swapping." |

---

## Replacement Policy

- Deals with the selection of a page in main memory to be replaced when a new page must be brought in
- Objective is that the page removed be the page least likely to be referenced in the near future
- The more elaborate the replacement policy, the greater the hardware and software overhead to implement it

### Page Replacement Algorithms

#### Optimal (OPT)
Replaces the page that will not be referenced for the longest time in the future. Used as a benchmark — not implementable in practice since it requires future knowledge.

#### Least Recently Used (LRU)
- Replaces the page that has not been referenced for the longest time
- By the principle of locality, this should be the page least likely to be referenced in the near future
- Difficult to implement
  - One approach is to tag each page with the time of last reference
  - This requires a great deal of overhead

#### First-in-First-out (FIFO)
- Treats page frames allocated to a process as a circular buffer
- Pages are removed in round-robin style — simple replacement policy to implement
- The page that has been in memory the longest is replaced

#### Clock Policy
- Requires the association of an additional bit with each frame — referred to as the **use bit**
- When a page is first loaded in memory or referenced, the use bit is set to 1
- The set of frames is considered to be a circular buffer
- Any frame with a use bit of 1 is passed over by the algorithm
- Page frames are visualized as laid out in a circle

### Page Replacement Algorithm Comparison (Example)

Page address stream: **2 3 2 1 5 2 4 5 3 2 5 2**

| Algorithm | Page Faults |
|-----------|------------|
| OPT | 3 |
| LRU | 4 |
| FIFO | 6 |
| CLOCK | 5 |

> OPT produces the fewest page faults but is not implementable. CLOCK approximates LRU with lower overhead, making it the most practical choice.

### Performance Comparison

When comparing fixed-allocation local page replacement algorithms by page fault rate vs. number of frames allocated, the performance ranking from best to worst is: **OPT → LRU → CLOCK → FIFO**. All algorithms converge as more frames are allocated, with higher frame counts producing fewer page faults across the board.
