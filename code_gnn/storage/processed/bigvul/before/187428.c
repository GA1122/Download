 static void ptrace_siblings(pid_t pid, pid_t main_tid, std::set<pid_t>& tids) {
  char task_path[64];
  
// static bool ptrace_attach_thread(pid_t pid, pid_t tid) {
//   if (ptrace(PTRACE_ATTACH, tid, 0, 0) != 0) {
//     return false;
//   }
  
  snprintf(task_path, sizeof(task_path), "/proc/%d/task", pid);
//    
//   if (!pid_contains_tid(pid, tid)) {
//     if (ptrace(PTRACE_DETACH, tid, 0, 0) != 0) {
//       ALOGE("debuggerd: failed to detach from thread '%d'", tid);
//       exit(1);
//     }
//     return false;
//   }
// 
//   return true;
// }
// 
// static void ptrace_siblings(pid_t pid, pid_t main_tid, std::set<pid_t>& tids) {
//   char task_path[PATH_MAX];
// 
//   if (snprintf(task_path, PATH_MAX, "/proc/%d/task", pid) >= PATH_MAX) {
//     ALOGE("debuggerd: task path overflow (pid = %d)\n", pid);
//     abort();
//   }
  
    std::unique_ptr<DIR, int (*)(DIR*)> d(opendir(task_path), closedir);
  
  if (!d) {
     ALOGE("debuggerd: failed to open /proc/%d/task: %s", pid, strerror(errno));
  return;
  }
 
  struct dirent* de;
  while ((de = readdir(d.get())) != NULL) {
  if (!strcmp(de->d_name, ".") || !strcmp(de->d_name, "..")) {
  continue;
  }
 
  char* end;
  pid_t tid = strtoul(de->d_name, &end, 10);
  if (*end) {
  continue;
  }
 
  if (tid == main_tid) {
 
        continue;
      }
  
    if (ptrace(PTRACE_ATTACH, tid, 0, 0) < 0) {
//     if (!ptrace_attach_thread(pid, tid)) {
        ALOGE("debuggerd: ptrace attach to %d failed: %s", tid, strerror(errno));
        continue;
      }
 
     tids.insert(tid);
  }
 }