DEFINE_TRACE(InspectorTraceEvents) {
  visitor->Trace(instrumenting_agents_);
  InspectorAgent::Trace(visitor);
}
