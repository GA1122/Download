static bool ExecuteDeleteToEndOfParagraph(LocalFrame& frame,
                                          Event*,
                                          EditorCommandSource,
                                          const String&) {
  frame.GetEditor().DeleteWithDirection(DeleteDirection::kForward,
                                        TextGranularity::kParagraphBoundary,
                                        true, false);
  return true;
}
