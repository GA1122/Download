bool LogMessageHandlerWithTraceEvent(int, const char*, int, size_t,
                                     const std::string&) {
  TRACE_EVENT0("log", "trace_event");
  return false;
}
