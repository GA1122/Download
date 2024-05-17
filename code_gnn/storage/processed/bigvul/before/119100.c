StackTrace::StackTrace() {

#if !defined(__UCLIBC__)
  count_ = base::saturated_cast<size_t>(backtrace(trace_, arraysize(trace_)));
#else
  count_ = 0;
#endif
}
