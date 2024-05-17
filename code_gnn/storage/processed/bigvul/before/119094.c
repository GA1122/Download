void StackTrace::OutputToStream(std::ostream* os) const {
  StreamBacktraceOutputHandler handler(os);
  ProcessBacktrace(trace_, count_, &handler);
}
