static void dump_nearby_maps(BacktraceMap* map, log_t* log, pid_t tid) {
 siginfo_t si;
  memset(&si, 0, sizeof(si));
 if (ptrace(PTRACE_GETSIGINFO, tid, 0, &si)) {
    _LOG(log, logtype::MAPS, "cannot get siginfo for %d: %s\n", tid, strerror(errno));
 return;
 }

 bool has_fault_address = signal_has_si_addr(si.si_signo);
 uintptr_t addr = reinterpret_cast<uintptr_t>(si.si_addr);

  _LOG(log, logtype::MAPS, "\nmemory map: %s\n", has_fault_address ? "(fault address prefixed with --->)" : "");

 if (has_fault_address && (addr < map->begin()->start)) {
    _LOG(log, logtype::MAPS, "--->Fault address falls at %" PRIPTR " before any mapped regions\n", addr);
 }

 BacktraceMap::const_iterator prev = map->begin();
 for (BacktraceMap::const_iterator it = map->begin(); it != map->end(); ++it) {
 if (addr >= (*prev).end && addr < (*it).start) {
      _LOG(log, logtype::MAPS, "--->Fault address falls at %" PRIPTR " between mapped regions\n", addr);
 }
    prev = it;
 bool in_map = has_fault_address && (addr >= (*it).start) && (addr < (*it).end);
    dump_map(log, &*it, in_map);
 }
 if (has_fault_address && (addr >= (*prev).end)) {
    _LOG(log, logtype::MAPS, "--->Fault address falls at %" PRIPTR " after any mapped regions\n", addr);
 }
}
