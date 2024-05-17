static bool perform_dump(const debugger_request_t& request, int fd, int tombstone_fd,
 BacktraceMap* backtrace_map, const std::set<pid_t>& siblings,
 int* crash_signal, std::string* amfd_data) {
 if (TEMP_FAILURE_RETRY(write(fd, "\0", 1)) != 1) {
    ALOGE("debuggerd: failed to respond to client: %s\n", strerror(errno));
 return false;
 }

 int total_sleep_time_usec = 0;
 while (true) {
 int signal = wait_for_signal(request.tid, &total_sleep_time_usec);
 switch (signal) {
 case -1:
        ALOGE("debuggerd: timed out waiting for signal");
 return false;

 case SIGSTOP:
 if (request.action == DEBUGGER_ACTION_DUMP_TOMBSTONE) {
          ALOGV("debuggerd: stopped -- dumping to tombstone");
          engrave_tombstone(tombstone_fd, backtrace_map, request.pid, request.tid, siblings, signal,
                            request.original_si_code, request.abort_msg_address, amfd_data);
 } else if (request.action == DEBUGGER_ACTION_DUMP_BACKTRACE) {
          ALOGV("debuggerd: stopped -- dumping to fd");
          dump_backtrace(fd, backtrace_map, request.pid, request.tid, siblings, nullptr);
 } else {
          ALOGV("debuggerd: stopped -- continuing");
 if (ptrace(PTRACE_CONT, request.tid, 0, 0) != 0) {
            ALOGE("debuggerd: ptrace continue failed: %s", strerror(errno));
 return false;
 }
 continue;  
 }
 break;

 case SIGABRT:
 case SIGBUS:
 case SIGFPE:
 case SIGILL:
 case SIGSEGV:
#ifdef SIGSTKFLT
 case SIGSTKFLT:
#endif
 case SIGSYS:
 case SIGTRAP:
        ALOGV("stopped -- fatal signal\n");
 *crash_signal = signal;
        engrave_tombstone(tombstone_fd, backtrace_map, request.pid, request.tid, siblings, signal,
                          request.original_si_code, request.abort_msg_address, amfd_data);
 break;

 default:
        ALOGE("debuggerd: process stopped due to unexpected signal %d\n", signal);
 break;
 }
 break;
 }

 return true;
}
