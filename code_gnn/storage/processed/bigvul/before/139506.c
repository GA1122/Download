static bool ExecuteSelectSentence(LocalFrame& frame,
                                  Event*,
                                  EditorCommandSource,
                                  const String&) {
  return ExpandSelectionToGranularity(frame, TextGranularity::kSentence);
}
