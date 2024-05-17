static bool ExecuteDeleteToBeginningOfParagraph(LocalFrame& frame,
                                                Event*,
                                                EditorCommandSource,
                                                const String&) {
  frame.GetEditor().DeleteWithDirection(DeleteDirection::kBackward,
                                        TextGranularity::kParagraphBoundary,
                                        true, false);
  return true;
}
