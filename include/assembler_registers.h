/** x86 assembler
 * https://www.x.com/7etsuo
 */

#ifndef REGISTERS_H
#define REGISTERS_H

#define REG_GP      0   
#define REG_POINTER 1   
#define REG_SEGMENT 2   

/* **************** General Purpose Registers
 * Note: you cannot access AH, BH, CH and DH when using the REX.W instruction prefix.
 * This prefix is added (automatically by assemblers) when an operand contains a 64-bit register.
 *
 * ======================================================================================================
 * Monikers                                                                 | Description
 * ======================================================================================================
 * 64   |    32     |  16      | 8 high bits of lower 16 bits  | 8 - bit    |
 * ======================================================================================================
 * RAX	|   EAX     |     AX    |  AH                           | AL	    | Accumulator
 * RBX	|   EBX     |     BX    |  BH                           | BL	    | Base
 * RCX	|   ECX     |     CX    |  CH                           | CL	    | Counter
 * RDX	|   EDX     |     DX    |  DH                           | DL	    | Data(commonly extends the A register)
 * RSI	|   ESI     |     SI    |  NA                           | SIL	    | Source index for string operations
 * RDI	|   EDI     |     DI    |  NA                           | DIL	    | Destination index for string operations
 * RSP	|   ESP     |     SP    |  NA                           | SPL	    | Stack Pointer
 * RBP	|   EBP     |     BP    |  NA                           | BPL	    | Base Pointer(meant for stack frames)
 * R8	|   R8D     |    R8W    |  NA                           | R8B	    | General purpose
 * R9	|   R9D     |    R9W    |  NA                           | R9B	    | General purpose
 * R10	|   R10D    | 	R10W    |  NA                           | R10B	    | General purpose
 * R11	|   R11D    |	R11W    |  NA                           | R11B	    | General purpose
 * R12	|   R12D    |	R12W    |  NA                           | R12B	    | General purpose
 * R13	|   R13D    |	R13W    |  NA                           | R13B	    | General purpose
 * R14	|   R14D    |	R14W    |  NA                           | R14B	    | General purpose
 * R15	|   R15D    |	R15W    |  NA                           | R15B	    | General purpose
 * ======================================================================================================
 */

enum ENUM_REG_32 { EAX, ECX, EDX, EBX, ESP, EBP, ESI, EDI };
enum ENUM_REG_16 { AX, CX, DX, BX, SP, BP, SI, DI };
enum ENUM_REG8 { AL, CL, DL, BL, AH, CH, DH, BH };
enum ENUM_REG_SEGMENT { CS, DS, SS, ES, FS, GS };

extern const char* reg_32[];
extern const char* reg_16[];
extern const char* reg_8[];


/* **************** Pointer Registers
 * Note: The instruction pointer can only be used in RIP - relative addressing, which was introduced with long mode.
 *
 * ======================================================================================================
 * Monikers	                    | Description
 * ======================================================================================================
 * 64-bit	 |  32-bit | 16-bit |
 * ======================================================================================================
 * RIP	     |  EIP	  | IP	    | Instruction Pointer
 * ======================================================================================================
 */


/* **************** Segment Registers
  * Note: All these are 16 bits long.
  * Segments of CS, DS, ES, and SS are treated as if their base was 0 no matter what the segment descriptors in the GDT say.
  * Exceptions are FS and GS which have MSRs to change their base.
  * Limit checks are disabled for all segments.
  *
  * ======================================================================================================
  * Monikers     |   Description
  * ======================================================================================================
  * CS	        |   Code Segment
  * DS	        |   Data Segment
  * SS	        |   Stack Segment
  * ES	        |   Extra Segment(used for string operations)
  * FS	        |   General - purpose Segment
  * GS	        |   General - purpose Segment
  * ======================================================================================================
  */

/* **************** RFLAGS Register
  * Note: The lower 32 bits of RFLAGS is the same as EFLAGS.
  *
  *======================================================================================================
  *   Bit(s)     |  Label	| Description
  * =====================================================================================================
  *       0      |   CF      | Carry Flag
  *       1      |    1      | Reserved
  *       2      |   PF      | Parity Flag
  *       3      |    0      | Reserved
  *       4      |   AF      | Auxiliary Carry Flag
  *       5      |    0      | Reserved
  *       6      |   ZF      | Zero Flag
  *       7      |   SF      | Sign Flag
  *       8      |   TF      | Trap Flag
  *       9      |   IF      | Interrupt Enable Flag
  *      10      |   DF      | Direction Flag
  *      11      |   OF      | Overflow Flag
  *      12      |   13      | IOPL	I/O Privilege Level
  *      14      |   NT      | Nested Task
  *      15      |    0      | Reserved
  *      16      |   RF      | Resume Flag
  *      17      |   VM      | Virtual-8086 Mode
  *      18      |   AC      | Alignment Check / Access Control
  *      19      |  VIF      | Virtual Interrupt Flag
  *      20      |  VIP      | Virtual Interrupt Pending
  *      21      |   ID      | ID Flag
  *   22-63      |    0      | Reserved
  * ======================================================================================================
  */
// Control Registers
/* **************** CR0
  * Note: that this register is the only control register that can be written and read via 2 ways
  *
  * ======================================================================================================
  *  Bit(s)  |   Label |	Description
  * ======================================================================================================
  *  0	    |    PE	  |  Protected Mode Enable
  *  1	    |    MP	  |  Monitor Co-Processor
  *  2	    |    EM	  |  Emulation
  *  3	    |    TS	  |  Task Switched
  *  4	    |    ET	  |  Extension Type
  *  5	    |    NE	  |  Numeric Error
  *  6-15    |    0	  |  Reserved
  *  16	    |    WP	  |  Write Protect
  *  17	    |    0	  |  Reserved
  *  18	    |    AM	  |  Alignment Mask
  *  19-28   |    0	  |  Reserved
  *  29	    |    NW	  |  Not-Write Through
  *  30	    |    CD	  |  Cache Disable
  *  31	    |    PG	  |  Paging
  *  32-63   |    0	  |  Reserved
  * ======================================================================================================
  */

/* **************** CR2
  * This control register contains the linear (virtual) address which triggered a page fault, available in the page fault's interrupt handler.
  *
  * ======================================================================================================
  */

/* **************** CR3
  * Note: Note that this must be page aligned
  *
  *======================================================================================================
  * Bit(s)|          | Label | Description	                  | Condition
  * ======================================================================================================
  * 0-11	 |   0-2    |   0   | Reserved                      | CR4.PCIDE = 0
  *       |     3    | PWT   | Page-Level Write Through      |
  *       |     5    | PCD   | Page-Level Cache Disable      |
  *       |  5-11    |   0   | Reserved                      |
  * ================ | ===================================== |
  *  0-11            | PCID                                  | CR4.PCIDE = 1
  * 12-63            | Physical Base Address of the PML4     |
  * ======================================================================================================
  */

/* **************** CR4
  *
  * ======================================================================================================
  *  Bit(s) |     Label       |  Description
  * ======================================================================================================
  *  0	  |        VME      |  Virtual-8086 Mode Extensions
  *  1	  |        PVI      |  Protected Mode Virtual Interrupts
  *  2	  |        TSD      |  Time Stamp enabled only in ring 0
  *  3	  |         DE      |  Debugging Extensions
  *  4	  |        PSE      |  Page Size Extension
  *  5	  |        PAE      |  Physical Address Extension
  *  6	  |        MCE      |  Machine Check Exception
  *  7	  |        PGE      |  Page Global Enable
  *  8	  |        PCE      |  Performance Monitoring Counter Enable
  *  9	  |     OSFXSR      |  OS support for fxsave and fxrstor instructions
  *  10	  | OSXMMEXCPT      |  OS Support for unmasked simd floating point exceptions
  *  11	  |       UMIP      |  User-Mode Instruction Prevention (SGDT, SIDT, SLDT, SMSW, and STR are disabled in user mode)
  *  12	  |          0      |  Reserved
  *  13	  |       VMXE      |  Virtual Machine Extensions Enable
  *  14	  |       SMXE      |  Safer Mode Extensions Enable
  *  15	  |          0      |  Reserved
  *  17	  |      PCIDE      |  PCID Enable
  *  18	  |    OSXSAVE      |  XSAVE And Processor Extended States Enable
  *  19	  |          0      |  Reserved
  *  20	  |       SMEP      |  Supervisor Mode Executions Protection Enable
  *  21	  |       SMAP      |  Supervisor Mode Access Protection Enable
  *  22	  |        PKE      |  Enable protection keys for user-mode pages
  *  23	  |        CET      |  Enable Control-flow Enforcement Technology
  *  24	  |        PKS      |  Enable protection keys for supervisor-mode pages
  *  25-63  |          0      |  Reserved
  * ======================================================================================================
  */

/* **************** CR8
  * CR8 is a new register accessible in 64-bit mode using the REX prefix.
  * CR8 is used to prioritize external interrupts and is referred to as the task-priority register (TPR).
  * The AMD64 architecture allows software to define up to 15 external interrupt-priority classes.
  * Priority classes are numbered from 1 to 15, with priority-class 1 being the lowest and priority-class 15 the highest.
  * CR8 uses the four low-order bits for specifying a task priority and the remaining 60 bits are reserved and must be written with zeros.
  * System software can use the TPR register to temporarily block low-priority interrupts from interrupting a high-priority task.
  * This is accomplished by loading TPR with a value corresponding to the highest-priority interrupt that is to be blocked.
  * For example, loading TPR with a value of 9 (1001b) blocks all interrupts with a priority class of 9 or less,
  * while allowing all interrupts with a pSegment Registersriority class of 10 or more to be recognized.
  * Loading TPR with 0 enables all external interrupts.
  * Loading TPR with 15 (1111b) disables all external interrupts.
  * The TPR is cleared to 0 on reset.
  *
  * ======================================================================================================
  *   Bit	  |   Purpose
  * ======================================================================================================
  *   0-3	  |  Priority
  *   4-63  |	 Reserved
  * ======================================================================================================
  */

/* **************** CR1, CR5-7, CR9-15
  * Reserved, the cpu will throw a #ud exeption when trying to access them.
  *
  * ======================================================================================================
  */

// MSRs

/* **************** IA32_EFER
  * Extended Feature Enable Register (EFER) is a model-specific register added in the AMD K6 processor,
  * to allow enabling the SYSCALL/SYSRET instruction, and later for entering and exiting long mode.
  * This register becomes architectural in AMD64 and has been adopted by Intel. Its MSR number is 0xC0000080.
  *
  * ======================================================================================================
  * Bit(s)    | Label     | Description
  * ======================================================================================================
  *     0     |   SCE     | System Call Extensions
  *   1-7     |     0     | Reserved
  *     8     |   LME     | Long Mode Enable
  *    10     |   LMA     | Long Mode Active
  *    11     |   NXE     | No-Execute Enable
  *    12     |  SVME     | Secure Virtual Machine Enable
  *    13     | LMSLE     | Long Mode Segment Limit Enable
  *    14     | FFXSR     | Fast FXSAVE/FXRSTOR
  *    15     |   TCE     | Translation Cache Extension
  * 16-63     |     0     | Reserved
  * ======================================Segment Registers================================================================
  */

/*
  *  SWAPGS Exchanges current GS base register value with value in MSR address 0xC0000102 (REG_KERNEL_GS_BASE)
  *  SYSCALL Fast call to privilege level 0 system procedures.
  *  SYSRET Return from fast systemcall.
  */

#define REG_MSR_EFER        0xC0000080  // SYSCALL/SYSRET
#define REG_MSR_FS_BASE     0xC0000100  // base addresses of the FS segment register 
#define REG_MSG_GS_BASE     0xC0000101  // base addresses of the GS segment registers
#define REG_KERNEL_GS_BASE  0xC0000102  // used to seperate kernel and user use of the GS register. 
// A buffer that gets exchanged with GS.base after a swapgs instruction. 

// Debug Registers

/* **************** DR0 - DR3
  * Note: Contain linear addresses of up to 4 breakpoints. If paging is enabled, they are translated to physical addresses.
  *
  * ======================================================================================================
  */

/* **************** DR6
  * Note: It permits the debugger to determine which debug conditions have occured.
  * When an enabled debug exception is enabled, low order bits 0-3 are set before entering debug exception handler.
  *
  * ======================================================================================================
  */

/* **************** DR7
  * A local breakpoint bit deactivates on hardware task switches, while a global does not.
  * 00b condition means execution break, 01b means a write watchpoint, and 11b means an R/W watchpoint. 10b is reserved for I/O R/W (unsupported).
  *
  * ======================================================================================================
  * Bit	   | Description
  * ======================================================================================================
  * 0	   | Local DR0 Breakpoint
  * 1	   | Global DR0 Breakpoint
  * 2	   | Local DR1 Breakpoint
  * 3	   | Global DR1 Breakpoint
  * 4	   | Local DR2 Breakpoint
  * 5	   | Global DR2 Breakpoint
  * 6	   | Local DR3 Breakpoint
  * 7	   | Global DR3 Breakpoint
  * 16-17   | Conditions for DR0
  * 18-19   | Size of DR0 Breakpoint
  * 20-21   | Conditions for DR1
  * 22-23   | Size of DR1 Breakpoint
  * 24-25   | Conditions for DR2
  * 26-27   | Size of DR2 Breakpoint
  * 28-29   | Conditions for DR3
  * 30-31   | Size of DR3 Breakpoint
  * ======================================================================================================
  */

/* **************** Test Registers
  *
  * ======================================================================================================
  * Name	      | Description
  * ======================================================================================================
  * TR3 - TR5  |	Undocumented
  * TR6	      | Test Command Register
  * TR7	      | Test Data Register
  * ======================================================================================================
  */

// Protected Mode Registers

/* **************** GDTR
  *
  * =======================================================================================================
  * Operand Size                 | Label	| Description
  * =======================================================================================================
  * 64-bit       |     32-bit    |       |
  * ===========================  |       |
  *          Bits 0-15           | Limit	| Size of GDT
  * ===========================  |       |
  * Bits 16-79   | Bits 16-47    | Base  | Starting Address of GDT
  * =======================================================================================================
  */

/* **************** LDTR
  * Stores the segment selector of the LDT.
  *
  * ======================================================================================================
  */

/* **************** TR
  * Stores the segment selector of the TSS.
  *
  * ======================================================================================================
  */

/* **************** IDTR
  *
  * =======================================================================================================
  * Operand Size                 | Label	| Description
  * =======================================================================================================
  * 64-bit       |     32-bit    |       |
  * ===========================  |       |
  *          Bits 0-15           | Limit	| Size of IDT
  * ===========================  |       |
  * Bits 16-79   | Bits 16-47    | Base  | Starting Address of IDT
  * =======================================================================================================
  */

int
get_control_register_code(const char* reg);
char
get_register_code(const char* reg);
int
get_register_type(const char* reg);
int
get_register_size(const char* reg);

#endif // REGISTERS_H
