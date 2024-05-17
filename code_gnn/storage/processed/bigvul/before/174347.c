Backtrace* Backtrace::Create(pid_t pid, pid_t tid, BacktraceMap* map) {
 if (pid == BACKTRACE_CURRENT_PROCESS) {
    pid = getpid();
 if (tid == BACKTRACE_CURRENT_THREAD) {
      tid = gettid();
 }
 } else if (tid == BACKTRACE_CURRENT_THREAD) {
    tid = pid;
 }

 if (pid == getpid()) {
 return new UnwindCurrent(pid, tid, map);
 } else {
 return new UnwindPtrace(pid, tid, map);
 }
}
