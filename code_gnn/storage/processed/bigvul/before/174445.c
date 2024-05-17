static void dump_thread(Backtrace* backtrace, log_t* log) {
  dump_registers(log, backtrace->Tid());
  dump_backtrace_and_stack(backtrace, log);

  dump_memory_and_code(log, backtrace->Tid());
  dump_nearby_maps(backtrace->GetMap(), log, backtrace->Tid());
}
