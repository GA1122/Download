void InspectorTraceEvents::Did(const probe::CallFunction& probe) {
  if (probe.depth)
    return;
  TRACE_EVENT_END0("devtools.timeline", "FunctionCall");
  TRACE_EVENT_INSTANT1(TRACE_DISABLED_BY_DEFAULT("devtools.timeline"),
                       "UpdateCounters", TRACE_EVENT_SCOPE_THREAD, "data",
                       InspectorUpdateCountersEvent::Data());
}
