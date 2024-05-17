static bool dump_sibling_thread_report(
 log_t* log, pid_t pid, pid_t tid, int* total_sleep_time_usec, BacktraceMap* map) {
 char task_path[64];

  snprintf(task_path, sizeof(task_path), "/proc/%d/task", pid);

 DIR* d = opendir(task_path);
 if (d == NULL) {
    ALOGE("Cannot open /proc/%d/task\n", pid);
 return false;
 }

 bool detach_failed = false;
 struct dirent* de;
 while ((de = readdir(d)) != NULL) {
 if (!strcmp(de->d_name, ".") || !strcmp(de->d_name, "..")) {
 continue;
 }

 char* end;
 pid_t new_tid = strtoul(de->d_name, &end, 10);
 if (*end || new_tid == tid) {
 continue;
 }

 if (ptrace(PTRACE_ATTACH, new_tid, 0, 0) < 0) {
      _LOG(log, logtype::ERROR, "ptrace attach to %d failed: %s\n", new_tid, strerror(errno));
 continue;
 }

 if (wait_for_sigstop(new_tid, total_sleep_time_usec, &detach_failed) == -1) {
 continue;
 }

    log->current_tid = new_tid;
    _LOG(log, logtype::THREAD, "--- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---\n");
    dump_thread_info(log, pid, new_tid);

 UniquePtr<Backtrace> backtrace(Backtrace::Create(pid, new_tid, map));
 if (backtrace->Unwind(0)) {
      dump_thread(backtrace.get(), log);
 }

    log->current_tid = log->crashed_tid;

 if (ptrace(PTRACE_DETACH, new_tid, 0, 0) != 0) {
      _LOG(log, logtype::ERROR, "ptrace detach from %d failed: %s\n", new_tid, strerror(errno));
      detach_failed = true;
 }
 }

  closedir(d);
 return detach_failed;
}
