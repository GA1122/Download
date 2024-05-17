void InspectorTraceEvents::DidFailLoading(unsigned long identifier,
                                          DocumentLoader* loader,
                                          const ResourceError&) {
  TRACE_EVENT_INSTANT1(
      "devtools.timeline", "ResourceFinish", TRACE_EVENT_SCOPE_THREAD, "data",
      InspectorResourceFinishEvent::Data(identifier, 0, true, 0, 0));
}
