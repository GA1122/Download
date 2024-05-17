static bool ExecuteSwapWithMark(LocalFrame& frame,
                                Event*,
                                EditorCommandSource,
                                const String&) {
  const VisibleSelection& mark = frame.GetEditor().Mark();
  const VisibleSelection& selection =
      frame.Selection().ComputeVisibleSelectionInDOMTreeDeprecated();
  if (mark.IsNone() || selection.IsNone())
    return false;
  frame.Selection().SetSelection(mark.AsSelection());
  frame.GetEditor().SetMark(selection);
  return true;
}
