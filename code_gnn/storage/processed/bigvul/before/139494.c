static bool ExecutePasteGlobalSelection(LocalFrame& frame,
                                        Event*,
                                        EditorCommandSource source,
                                        const String&) {
  if (!CanReadClipboard(frame, source))
    return false;
  if (!frame.GetEditor().Behavior().SupportsGlobalSelection())
    return false;
  DCHECK_EQ(source, kCommandFromMenuOrKeyBinding);

  bool old_selection_mode = Pasteboard::GeneralPasteboard()->IsSelectionMode();
  Pasteboard::GeneralPasteboard()->SetSelectionMode(true);
  frame.GetEditor().Paste(source);
  Pasteboard::GeneralPasteboard()->SetSelectionMode(old_selection_mode);
  return true;
}
