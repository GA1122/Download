void SetCallStack(TracedValue* value) {
  static const unsigned char* trace_category_enabled = 0;
  WTF_ANNOTATE_BENIGN_RACE(&trace_category_enabled, "trace_event category");
  if (!trace_category_enabled)
    trace_category_enabled = TRACE_EVENT_API_GET_CATEGORY_GROUP_ENABLED(
        TRACE_DISABLED_BY_DEFAULT("devtools.timeline.stack"));
  if (!*trace_category_enabled)
    return;
  SourceLocation::Capture()->ToTracedValue(value, "stackTrace");
  v8::Isolate::GetCurrent()->GetCpuProfiler()->CollectSample();
}
