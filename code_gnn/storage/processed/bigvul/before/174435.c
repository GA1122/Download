static bool dump_crash(log_t* log, pid_t pid, pid_t tid, int signal, int si_code,
 uintptr_t abort_msg_address, bool dump_sibling_threads,
 int* total_sleep_time_usec) {
 char value[PROPERTY_VALUE_MAX];
  property_get("ro.debuggable", value, "0");
 bool want_logs = (value[0] == '1');

 if (log->amfd >= 0) {
 uint32_t datum = htonl(pid);
    TEMP_FAILURE_RETRY( write(log->amfd, &datum, 4) );
    datum = htonl(signal);
    TEMP_FAILURE_RETRY( write(log->amfd, &datum, 4) );
 }

  _LOG(log, logtype::HEADER,
 "*** *** *** *** *** *** *** *** *** *** *** *** *** *** *** ***\n");
  dump_header_info(log);
  dump_thread_info(log, pid, tid);

 if (signal) {
    dump_signal_info(log, tid, signal, si_code);
 }

 UniquePtr<BacktraceMap> map(BacktraceMap::Create(pid));
 UniquePtr<Backtrace> backtrace(Backtrace::Create(pid, tid, map.get()));
 if (backtrace->Unwind(0)) {
    dump_abort_message(backtrace.get(), log, abort_msg_address);
    dump_thread(backtrace.get(), log);
 }

 if (want_logs) {
    dump_logs(log, pid, 5);
 }

 bool detach_failed = false;
 if (dump_sibling_threads) {
    detach_failed = dump_sibling_thread_report(log, pid, tid, total_sleep_time_usec, map.get());
 }

 if (want_logs) {
    dump_logs(log, pid, 0);
 }

 if (log->amfd >= 0) {
 uint8_t eodMarker = 0;
    TEMP_FAILURE_RETRY( write(log->amfd, &eodMarker, 1) );
    TEMP_FAILURE_RETRY( read(log->amfd, &eodMarker, 1) );
 }

 return detach_failed;
}
