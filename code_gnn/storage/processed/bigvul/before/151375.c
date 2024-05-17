void InspectorTraceEvents::Will(const probe::CallFunction& probe) {
  if (probe.depth)
    return;
  TRACE_EVENT_BEGIN1(
      "devtools.timeline", "FunctionCall", "data",
      InspectorFunctionCallEvent::Data(probe.context, probe.function));
}
