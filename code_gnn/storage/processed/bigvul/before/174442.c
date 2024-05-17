static void dump_signal_info(log_t* log, pid_t tid, int signal, int si_code) {
 siginfo_t si;
  memset(&si, 0, sizeof(si));
 if (ptrace(PTRACE_GETSIGINFO, tid, 0, &si) == -1) {
    _LOG(log, logtype::HEADER, "cannot get siginfo: %s\n", strerror(errno));
 return;
 }

  si.si_code = si_code;

 char addr_desc[32];  
 if (signal_has_si_addr(signal)) {
    snprintf(addr_desc, sizeof(addr_desc), "%p", si.si_addr);
 } else {
    snprintf(addr_desc, sizeof(addr_desc), "--------");
 }

  _LOG(log, logtype::HEADER, "signal %d (%s), code %d (%s), fault addr %s\n",
       signal, get_signame(signal), si.si_code, get_sigcode(signal, si.si_code), addr_desc);
}
