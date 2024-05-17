TraceEvent::TraceEvent()
    : duration_(TimeDelta::FromInternalValue(-1)),
      id_(0u),
      category_group_enabled_(NULL),
      name_(NULL),
      thread_id_(0),
      phase_(TRACE_EVENT_PHASE_BEGIN),
      flags_(0) {
  for (int i = 0; i < kTraceMaxNumArgs; ++i)
    arg_names_[i] = NULL;
  memset(arg_values_, 0, sizeof(arg_values_));
}
