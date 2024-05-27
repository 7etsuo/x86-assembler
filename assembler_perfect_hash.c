/** x86 assembler
 * https://www.x.com/7etsuo
 */

#include <string.h>

#include "./include/assembler_perfect_hash.h"

static int REG_T1[] = { 69, 46, 29, 103, 23, 85, 59, 81 };

static int REG_T2[] = { 48, 53, 50, 106, 67, 77, 37, 29 };

static int REG_G[] = { 0, 40, 7, 0, 0, 0, 0, 75, 33, 0, 0, 0, 0, 25, 44, 31,
  0, 48, 13, 0, 13, 50, 55, 0, 10, 0, 81, 75, 50, 33, 56, 0, 0, 68, 0, 104,
  20, 99, 103, 0, 2, 0, 0, 39, 98, 18, 20, 45, 0, 0, 0, 39, 15, 77, 0, 0,
  0, 0, 0, 26, 0, 28, 7, 94, 34, 104, 10, 49, 0, 74, 7, 0, 0, 42, 27, 57,
  14, 62, 9, 57, 3, 95, 0, 75, 99, 0, 0, 0, 34, 14, 23, 12, 50, 93, 21, 0,
  0, 14, 52, 25, 60, 15, 106, 55, 0, 6, 0 };

static char* REG_K[] = { "rax", "rbx", "rcx", "rdx", "rsi", "rdi", "rsp", "rbp",
  "r8", "r9", "r10", "r11", "r12", "r13", "r14", "r15", "eax", "ebx",
  "ecx", "edx", "esi", "edi", "esp", "ebp", "r8d", "r9d", "r10d", "r11d",
  "r14d", "r15d", "ax", "bx", "cx", "dx", "si", "di", "sp", "bp", "r8w",
  "r9w", "r10w", "r11w", "r12w", "r13w", "r14w", "r15w", "al", "cl", "dl",
  "bl", "ah", "ch", "dh", "bh", "r13b", "rip", "eip", "ip", "cs", "ds",
  "ss", "es", "fs", "gs", "dwordptr", "dword", "wordptr", "word",
  "byteptr", "byte" };

static int SYS_T1[] = { 512, 620, 679, 778, 51, 177, 35, 53, 191, 60, 848, 66,
  121, 132, 54, 680, 137, 697, 704, 638, 233, 752, 128, 619, 104, 480, 93 };

static int SYS_T2[] = { 529, 367, 656, 221, 110, 49, 550, 598, 16, 244, 419,
  137, 459, 356, 497, 635, 225, 611, 764, 332, 454, 773, 455, 127, 797,
  258, 464 };

static int SYS_G[] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 830, 0, 0, 0, 0, 0,
  0, 124, 0, 0, 0, 571, 0, 132, 0, 37, 0, 0, 0, 0, 518, 54, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 805, 0, 0, 0, 29, 248, 0, 612, 0, 0, 0, 144, 0, 0, 84, 0,
  0, 0, 0, 0, 449, 119, 0, 0, 0, 114, 0, 0, 0, 122, 0, 0, 259, 0, 0, 0, 0,
  524, 0, 0, 0, 0, 0, 0, 214, 0, 0, 275, 343, 0, 0, 0, 0, 0, 0, 36, 0, 18,
  0, 0, 0, 0, 0, 0, 449, 29, 0, 506, 0, 0, 0, 0, 0, 537, 295, 0, 453, 0, 0,
  0, 0, 137, 0, 0, 395, 0, 271, 0, 0, 532, 621, 0, 0, 0, 0, 0, 0, 540, 528,
  0, 0, 309, 0, 0, 623, 191, 0, 0, 168, 48, 708, 0, 470, 0, 0, 191, 354, 0,
  0, 836, 282, 0, 148, 345, 0, 123, 0, 206, 0, 659, 0, 0, 0, 0, 0, 0, 0, 0,
  799, 0, 0, 726, 629, 0, 0, 0, 0, 65, 698, 0, 0, 607, 131, 0, 5, 0, 262,
  0, 0, 0, 92, 298, 50, 0, 0, 0, 182, 0, 0, 0, 731, 0, 0, 463, 0, 324, 0,
  0, 0, 397, 0, 0, 294, 0, 140, 751, 0, 803, 74, 0, 0, 0, 0, 0, 38, 0, 0,
  643, 0, 0, 0, 0, 125, 0, 835, 117, 0, 0, 356, 333, 272, 0, 678, 265, 0,
  0, 0, 50, 0, 0, 0, 0, 0, 0, 797, 380, 195, 0, 540, 0, 0, 375, 675, 0, 0,
  0, 0, 0, 0, 0, 0, 160, 0, 374, 176, 232, 0, 0, 0, 0, 387, 0, 0, 372, 109,
  0, 371, 0, 0, 736, 797, 0, 242, 0, 0, 0, 0, 0, 0, 322, 185, 310, 216,
  260, 685, 0, 0, 249, 439, 0, 0, 191, 0, 795, 414, 0, 0, 175, 437, 278,
  454, 0, 160, 0, 1, 0, 267, 0, 238, 47, 763, 147, 789, 0, 0, 0, 596, 0,
  133, 117, 811, 786, 182, 620, 821, 383, 0, 0, 341, 0, 643, 0, 0, 86, 0,
  0, 139, 0, 74, 732, 0, 0, 0, 0, 0, 43, 0, 255, 343, 0, 0, 0, 0, 478, 0,
  0, 0, 0, 0, 0, 410, 85, 0, 312, 0, 695, 0, 0, 848, 526, 0, 140, 187, 82,
  0, 286, 571, 0, 0, 0, 0, 0, 0, 0, 0, 815, 0, 0, 0, 463, 0, 490, 0, 766,
  0, 413, 101, 0, 0, 733, 25, 722, 379, 399, 229, 469, 0, 0, 0, 0, 0, 0, 0,
  132, 0, 783, 37, 0, 622, 0, 176, 241, 289, 261, 0, 0, 0, 209, 281, 176,
  394, 0, 843, 318, 470, 746, 0, 802, 79, 168, 0, 14, 257, 0, 0, 300, 0,
  814, 0, 0, 0, 332, 202, 0, 234, 0, 360, 38, 40, 663, 263, 754, 0, 0, 226,
  812, 0, 397, 204, 54, 394, 332, 0, 0, 0, 335, 0, 0, 0, 0, 0, 251, 312, 0,
  0, 766, 0, 0, 217, 0, 0, 0, 0, 0, 438, 344, 321, 199, 120, 24, 298, 688,
  140, 144, 299, 129, 151, 163, 0, 30, 446, 0, 0, 834, 0, 135, 194, 184,
  186, 172, 0, 9, 140, 0, 0, 80, 675, 756, 0, 406, 0, 0, 33, 268, 655, 0,
  233, 0, 254, 0, 0, 738, 738, 0, 311, 0, 673, 0, 418, 0, 812, 0, 193, 645,
  0, 167, 220, 12, 285, 0, 215, 0, 61, 227, 284, 0, 0, 0, 0, 117, 86, 53,
  0, 0, 221, 265, 390, 73, 0, 716, 0, 136, 351, 519, 521, 394, 0, 370, 653,
  52, 439, 362, 0, 154, 0, 68, 180, 0, 0, 818, 572, 103, 659, 428, 0, 402,
  0, 0, 533, 0, 131, 73, 0, 100, 296, 0, 0, 252, 0, 108, 715, 0, 16, 0, 0,
  450, 0, 0, 0, 818, 641, 168, 280, 329, 69, 748, 0, 672, 176, 662, 0, 66,
  0, 71, 0, 327, 10, 765, 0, 0, 225, 147, 35, 543, 239, 807, 0, 238, 334,
  0, 0, 226, 621, 0, 0, 0, 0, 0, 0, 170, 333, 0, 0, 65, 453, 378, 0, 0,
  346, 0, 591, 99, 229, 360, 802, 766, 0, 0, 0, 361, 81, 141, 385, 525,
  278, 674, 0, 765, 365, 350, 0, 489, 0, 188, 246, 0, 83, 0, 0, 0, 0, 125,
  650, 817, 770, 223, 377, 88, 0, 0, 179, 232, 714, 841, 0, 0, 175, 0, 638,
  829, 253, 0, 0, 0, 0, 0, 0, 161, 342, 0, 0, 128, 328, 404, 0, 417, 630,
  11, 139, 225, 688, 0, 107, 239, 348, 386, 0, 761, 0, 0, 121, 0, 390, 0,
  210, 685, 641, 253, 0, 635, 355, 0, 0, 230, 245, 0, 0, 263, 78, 187, 190,
  0, 0, 803, 0, 0, 0, 0, 0, 0, 335, 0, 440, 0, 447, 78, 317, 363, 0, 0, 0,
  625, 115, 0, 67, 396, 0, 0, 825, 115, 0, 173, 78, 95, 590 };

static char* SYS_K[] = { "sys_restart_syscall", "sys_exit", "sys_fork",
  "sys_read", "sys_write", "sys_open", "sys_close", "sys_waitpid",
  "sys_creat", "sys_link", "sys_unlink", "sys_execve", "sys_chdir",
  "sys_time32", "sys_mknod", "sys_chmod", "sys_lchown16", "sys_break",
  "sys_stat", "sys_lseek", "sys_getpid", "sys_mount", "sys_umount",
  "sys_access", "sys_nice", "sys_ftime", "sys_sync", "sys_kill",
  "sys_rename", "sys_mkdir", "sys_rmdir", "sys_dup", "sys_pipe",
  "sys_times", "sys_prof", "sys_brk", "sys_setgid16", "sys_getgid16",
  "sys_signal", "sys_geteuid16", "sys_getegid16", "sys_acct", "sys_umount",
  "sys_lock", "sys_ioctl", "sys_fcntl", "sys_mpx", "sys_setpgid",
  "sys_ulimit", "sys_olduname", "sys_umask", "sys_chroot", "sys_ustat",
  "sys_dup2", "sys_sigpending", "sys_sethostname", "sys_setrlimit",
  "sys_old_getrlimit", "sys_getrusage", "sys_gettimeofday",
  "sys_settimeofday", "sys_getgroups16", "sys_setgroups16",
  "sys_old_select", "sys_symlink", "sys_lstat", "sys_readlink",
  "sys_uselib", "sys_swapon", "sys_reboot", "sys_old_readdir",
  "sys_old_mmap", "sys_munmap", "sys_truncate", "sys_ftruncate",
  "sys_fchmod", "sys_fchown16", "sys_getpriority", "sys_setpriority",
  "sys_profil", "sys_statfs", "sys_fstatfs", "sys_ioperm",
  "sys_socketcall", "sys_syslog", "sys_setitimer", "sys_getitimer",
  "sys_newstat", "sys_newlstat", "sys_newfstat", "sys_uname", "sys_iopl",
  "sys_vm86old", "sys_wait4", "sys_swapoff", "sys_sysinfo", "sys_ipc",
  "sys_fsync", "sys_sigreturn", "sys_clone", "sys_setdomainname",
  "sys_newuname", "sys_modify_ldt", "sys_adjtimex_time32", "sys_mprotect",
  "sys_sigprocmask", "sys_create_module", "sys_init_module",
  "sys_delete_module", "sys_get_kernel_syms", "sys_quotactl",
  "sys_getpgid", "sys_fchdir", "sys_ni_syscall", "sys_sysfs",
  "sys_personality", "sys_afs_syscall", "sys_setfsuid16", "sys_setfsgid16",
  "sys_llseek", "sys_getdents", "sys_select", "sys_flock", "sys_msync",
  "sys_readv", "sys_writev", "sys_getsid", "sys_fdatasync",
  "sys_ni_syscall", "sys_mlock", "sys_munlock", "sys_mlockall",
  "sys_munlockall", "sys_sched_setparam", "sys_sched_getparam",
  "sys_sched_setscheduler", "sys_sched_getscheduler", "sys_sched_yield",
  "sys_sched_get_priority_max", "sys_sched_get_priority_min",
  "sys_sched_rr_get_interval_t", "sys_nanosleep_time32", "sys_mremap",
  "sys_setresuid16", "sys_getresuid16", "sys_vm86", "sys_query_module",
  "sys_poll", "sys_nfsservctl", "sys_setresgid16", "sys_getresgid16",
  "sys_prctl", "sys_rt_sigreturn", "sys_rt_sigaction",
  "sys_rt_sigprocmask", "sys_rt_sigpending", "sys_rt_sigtimedwait_time32",
  "sys_rt_sigqueueinfo", "sys_rt_sigsuspend", "sys_ia32_pread64",
  "sys_ia32_pwrite64", "sys_chown16", "sys_getcwd", "sys_capget",
  "sys_capset", "sys_sigaltstack", "sys_sendfile", "sys_getpmsg",
  "sys_putpmsg", "sys_vfork", "sys_getrlimit", "sys_mmap_pgoff",
  "sys_ia32_truncate64", "sys_ia32_ftruncate64", "sys_stat64",
  "sys_lstat64", "sys_fstat64", "sys_lchown", "sys_getuid", "sys_getgid",
  "sys_geteuid", "sys_getegid", "sys_setreuid", "sys_setregid",
  "sys_getgroups", "sys_setgroups", "sys_fchown", "sys_setresuid",
  "sys_getresuid", "sys_setresgid", "sys_getresgid", "sys_chown",
  "sys_setuid", "sys_setgid", "sys_setfsuid", "sys_setfsgid",
  "sys_pivot_root", "sys_mincore", "sys_madvise", "sys_getdents64",
  "sys_fcntl64", "sys_gettid", "sys_ia32_readahead", "sys_setxattr",
  "sys_lsetxattr", "sys_fsetxattr", "sys_getxattr", "sys_lgetxattr",
  "sys_fgetxattr", "sys_listxattr", "sys_llistxattr", "sys_flistxattr",
  "sys_removexattr", "sys_lremovexattr", "sys_fremovexattr", "sys_tkill",
  "sys_sendfile64", "sys_futex_time32", "sys_sched_setaffinity",
  "sys_sched_getaffinity", "sys_set_thread_area", "sys_get_thread_area",
  "sys_io_setup", "sys_io_destroy", "sys_io_getevents_time32",
  "sys_io_submit", "sys_io_cancel", "sys_ia32_fadvise64", "sys_exit_group",
  "sys_lookup_dcookie", "sys_epoll_create", "sys_epoll_ctl",
  "sys_epoll_wait", "sys_remap_file_pages", "sys_set_tid_address",
  "sys_timer_create", "sys_timer_settime32", "sys_timer_gettime32",
  "sys_timer_getoverrun", "sys_timer_delete", "sys_clock_settime32",
  "sys_clock_gettime32", "sys_clock_getres_time32",
  "sys_clock_nanosleep_time32", "sys_statfs64", "sys_fstatfs64",
  "sys_tgkill", "sys_utimes_time32", "sys_ia32_fadvise64_64",
  "sys_vserver", "sys_mbind", "sys_get_mempolicy", "sys_set_mempolicy",
  "sys_mq_open", "sys_mq_unlink", "sys_mq_timedsend_time32",
  "sys_mq_timedreceive_time32", "sys_mq_notify", "sys_mq_getsetattr",
  "sys_kexec_load", "sys_waitid", "sys_add_key", "sys_request_key",
  "sys_keyctl", "sys_ioprio_set", "sys_ioprio_get", "sys_inotify_init",
  "sys_inotify_add_watch", "sys_inotify_rm_watch", "sys_migrate_pages",
  "sys_openat", "sys_mkdirat", "sys_mknodat", "sys_fchownat",
  "sys_futimesat_time32", "sys_fstatat64", "sys_unlinkat", "sys_renameat",
  "sys_linkat", "sys_symlinkat", "sys_readlinkat", "sys_fchmodat",
  "sys_faccessat", "sys_pselect6_time32", "sys_ppoll_time32",
  "sys_unshare", "sys_set_robust_list", "sys_get_robust_list",
  "sys_splice", "sys_ia32_sync_file_range", "sys_tee", "sys_vmsplice",
  "sys_move_pages", "sys_getcpu", "sys_epoll_pwait",
  "sys_utimensat_time32", "sys_signalfd", "sys_timerfd_create",
  "sys_eventfd", "sys_ia32_fallocate", "sys_timerfd_settime32",
  "sys_timerfd_gettime32", "sys_signalfd4", "sys_eventfd2",
  "sys_epoll_create1", "sys_dup3", "sys_pipe2", "sys_inotify_init1",
  "sys_preadv", "sys_pwritev", "sys_rt_tgsigqueueinfo",
  "sys_perf_event_open", "sys_recvmmsg_time32", "sys_fanotify_init",
  "sys_fanotify_mark", "sys_prlimit64", "sys_name_to_handle_at",
  "sys_clock_adjtime32", "sys_syncfsc", "sys_process_vm_readv",
  "sys_process_vm_writev", "sys_kcmp", "sys_finit_module",
  "sys_sched_setattr", "sys_sched_getattr", "sys_renameat2", "sys_seccomp",
  "sys_getrandom", "sys_memfd_create", "sys_bpf", "sys_execveat",
  "sys_socket", "sys_socketpair", "sys_bind", "sys_connect", "sys_listen",
  "sys_accept4", "sys_getsockopt", "sys_setsockopt", "sys_getsockname",
  "sys_getpeername", "sys_sendto", "sys_sendmsg", "sys_recvfrom",
  "sys_recvmsg", "sys_shutdown", "sys_userfaultfd", "sys_membarrier",
  "sys_mlock2", "sys_copy_file_range", "sys_preadv2", "sys_pwritev2",
  "sys_pkey_mprotect", "sys_pkey_alloc", "sys_pkey_free", "sys_statx",
  "sys_arch_prctl", "sys_io_pgetevents_time32", "sys_rseq", "sys_semget",
  "sys_semctl", "sys_shmget", "sys_shmctl", "sys_shmat", "sys_shmdt",
  "sys_msgget", "sys_msgsnd", "sys_msgrcv", "sys_msgctl",
  "sys_clock_gettime", "sys_clock_settime", "sys_clock_adjtime",
  "sys_clock_getres", "sys_clock_nanosleep", "sys_timer_gettime",
  "sys_timer_settime", "sys_timerfd_gettime", "sys_timerfd_settime",
  "sys_utimensat", "sys_pselect6", "sys_ppoll", "sys_io_pgetevents",
  "sys_recvmmsg", "sys_mq_timedsend", "sys_mq_timedreceive",
  "sys_semtimedop", "sys_rt_sigtimedwait", "sys_futex",
  "sys_sched_rr_get_interval", "sys_pidfd_send_signal",
  "sys_io_uring_setup", "sys_io_uring_enter", "sys_io_uring_register",
  "sys_open_tree", "sys_move_mount", "sys_fsopen", "sys_fsconfig",
  "sys_fsmount", "sys_fspick", "sys_pidfd_open", "sys_clone3",
  "sys_close_range", "sys_openat2", "sys_pidfd_getfd", "sys_faccessat2",
  "sys_process_madvise", "sys_epoll_pwait2", "sys_mount_setattr",
  "sys_quotactl_fd", "sys_landlock_create_ruleset",
  "sys_landlock_add_rule", "sys_landlock_restrict_self",
  "sys_memfd_secret", "sys_process_mrelease" };


static int
sys_hash_g(const char* key, const int* T)
{
  int i, sum = 0;

  for (i = 0; key[i] != '\0'; i++) {
    sum += T[i] * key[i];
    sum %= 849;
  }
  return SYS_G[sum];
}


static int
sys_perfect_hash(const char* key)
{
  if (strlen(key) > 27)
    return 0;

  return (sys_hash_g(key, SYS_T1) + sys_hash_g(key, SYS_T2)) % 849;
}


int
sys_has_key(const char* key)
{
  int hash_value = sys_perfect_hash(key);

  if (hash_value < 414 && strcmp(key, SYS_K[hash_value]) == 0)
    return hash_value;

  return -1;
}


static int
register_hash_g(const char* key, const int* T)
{
  int i, sum = 0;

  for (i = 0; key[i] != '\0'; i++) {
    sum += T[i] * key[i];
    sum %= 107;
  }
  return REG_G[sum];
}

static int
register_perfect_hash(const char* key)
{
  if (strlen(key) > 8)
    return 0;

  return (register_hash_g(key, REG_T1) + register_hash_g(key, REG_T2)) % 107;
}

int
register_has_key(const char* key)
{
  int hash_value = register_perfect_hash(key);

  if (hash_value < 70 && strcmp(key, REG_K[hash_value]) == 0)
    return hash_value;

  return -1;
}
