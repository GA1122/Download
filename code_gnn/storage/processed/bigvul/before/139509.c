static bool ExecuteSetMark(LocalFrame& frame,
                           Event*,
                           EditorCommandSource,
                           const String&) {
  frame.GetEditor().SetMark(
      frame.Selection().ComputeVisibleSelectionInDOMTreeDeprecated());
  return true;
}
