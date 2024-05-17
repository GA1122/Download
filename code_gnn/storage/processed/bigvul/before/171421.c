static void handle_request(int fd) {
  ALOGV("handle_request(%d)\n", fd);

 ScopedFd closer(fd);
 debugger_request_t request;
  memset(&request, 0, sizeof(request));
 int status = read_request(fd, &request);
 if (status != 0) {
 return;
 }

  ALOGW("debuggerd: handling request: pid=%d uid=%d gid=%d tid=%d\n", request.pid, request.uid,
        request.gid, request.tid);

#if defined(__LP64__)
 if (is32bit(request.tid)) {
 if (request.action == DEBUGGER_ACTION_DUMP_BACKTRACE ||
        request.action == DEBUGGER_ACTION_DUMP_TOMBSTONE) {
      redirect_to_32(fd, &request);
 } else {
      ALOGE("debuggerd: Not allowed to redirect action %d to 32 bit debuggerd\n", request.action);
 }
 return;
 }
#endif

 pid_t fork_pid = fork();
 if (fork_pid == -1) {
    ALOGE("debuggerd: failed to fork: %s\n", strerror(errno));
 } else if (fork_pid == 0) {
    worker_process(fd, request);
 } else {
    monitor_worker_process(fork_pid, request);
 }
}
