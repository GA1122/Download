static bool EnabledRangeInRichlyEditableText(LocalFrame& frame,
                                             Event*,
                                             EditorCommandSource source) {
  frame.GetDocument()->UpdateStyleAndLayoutIgnorePendingStylesheets();
  if (source == kCommandFromMenuOrKeyBinding &&
      !frame.Selection().SelectionHasFocus())
    return false;
  return frame.Selection()
             .ComputeVisibleSelectionInDOMTreeDeprecated()
             .IsRange() &&
         frame.Selection()
             .ComputeVisibleSelectionInDOMTreeDeprecated()
             .IsContentRichlyEditable();
}
