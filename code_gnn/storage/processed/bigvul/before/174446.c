static void dump_thread_info(log_t* log, pid_t pid, pid_t tid) {
 char path[64];
 char threadnamebuf[1024];
 char* threadname = NULL;
 FILE *fp;

  snprintf(path, sizeof(path), "/proc/%d/comm", tid);
 if ((fp = fopen(path, "r"))) {
    threadname = fgets(threadnamebuf, sizeof(threadnamebuf), fp);
    fclose(fp);
 if (threadname) {
 size_t len = strlen(threadname);
 if (len && threadname[len - 1] == '\n') {
        threadname[len - 1] = '\0';
 }
 }
 }
 static const char logd[] = "logd";
 if (!strncmp(threadname, logd, sizeof(logd) - 1)
 && (!threadname[sizeof(logd) - 1] || (threadname[sizeof(logd) - 1] == '.'))) {
    log->should_retrieve_logcat = false;
 }

 char procnamebuf[1024];
 char* procname = NULL;

  snprintf(path, sizeof(path), "/proc/%d/cmdline", pid);
 if ((fp = fopen(path, "r"))) {
    procname = fgets(procnamebuf, sizeof(procnamebuf), fp);
    fclose(fp);
 }

  _LOG(log, logtype::HEADER, "pid: %d, tid: %d, name: %s  >>> %s <<<\n", pid, tid,
       threadname ? threadname : "UNKNOWN", procname ? procname : "UNKNOWN");
}
