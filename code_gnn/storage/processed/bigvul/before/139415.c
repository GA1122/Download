static bool ExecuteDeleteToMark(LocalFrame& frame,
                                Event*,
                                EditorCommandSource,
                                const String&) {
  const EphemeralRange mark =
      frame.GetEditor().Mark().ToNormalizedEphemeralRange();
  if (mark.IsNotNull()) {
    frame.Selection().SetSelection(
        SelectionInDOMTree::Builder()
            .SetBaseAndExtent(
                UnionEphemeralRanges(mark, frame.GetEditor().SelectedRange()))
            .Build(),
        FrameSelection::kCloseTyping);
  }
  frame.GetEditor().PerformDelete();
  frame.GetEditor().SetMark(
      frame.Selection().ComputeVisibleSelectionInDOMTreeDeprecated());
  return true;
}
