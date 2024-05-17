void TraceEvent::EstimateTraceMemoryOverhead(
    TraceEventMemoryOverhead* overhead) {
  overhead->Add("TraceEvent", sizeof(*this));

  if (parameter_copy_storage_)
    overhead->AddRefCountedString(*parameter_copy_storage_.get());

  for (size_t i = 0; i < kTraceMaxNumArgs; ++i) {
    if (arg_types_[i] == TRACE_VALUE_TYPE_CONVERTABLE)
      convertable_values_[i]->EstimateTraceMemoryOverhead(overhead);
  }
}
