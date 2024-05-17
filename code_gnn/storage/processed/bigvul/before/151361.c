void InspectorTraceEvents::PaintTiming(Document* document,
                                       const char* name,
                                       double timestamp) {
  TRACE_EVENT_MARK_WITH_TIMESTAMP1("loading,rail,devtools.timeline", name,
                                   TraceEvent::ToTraceTimestamp(timestamp),
                                   "frame", document->GetFrame());
}
