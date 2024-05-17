void StackTrace::Print() const {

#if !defined(__UCLIBC__)
  PrintBacktraceOutputHandler handler;
  ProcessBacktrace(trace_, count_, &handler);
#endif
}
