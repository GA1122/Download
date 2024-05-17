static bool ExecuteForwardDelete(LocalFrame& frame,
                                 Event*,
                                 EditorCommandSource source,
                                 const String&) {
  EditingState editing_state;
  switch (source) {
    case kCommandFromMenuOrKeyBinding:
      frame.GetEditor().DeleteWithDirection(
          DeleteDirection::kForward, TextGranularity::kCharacter, false, true);
      return true;
    case kCommandFromDOM:
      DCHECK(frame.GetDocument());
      TypingCommand::ForwardDeleteKeyPressed(*frame.GetDocument(),
                                             &editing_state);
      if (editing_state.IsAborted())
        return false;
      return true;
  }
  NOTREACHED();
  return false;
}
