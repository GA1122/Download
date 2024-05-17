void InspectorTraceEvents::Will(const probe::ParseHTML& probe) {
  TRACE_EVENT_BEGIN1(
      "devtools.timeline", "ParseHTML", "beginData",
      InspectorParseHtmlBeginData(probe.parser->GetDocument(),
                                  probe.parser->LineNumber().ZeroBasedInt()));
}
